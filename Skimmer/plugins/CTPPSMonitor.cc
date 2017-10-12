// -*- C++ -*-
//
// Package:    CTPPSMonitor
// Class:      CTPPSMonitor
// 

/**\class CTPPSMonitor CTPPSMonitor.cc CTPPSMonitor/plugins/CTPPSMonitor.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/

//
// Original Author:  Diego Figueiredo
//         Created:  Tue, 06 Jun 2017 14:36:11 GMT
//
//

// system include files
#include "CTPPSMonitor.h"

//
// constructors and destructor
//
CTPPSMonitor::CTPPSMonitor(const edm::ParameterSet &iConfig):
  tokenStatus_      ( consumes< edm::DetSetVector<TotemVFATStatus> >       (iConfig.getParameter<edm::InputTag>( "tagStatus" ) ) ),
  tokenLocalTrack_  ( consumes< edm::DetSetVector<TotemRPLocalTrack> >     (iConfig.getParameter<edm::InputTag>( "tagLocalTrack" ) ) ),
  tokenDigi_        ( consumes< edm::DetSetVector<CTPPSDiamondDigi> >      (iConfig.getParameter<edm::InputTag>( "tagDigi" ) ) ),
  tokenDiamondHit_  ( consumes< edm::DetSetVector<CTPPSDiamondRecHit> >    (iConfig.getParameter<edm::InputTag>( "tagDiamondRecHits" ) ) ),
  tokenDiamondTrack_( consumes< edm::DetSetVector<CTPPSDiamondLocalTrack> >(iConfig.getParameter<edm::InputTag>( "tagDiamondLocalTracks" ) ) ),
  tokenFEDInfo_     ( consumes< std::vector<TotemFEDInfo> >                (iConfig.getParameter<edm::InputTag>( "tagFEDInfo" ) ) ),
  bx_        (iConfig.getUntrackedParameter< std::vector<int> >( "bx") ),
  verbosity_        (iConfig.getUntrackedParameter<unsigned int>( "verbosity", 0 ) ),
  path_             (iConfig.getUntrackedParameter<std::string> ( "path" ) ),
  ufirstHisto_       (iConfig.getParameter<double> ( "ufirstHisto" ) ),
  ulastHisto_       (iConfig.getParameter<double> ( "ulastHisto" ) ),
  reducedPlots_        (iConfig.getUntrackedParameter<unsigned int>( "reducedPlots", 0 ) ),
  createNtuple_        (iConfig.getUntrackedParameter<Bool_t>( "createNtuple", false ) )
{

}

CTPPSMonitor::~CTPPSMonitor()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  delete fs;

}

// ------------ method called for each event  ------------
  void
CTPPSMonitor::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;

  edm::Handle< edm::DetSetVector<TotemVFATStatus> > diamondVFATStatus;
  iEvent.getByToken( tokenStatus_, diamondVFATStatus );

  edm::Handle< edm::DetSetVector<TotemRPLocalTrack> > stripTracks;
  iEvent.getByToken( tokenLocalTrack_, stripTracks );

  edm::Handle< edm::DetSetVector<CTPPSDiamondDigi> > diamondDigis;
  iEvent.getByToken( tokenDigi_, diamondDigis );

  edm::Handle< std::vector<TotemFEDInfo> > fedInfo;
  iEvent.getByToken( tokenFEDInfo_, fedInfo );

  edm::Handle< edm::DetSetVector<CTPPSDiamondRecHit> > diamondRecHits;
  iEvent.getByToken( tokenDiamondHit_, diamondRecHits );

  edm::Handle< edm::DetSetVector<CTPPSDiamondLocalTrack> > diamondLocalTracks;
  iEvent.getByToken( tokenDiamondTrack_, diamondLocalTracks );

  // check validity
  valid = true;
  valid &= diamondVFATStatus.isValid();
  valid &= diamondDigis.isValid();
  valid &= fedInfo.isValid();

  run_number = iEvent.id().run();
  bx_cms = iEvent.bunchCrossing();
  lumi_section = iEvent.luminosityBlock();
  orbit = iEvent.orbitNumber();

  double arm0_plane1=0;
  double arm0_plane3=0;
  double arm1_plane1=0;
  double arm1_plane3=0;

  for ( const auto& digis : *diamondDigis ) {
    const CTPPSDiamondDetId detId( digis.detId() );
    for ( const auto& digi : digis ) {

      if(verbosity_){
	std::cout << "Arm:" << detId.arm() << std::endl;
	std::cout << "Station:" << detId.station() << std::endl;
	std::cout << "Rp::" << detId.rp() << std::endl;
	std::cout << "Plane:" << detId.plane() << std::endl;
	std::cout << "Channel:" << detId.channel() << std::endl;
	std::cout << "Multiple Hit:" << digi.getMultipleHit() << std::endl;
	std::cout << "Leading Edge:" << digi.getLeadingEdge() << std::endl;
	std::cout << "Trailing Edge:" << digi.getTrailingEdge() << std::endl;
	std::cout << "Voltage:" << digi.getThresholdVoltage() << std::endl;
	std::cout << "HPTDC Error:" << digi.getHPTDCErrorFlags().getErrorFlag() << std::endl;
      }

      // Avoid HPTDC Multiple Hits
      if(digi.getMultipleHit()>0) continue;

      // Only clock
      if ( detId.channel() == CHANNEL_OF_VFAT_CLOCK){
	if ( digi.getLeadingEdge() != 0 ){
	  hVector_h_clock_leading.at(detId.arm()).at(detId.plane())->Fill(HPTDC_BIN_WIDTH_NS * digi.getLeadingEdge());
	  hVector_h_clock_trailing.at(detId.arm()).at(detId.plane())->Fill(HPTDC_BIN_WIDTH_NS * digi.getTrailingEdge());
	  if(detId.arm()==0 && detId.plane()==1) arm0_plane1 = HPTDC_BIN_WIDTH_NS * digi.getLeadingEdge();
	  if(detId.arm()==0 && detId.plane()==3) arm0_plane3 = HPTDC_BIN_WIDTH_NS * digi.getLeadingEdge();
	  if(detId.arm()==1 && detId.plane()==1) arm1_plane1 = HPTDC_BIN_WIDTH_NS * digi.getLeadingEdge();
	  if(detId.arm()==1 && detId.plane()==3) arm1_plane3 = HPTDC_BIN_WIDTH_NS * digi.getLeadingEdge();
	}
	// For all other channels     
      }else{
	if(digi.getLeadingEdge()==0 || digi.getTrailingEdge()==0) continue;
	if(bx_.size()>0){
	  for (std::vector<std::string>::size_type i=0; i<bx_.size(); i++){
	    if(bx_cms!=bx_[i]) continue;
	    hVector_h_ch_getLeading_lumisection.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill(lumi_section, digi.getLeadingEdge()*HPTDC_BIN_WIDTH_NS);
	    hVector_h_ch_getLeading.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill(digi.getLeadingEdge()*HPTDC_BIN_WIDTH_NS);
	    hVector_h_ch_getTrailing.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill(digi.getTrailingEdge()*HPTDC_BIN_WIDTH_NS);
	    if(ufirstHisto_ >= 0 && ulastHisto_ >=0 && digi.getLeadingEdge()>=ufirstHisto_ && digi.getLeadingEdge()<ulastHisto_){
	      hVector_h_ch_getLeading_deltat.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill((digi.getTrailingEdge()-digi.getLeadingEdge())*HPTDC_BIN_WIDTH_NS, digi.getLeadingEdge()*HPTDC_BIN_WIDTH_NS);
	      hVector_h_ch_deltat.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill((digi.getTrailingEdge()-digi.getLeadingEdge())*HPTDC_BIN_WIDTH_NS);
	      hVector_h_ch_mean_getLeading_lumisection.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill(lumi_section, digi.getLeadingEdge()*HPTDC_BIN_WIDTH_NS);
	    }else{
	      hVector_h_ch_getLeading_deltat.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill((digi.getTrailingEdge()-digi.getLeadingEdge())*HPTDC_BIN_WIDTH_NS, digi.getLeadingEdge()*HPTDC_BIN_WIDTH_NS);
	      hVector_h_ch_deltat.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill((digi.getTrailingEdge()-digi.getLeadingEdge())*HPTDC_BIN_WIDTH_NS);
	      hVector_h_ch_mean_getLeading_lumisection.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill(lumi_section, digi.getLeadingEdge()*HPTDC_BIN_WIDTH_NS);
	    }
	  }
	}else{
	  hVector_h_ch_getLeading_lumisection.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill(lumi_section, digi.getLeadingEdge()*HPTDC_BIN_WIDTH_NS);
	  hVector_h_ch_getLeading.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill(digi.getLeadingEdge()*HPTDC_BIN_WIDTH_NS);
	  hVector_h_ch_getTrailing.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill(digi.getTrailingEdge()*HPTDC_BIN_WIDTH_NS);
	  if(ufirstHisto_ >= 0 && ulastHisto_ >=0 && digi.getLeadingEdge()>=ufirstHisto_ && digi.getLeadingEdge()<ulastHisto_){
	    hVector_h_ch_getLeading_deltat.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill((digi.getTrailingEdge()-digi.getLeadingEdge())*HPTDC_BIN_WIDTH_NS, digi.getLeadingEdge()*HPTDC_BIN_WIDTH_NS);
	    hVector_h_ch_deltat.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill((digi.getTrailingEdge()-digi.getLeadingEdge())*HPTDC_BIN_WIDTH_NS);
	    hVector_h_ch_mean_getLeading_lumisection.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill(lumi_section, digi.getLeadingEdge()*HPTDC_BIN_WIDTH_NS);
	  }else{
	    hVector_h_ch_getLeading_deltat.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill((digi.getTrailingEdge()-digi.getLeadingEdge())*HPTDC_BIN_WIDTH_NS, digi.getLeadingEdge()*HPTDC_BIN_WIDTH_NS);
	    hVector_h_ch_deltat.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill((digi.getTrailingEdge()-digi.getLeadingEdge())*HPTDC_BIN_WIDTH_NS);
	    hVector_h_ch_mean_getLeading_lumisection.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill(lumi_section, digi.getLeadingEdge()*HPTDC_BIN_WIDTH_NS);
	  }
	}
      }
    }
  }

  if(verbosity_){
    std::cout << "\nClock Injected at CH30:" << std::endl;
    std::cout << "Arm0, Plane1: " << arm0_plane1 << std::endl;
    std::cout << "Arm0, Plane3: " << arm0_plane3 << std::endl;
    std::cout << "Arm1, Plane1: " << arm1_plane1 << std::endl;
    std::cout << "Arm1, Plane3: " << arm1_plane3 << std::endl;
  }

  if(arm0_plane1 != 0 && arm0_plane3 != 0){
    hVector_combination1D.at(0)->Fill(arm0_plane1 - arm0_plane3);
    hVector_combination2D.at(0)->Fill(arm0_plane1 - arm0_plane3, arm0_plane1);
  }
  if(arm1_plane1 != 0 && arm1_plane3 != 0){
    hVector_combination1D.at(1)->Fill(arm1_plane1 - arm1_plane3);
    hVector_combination2D.at(1)->Fill(arm1_plane1 - arm1_plane3, arm1_plane1);
  }
  if(arm0_plane1 != 0 && arm1_plane1 != 0){
    hVector_combination1D.at(2)->Fill(arm0_plane1 - arm1_plane1);
    hVector_combination2D.at(2)->Fill(arm0_plane1 - arm1_plane1, arm0_plane1);
  }
  if(arm0_plane1 != 0 && arm1_plane3 != 0){
    hVector_combination1D.at(3)->Fill(arm0_plane1 - arm1_plane3);
    hVector_combination2D.at(3)->Fill(arm0_plane1 - arm1_plane3, arm0_plane1);
  }
  if(arm0_plane3 != 0 && arm1_plane1 != 0){
    hVector_combination1D.at(4)->Fill(arm0_plane3 - arm1_plane1);
    hVector_combination2D.at(4)->Fill(arm0_plane3 - arm1_plane1, arm0_plane3);
  }
  if(arm0_plane3 != 0 && arm1_plane3 != 0){
    hVector_combination1D.at(5)->Fill(arm0_plane3 - arm1_plane3);
    hVector_combination2D.at(5)->Fill(arm0_plane3 - arm1_plane3, arm0_plane3);
  }

}

// ------------ method called once each job just before starting event loop  ------------
  void 
CTPPSMonitor::beginJob()
{

  if(ufirstHisto_>=125) ufirstHisto_=124;
  if(ulastHisto_>125) ufirstHisto_=125;
  if(ufirstHisto_>ulastHisto_) ufirstHisto_=0;

  TH1::AddDirectory(true);
  TH2::AddDirectory(true);

  TString filename_histo = "outputfile_" + path_ + ".root";
  fs = TFile::Open(filename_histo,"RECREATE");
  char name[300];

  for(int i=0; i<6; i++){
    std::string tag;
    if (i==0) tag="clock_combination_arm0pl1-arm0pl3";
    if (i==1) tag="clock_combination_arm1pl1-arm1pl3";
    if (i==2) tag="clock_combination_arm0pl1-arm1pl1";
    if (i==3) tag="clock_combination_arm0pl1-arm1pl3";
    if (i==4) tag="clock_combination_arm0pl3-arm1pl1";
    if (i==5) tag="clock_combination_arm0pl3-arm1pl3";
    sprintf(name,"%s",tag.c_str());
    TH2F *histo_combination2D = new TH2F(name,";#Deltat [ns]; [ns]",1000, -20, 20, 2000, 0, 50);
    hVector_combination2D.push_back(histo_combination2D);

    if (i==0) tag="clock_combination_deltat_arm0pl1-arm0pl3";
    if (i==1) tag="clock_combination_deltat_arm1pl1-arm1pl3";
    if (i==2) tag="clock_combination_deltat_arm0pl1-arm1pl1";
    if (i==3) tag="clock_combination_deltat_arm0pl1-arm1pl3";
    if (i==4) tag="clock_combination_deltat_arm0pl3-arm1pl1";
    if (i==5) tag="clock_combination_deltat_arm0pl3-arm1pl3";
    sprintf(name,"%s",tag.c_str());
    TH1F *histo_combination1D = new TH1F(name,";#Deltat [ns]; N events",1000, -20, 20);
    hVector_combination1D.push_back(histo_combination1D);
  }

  for (UInt_t arm_i = 0; arm_i < CTPPS_NUM_OF_ARMS; ++arm_i){
    std::vector< std::vector<TProfile*> > vec_arm_per_ch_mean_getLeading_lumisection;
    std::vector< std::vector<TH2F*> > vec_arm_per_ch_getLeading_lumisection;
    std::vector< std::vector<TH2F*> > vec_arm_per_ch_getLeading_deltat;
    std::vector< std::vector<TH1D*> > vec_arm_per_ch_getLeading;
    std::vector< std::vector<TH1D*> > vec_arm_per_ch_getTrailing;
    std::vector< std::vector<TH1D*> > vec_arm_per_ch_deltat;

    std::vector<TH1D*> vec_arm_per_pl_result_leading;
    std::vector<TH1D*> vec_arm_per_pl_result_trailing;

    std::vector<TH1D*> vec_arm_clock_leading;
    std::vector<TH1D*> vec_arm_clock_trailing;

    for (UInt_t pl_i = 0; pl_i < CTPPS_DIAMOND_NUM_OF_PLANES; ++pl_i){
      std::vector<TProfile*> vec_ch_mean_getLeading_lumisection;
      std::vector<TH2F*> vec_ch_getLeading_lumisection;
      std::vector<TH2F*> vec_ch_getLeading_deltat;
      std::vector<TH1D*> vec_ch_getLeading;
      std::vector<TH1D*> vec_ch_getTrailing;
      std::vector<TH1D*> vec_ch_deltat;

      sprintf(name,"result_leading_arm%i_pl%i", arm_i, pl_i);
      TH1D *histo_pl_result_leading = new TH1D(name,"; Channel; Signal Fit, Mean [ns]",16,0,16);
      vec_arm_per_pl_result_leading.push_back(histo_pl_result_leading);

      sprintf(name,"result_trailing_arm%i_pl%i", arm_i, pl_i);
      TH1D *histo_pl_result_trailing = new TH1D(name,"; Channel; Signal Fit, Mean [ns]",16,0,16);
      vec_arm_per_pl_result_trailing.push_back(histo_pl_result_trailing);

      sprintf(name,"clock_leading_arm%i_pl%i", arm_i, pl_i);
      TH1D *histo_clock_leading = new TH1D(name,";[ns]; N events",2000,0,50);
      vec_arm_clock_leading.push_back(histo_clock_leading);

      sprintf(name,"clock_trailing_arm%i_pl%i", arm_i, pl_i);
      TH1D *histo_clock_trailing = new TH1D(name,";[ns]; N events",2000,0,50);
      vec_arm_clock_trailing.push_back(histo_clock_trailing);

      for (UInt_t ch_i = 0; ch_i < CTPPS_DIAMOND_NUM_OF_CHANNELS; ++ch_i){
	sprintf(name,"mean_getLeadingVslumisection_arm%i_pl%i_ch%i", arm_i, pl_i, ch_i);
	TProfile *histo_ch_mean_getLeading_lumisection = new TProfile(name,";LS; [ns]",500, 0, 1000);
	vec_ch_mean_getLeading_lumisection.push_back(histo_ch_mean_getLeading_lumisection);

	sprintf(name,"getLeadingVslumisection_arm%i_pl%i_ch%i", arm_i, pl_i, ch_i);
	TH2F *histo_ch_getLeading_lumisection = new TH2F(name,";LS; [ns]",500, 0, 1000, 1500, 0, 200);
	vec_ch_getLeading_lumisection.push_back(histo_ch_getLeading_lumisection);

	sprintf(name,"getLeadingVsDeltat_arm%i_pl%i_ch%i", arm_i, pl_i, ch_i);
	TH2F *histo_ch_getLeading_deltat = new TH2F(name,";Trailing Edge - Leading Edge [ns]; Leading Edge [ns]",500, -20, 20, 1500, 0, 200);
	vec_ch_getLeading_deltat.push_back(histo_ch_getLeading_deltat);

	sprintf(name,"getLeading_arm%i_pl%i_ch%i", arm_i, pl_i, ch_i);
	TH1D *histo_ch_getLeading = new TH1D(name,";[ns]; N events",1500,0,200);
	vec_ch_getLeading.push_back(histo_ch_getLeading);

	sprintf(name,"getTrailing_arm%i_pl%i_ch%i", arm_i, pl_i, ch_i);
	TH1D *histo_ch_getTrailing = new TH1D(name,";[ns]; N events",1500,0,200);
	vec_ch_getTrailing.push_back(histo_ch_getTrailing);

	sprintf(name,"deltat_arm%i_pl%i_ch%i", arm_i, pl_i, ch_i);
	TH1D *histo_ch_deltat = new TH1D(name,"; [ns]; N events",500,-20.,20.);
	vec_ch_deltat.push_back(histo_ch_deltat);
      }
      vec_arm_per_ch_mean_getLeading_lumisection.push_back(vec_ch_mean_getLeading_lumisection);
      vec_arm_per_ch_getLeading_lumisection.push_back(vec_ch_getLeading_lumisection);
      vec_arm_per_ch_getLeading_deltat.push_back(vec_ch_getLeading_deltat);
      vec_arm_per_ch_getLeading.push_back(vec_ch_getLeading);
      vec_arm_per_ch_getTrailing.push_back(vec_ch_getTrailing);
      vec_arm_per_ch_deltat.push_back(vec_ch_deltat);
    }
    hVector_h_ch_mean_getLeading_lumisection.push_back(vec_arm_per_ch_mean_getLeading_lumisection);
    hVector_h_ch_getLeading_lumisection.push_back(vec_arm_per_ch_getLeading_lumisection);
    hVector_h_ch_getLeading_deltat.push_back( vec_arm_per_ch_getLeading_deltat );
    hVector_h_ch_getLeading.push_back( vec_arm_per_ch_getLeading);
    hVector_h_ch_getTrailing.push_back( vec_arm_per_ch_getTrailing);
    hVector_h_ch_deltat.push_back( vec_arm_per_ch_deltat);
    hVector_h_pl_result_leading.push_back(vec_arm_per_pl_result_leading);
    hVector_h_pl_result_trailing.push_back(vec_arm_per_pl_result_trailing);
    hVector_h_clock_leading.push_back( vec_arm_clock_leading);
    hVector_h_clock_trailing.push_back( vec_arm_clock_trailing);
  }

  TString filename_ntuple = "ntuple_" + path_ + ".root";
  fs_tree = TFile::Open(filename_ntuple,"RECREATE");
  fs_tree->cd();
  tree_ = new TTree("calibration","Calibration TTree");

  tree_->Branch("run_number", &brun_number_, "run_number/I");
  tree_->Branch("arm", &barm_, "arm/I");
  tree_->Branch("plane", &bplane_, "plane/I");
  tree_->Branch("channel", &bchannel_, "channel/I");
  tree_->Branch("mean", &bmean_, "mean/D");
  tree_->Branch("sigma", &bsigma_, "sigma/D");
  tree_->Branch("chi2", &bchi2_, "chi2/D");

}

// ------------ method called once each job just after ending the event loop  ------------
  void 
CTPPSMonitor::endJob() 
{

  fs->cd();

  gROOT->SetBatch(kTRUE);
  gSystem->mkdir(path_.c_str());
  gStyle->SetOptFit();

  TString fit_leading = "fit_results_leading_" + path_ + ".txt";
  TString fit_trailing = "fit_results_trailing_" + path_ + ".txt";
  std::ofstream outstring_leading(fit_leading);
  std::ofstream outstring_trailing(fit_trailing);

  gSystem->mkdir((path_+"/LumiSection").c_str());
  gSystem->mkdir((path_+"/LeadingEdge").c_str());
  gSystem->mkdir((path_+"/TrailingEdge").c_str());
  gSystem->mkdir((path_+"/TimeOverThreshold").c_str());
  gSystem->mkdir((path_+"/LeadingAndTrailingEdge").c_str());
  gSystem->mkdir((path_+"/Results").c_str());
  gSystem->mkdir((path_+"/Clock").c_str());

  char *path_cmssw = std::getenv("CMSSW_BASE");

  gSystem->CopyFile((std::string(path_cmssw)+"/src/CTPPSDiamondAnalyzer/Skimmer/doc/index.php").c_str(),(path_+"/index.php").c_str());
  gSystem->CopyFile((std::string(path_cmssw)+"/src/CTPPSDiamondAnalyzer/Skimmer/doc/index.php").c_str(),(path_+"/LumiSection/index.php").c_str());
  gSystem->CopyFile((std::string(path_cmssw)+"/src/CTPPSDiamondAnalyzer/Skimmer/doc/index.php").c_str(),(path_+"/LeadingEdge/index.php").c_str());
  gSystem->CopyFile((std::string(path_cmssw)+"/src/CTPPSDiamondAnalyzer/Skimmer/doc/index.php").c_str(),(path_+"/TrailingEdge/index.php").c_str());
  gSystem->CopyFile((std::string(path_cmssw)+"/src/CTPPSDiamondAnalyzer/Skimmer/doc/index.php").c_str(),(path_+"/TimeOverThreshold/index.php").c_str());
  gSystem->CopyFile((std::string(path_cmssw)+"/src/CTPPSDiamondAnalyzer/Skimmer/doc/index.php").c_str(),(path_+"/LeadingAndTrailingEdge/index.php").c_str());
  gSystem->CopyFile((std::string(path_cmssw)+"/src/CTPPSDiamondAnalyzer/Skimmer/doc/index.php").c_str(),(path_+"/Results/index.php").c_str());
  gSystem->CopyFile((std::string(path_cmssw)+"/src/CTPPSDiamondAnalyzer/Skimmer/doc/index.php").c_str(),(path_+"/Clock/index.php").c_str());

  TCanvas *c1 = new TCanvas("Calibration","",200,10,600,400);

  for(int i=0;i<6;i++){
    hVector_combination2D.at(i)->Write();
    hVector_combination1D.at(i)->Write();
    if(hVector_combination1D.at(i)->GetEntries()>0){
      hVector_combination1D.at(i)->SetStats(111111);
      double max_x = hVector_combination1D.at(i)->GetXaxis()->GetBinCenter(hVector_combination1D.at(i)->GetMaximumBin());
      hVector_combination1D.at(i)->Fit("gaus","","",max_x-0.05*max_x,max_x+0.05*max_x);
    } 
    hVector_combination1D.at(i)->Draw();
    c1->SaveAs(Form("%s/Clock/%s.png",path_.c_str(),hVector_combination1D.at(i)->GetName()));
    c1->Modified();
    c1->Update();
    hVector_combination2D.at(i)->Draw();
    c1->SaveAs(Form("%s/Clock/%s.png",path_.c_str(),hVector_combination2D.at(i)->GetName()));
    c1->Modified();
    c1->Update();
  }

  for (UInt_t arm_i = 0; arm_i < CTPPS_NUM_OF_ARMS; ++arm_i){
    for (UInt_t pl_i = 0; pl_i < CTPPS_DIAMOND_NUM_OF_PLANES; ++pl_i){

      hVector_h_clock_leading.at(arm_i).at(pl_i)->Write();
      hVector_h_clock_trailing.at(arm_i).at(pl_i)->Write();

      for (UInt_t ch_i = 0; ch_i < CTPPS_DIAMOND_NUM_OF_CHANNELS; ++ch_i){
	hVector_h_ch_mean_getLeading_lumisection.at(arm_i).at(pl_i).at(ch_i)->Write();
	hVector_h_ch_getLeading_lumisection.at(arm_i).at(pl_i).at(ch_i)->Write();
	hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->Write();
	hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->Write();
	if(reducedPlots_ > 10) hVector_h_ch_deltat.at(arm_i).at(pl_i).at(ch_i)->Write();

	TLegend* leg = new TLegend(0.7597956,0.822335,0.9931857,0.9949239,NULL,"brNDC");
	leg->SetFillColor(kWhite);
	leg->SetLineColor(kWhite);
	leg->AddEntry(hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i),"Leading Edge","LF");
	leg->AddEntry(hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i),"Trailing Edge","LF");
	hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->SetStats(0);
	hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->SetFillColor(kBlack);
	hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->SetLineColor(kBlack);
	hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->SetLineWidth(1.);
	hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->Draw();
	hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->SetStats(0);
	hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->SetFillColor(kOrange);
	hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->SetLineColor(kOrange);
	hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->SetLineWidth(1.);
	leg->Draw();
	hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->Draw("SAME");
	hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->SetRangeUser(ufirstHisto_, ulastHisto_);
	hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->SetRangeUser(ufirstHisto_, ulastHisto_);
	c1->SaveAs(Form("%s/LeadingAndTrailingEdge/arm%i_pl%i_ch%i.png",path_.c_str(),arm_i, pl_i, ch_i));
	c1->Modified();
	c1->Update();
	delete leg;

	if(hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetEntries()>0){
	  hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->SetStats(111111);
	  if(ufirstHisto_<0 || ulastHisto_<0 || ufirstHisto_==ulastHisto_){
	    double max_x = hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->GetBinCenter(hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetMaximumBin());
	    hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->Fit("gaus","","",max_x-0.25*max_x,max_x+0.25*max_x);
	  }else{
	    hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->SetRangeUser(ufirstHisto_, ulastHisto_);
	    double max_x = hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->GetBinCenter(hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetMaximumBin());
	    hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->Fit("gaus","","",max_x-0.25*max_x,max_x+0.25*max_x);
	  }
	  if(hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")){
	    int migrad = getFitStatus((char *)gMinuit->fCstatu.Data());
	    if(migrad>1){
	      hVector_h_pl_result_leading.at(arm_i).at(pl_i)->SetBinContent(ch_i+1,(double)hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(1));
	      hVector_h_pl_result_leading.at(arm_i).at(pl_i)->SetBinError(ch_i+1,(double)hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParError(1));
	      outstring_leading << "Arm: " << arm_i << "\tPlane: " << pl_i << "\tCh: " << ch_i << "\tMean: " << hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(1) << "\tError: " << hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParError(1) << "\tChi2: " << hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetChisquare() << std::endl;
	      brun_number_ = run_number;
	      barm_ = arm_i;
	      bplane_ = pl_i;
	      bchannel_ = ch_i;
	      bmean_ = hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParameter(1);
	      bsigma_ = hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetParError(1);
	      bchi2_ = hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetFunction("gaus")->GetChisquare();
	      fs_tree->cd();
	      tree_->Fill();
	      fs->cd();
	    }	 
	  }
	}

	if(hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetEntries()>0){
	  hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->SetStats(111111);
	  if(ufirstHisto_<0 || ulastHisto_<0 || ufirstHisto_==ulastHisto_){
	    double max_x = hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->GetBinCenter(hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetMaximumBin());
	    hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->Fit("landau","","",max_x-0.15*max_x,max_x+0.15*max_x);
	  }else{
	    hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->SetRangeUser(ufirstHisto_, ulastHisto_);
	    double max_x = hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->GetBinCenter(hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetMaximumBin());
	    hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->Fit("landau","","",max_x-0.15*max_x,max_x+0.15*max_x);
	  }
	  if(hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetFunction("landau")){
	    int migrad = getFitStatus((char *)gMinuit->fCstatu.Data())  ;
	    if(migrad>1){
	      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->SetBinContent(ch_i+1,(double)hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetFunction("landau")->GetParameter(1));
	      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->SetBinError(ch_i+1,(double)hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetFunction("landau")->GetParError(1));
	      outstring_trailing << "Arm: " << arm_i << "\tPlane: " << pl_i << "\tCh: " << ch_i << "\tMean: " << hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetFunction("landau")->GetParameter(1) << "\tError: " << hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetFunction("landau")->GetParError(1) << "\tChi2: " << hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetFunction("landau")->GetChisquare() << std::endl;
	    }
	  }
	}

	if( hVector_h_ch_deltat.at(arm_i).at(pl_i).at(ch_i)->GetEntries()>0){
	  double max_x =  hVector_h_ch_deltat.at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->GetBinCenter( hVector_h_ch_deltat.at(arm_i).at(pl_i).at(ch_i)->GetMaximumBin());
	  hVector_h_ch_deltat.at(arm_i).at(pl_i).at(ch_i)->Fit("landau","","",max_x-0.05*max_x,max_x+0.05*max_x);
	}

	hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->Draw();
	hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->SetRangeUser(ufirstHisto_, ulastHisto_);
	c1->SaveAs(Form("%s/LeadingEdge/%s.png", path_.c_str(), hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetName()));
	c1->Modified();
	c1->Update();

	hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->Draw();
	hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->SetRangeUser(ufirstHisto_, ulastHisto_);
	c1->SaveAs(Form("%s/TrailingEdge/%s.png", path_.c_str(), hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetName()));
	c1->Modified();
	c1->Update();

	hVector_h_ch_deltat.at(arm_i).at(pl_i).at(ch_i)->Draw();
	c1->SaveAs(Form("%s/TimeOverThreshold/arm%i_pl%i_ch%i.png",path_.c_str(), arm_i, pl_i, ch_i));
	c1->Modified();
	c1->Update();

	hVector_h_ch_getLeading_deltat.at(arm_i).at(pl_i).at(ch_i)->Draw();
	c1->SaveAs(Form("%s/TimeOverThreshold/%s.png", path_.c_str(), hVector_h_ch_getLeading_deltat.at(arm_i).at(pl_i).at(ch_i)->GetName()));
	c1->Modified();
	c1->Update();

	double max_y = hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->GetBinCenter(hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetMaximumBin());
	//int biny_min = hVector_h_ch_getLeading_lumisection.at(arm_i).at(pl_i).at(ch_i)->GetYaxis()->FindBin(ufirstHisto_);
	//int biny_max = hVector_h_ch_getLeading_lumisection.at(arm_i).at(pl_i).at(ch_i)->GetYaxis()->FindBin(ulastHisto_);

	hVector_h_ch_getLeading_lumisection.at(arm_i).at(pl_i).at(ch_i)->GetYaxis()->SetRangeUser(max_y-0.25*max_y,max_y+0.25*max_y);
	hVector_h_ch_getLeading_lumisection.at(arm_i).at(pl_i).at(ch_i)->SetMarkerStyle(20);
	hVector_h_ch_getLeading_lumisection.at(arm_i).at(pl_i).at(ch_i)->SetMarkerSize(.7);
	hVector_h_ch_getLeading_lumisection.at(arm_i).at(pl_i).at(ch_i)->Draw();
	c1->SaveAs(Form("%s/LumiSection/LeadingVsLumiSection_arm%i_pl%i_ch%i.png",path_.c_str(), arm_i, pl_i, ch_i));
	c1->Modified();
	c1->Update();

	hVector_h_ch_mean_getLeading_lumisection.at(arm_i).at(pl_i).at(ch_i)->SetMarkerColor(kRed);
	hVector_h_ch_mean_getLeading_lumisection.at(arm_i).at(pl_i).at(ch_i)->SetMarkerStyle(20);
	hVector_h_ch_mean_getLeading_lumisection.at(arm_i).at(pl_i).at(ch_i)->SetMarkerSize(.7);
	hVector_h_ch_mean_getLeading_lumisection.at(arm_i).at(pl_i).at(ch_i)->Draw();
	c1->SaveAs(Form("%s/LumiSection/LeadingAverageVsLumiSection_arm%i_pl%i_ch%i.png",path_.c_str(), arm_i, pl_i, ch_i));
	c1->Modified();
	c1->Update();

	TF1 *g1 = new TF1("g1","gaus",ufirstHisto_, ulastHisto_);
	g1->SetRange(max_y-0.25*max_y, max_y+0.25*max_y);
	hVector_h_ch_getLeading_lumisection.at(arm_i).at(pl_i).at(ch_i)->FitSlicesY(g1, 0, -1, 0,"QNR", 0);
	TH1F *hVector_h_ch_getLeading_lumisection_slice = (TH1F*)gDirectory->Get(Form("%s_1",hVector_h_ch_getLeading_lumisection.at(arm_i).at(pl_i).at(ch_i)->GetName()));
	hVector_h_ch_getLeading_lumisection_slice->SetMarkerStyle(20);
	hVector_h_ch_getLeading_lumisection_slice->SetMarkerSize(.7);
	hVector_h_ch_getLeading_lumisection_slice->SetName(Form("MeanVsLumisection_fit_arm%i_pl%i_ch%i", arm_i, pl_i, ch_i));
	hVector_h_ch_getLeading_lumisection_slice->Write();
	hVector_h_ch_getLeading_lumisection_slice->Draw();
	c1->SaveAs(Form("%s/LumiSection/MeanVsLumisection_fit_arm%i_pl%i_ch%i.png", path_.c_str(), arm_i, pl_i, ch_i));
	c1->Modified();
	c1->Update();

	TH1F *projh2Y = new TH1F("projectY", ";Fit Result [ns]; N events", 1000, 0, 150);
	for (Int_t i=0;i<hVector_h_ch_getLeading_lumisection_slice->GetNbinsX();i++){
	  if( hVector_h_ch_getLeading_lumisection_slice->GetBinContent(i) > 0) projh2Y->Fill(hVector_h_ch_getLeading_lumisection_slice->GetBinContent(i));
	}

	projh2Y->SetName(Form("ProjectY_MeanVsLumisection_arm%i_pl%i_ch%i", arm_i, pl_i, ch_i));
	projh2Y->Write();
	projh2Y->SetLineColor(kBlue);
	projh2Y->Draw("histo");
	c1->SaveAs(Form("%s/LumiSection/ProjectY_MeanVsLumisection_fit_arm%i_pl%i_ch%i.png", path_.c_str(), arm_i, pl_i, ch_i));
	c1->Modified();
	c1->Update();

      }

      hVector_h_pl_result_leading.at(arm_i).at(pl_i)->Write();
      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->Write();

      TLegend* leg = new TLegend(0.7597956,0.822335,0.9931857,0.9949239,NULL,"brNDC");
      leg->SetFillColor(kWhite);
      leg->SetLineColor(kWhite);
      leg->AddEntry(hVector_h_pl_result_leading.at(arm_i).at(pl_i),"Leading Edge","LF");
      leg->AddEntry(hVector_h_pl_result_trailing.at(arm_i).at(pl_i),"Trailing Edge","LF");
      hVector_h_pl_result_leading.at(arm_i).at(pl_i)->GetYaxis()->SetRangeUser(0.,200.);
      hVector_h_pl_result_leading.at(arm_i).at(pl_i)->SetStats(0);
      hVector_h_pl_result_leading.at(arm_i).at(pl_i)->SetLineColor(kBlack);
      hVector_h_pl_result_leading.at(arm_i).at(pl_i)->SetLineWidth(1.);
      hVector_h_pl_result_leading.at(arm_i).at(pl_i)->SetMarkerColor(kBlack);
      hVector_h_pl_result_leading.at(arm_i).at(pl_i)->Draw("PLC PMC");
      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->GetYaxis()->SetRangeUser(0.,200.);
      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->SetStats(0);
      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->SetLineColor(kOrange);
      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->SetLineWidth(1.);
      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->SetMarkerColor(kOrange);
      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->Draw("SAME PLC PMC");
      leg->Draw();
      c1->SaveAs(Form("%s/Results/fit_results_arm%i_pl%i.png",path_.c_str(), arm_i, pl_i));
      c1->Modified();
      c1->Update();
      delete leg;

      hVector_h_clock_leading.at(arm_i).at(pl_i)->Draw();
      c1->SaveAs(Form("%s/Clock/clock_leading_arm%i_pl%i.png",path_.c_str(), arm_i, pl_i));
      c1->Modified();
      c1->Update();

      hVector_h_clock_trailing.at(arm_i).at(pl_i)->Draw();
      c1->SaveAs(Form("%s/Clock/clock_trailing_arm%i_pl%i.png",path_.c_str(), arm_i, pl_i));
      c1->Modified();
      c1->Update();

    }
  }

  outstring_leading.close();
  outstring_trailing.close();
  fs_tree->Write();
  fs->Close();
  fs_tree->Close();

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CTPPSMonitor::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

int CTPPSMonitor::getFitStatus (char *Migrad)
{
  int i ;
  char Status[][12]={"FAILED","PROBLEMS","CONVERGED","SUCCESSFUL"} ;

  for(i=0 ; i < 5 ; i++)
    if (strstr(Migrad,Status[i]) != NULL) return i ;

  return -1  ;
}

//define this as a plug-in
DEFINE_FWK_MODULE(CTPPSMonitor);
