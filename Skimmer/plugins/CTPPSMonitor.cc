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
  verbosity_        (iConfig.getUntrackedParameter<unsigned int>( "verbosity", 0 ) ),
  path_             (iConfig.getUntrackedParameter<std::string> ( "path" ) )
{

}

CTPPSMonitor::~CTPPSMonitor()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

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

  bx_cms = iEvent.bunchCrossing();
  lumi_section = iEvent.luminosityBlock();
  orbit = iEvent.orbitNumber();

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

      if(detId.channel()==30) continue;
      if(digi.getLeadingEdge()==0 || digi.getTrailingEdge()==0) continue;
      if(digi.getMultipleHit()==1) continue;

      hVector_h_ch_getLeading.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill(digi.getLeadingEdge()*HPTDC_BIN_WIDTH_NS);
      hVector_h_ch_getTrailing.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill(digi.getTrailingEdge()*HPTDC_BIN_WIDTH_NS);
      hVector_h_ch_deltat.at(detId.arm()).at(detId.plane()).at(detId.channel())->Fill((digi.getTrailingEdge()-digi.getLeadingEdge())*HPTDC_BIN_WIDTH_NS);

    }

  }

}


// ------------ method called once each job just before starting event loop  ------------
  void 
CTPPSMonitor::beginJob()
{

  //now do what ever initialization is needed
  usesResource("TFileService");
  edm::Service<TFileService> fs;

  char name[300];
  for (UInt_t arm_i = 0; arm_i < CTPPS_NUM_OF_ARMS; ++arm_i){
    std::vector< std::vector<TH1D*> > vec_arm_per_ch_getLeading;
    std::vector< std::vector<TH1D*> > vec_arm_per_ch_getTrailing;
    std::vector< std::vector<TH1D*> > vec_arm_per_ch_deltat;

    std::vector<TH1D*> vec_arm_per_pl_result_leading;
    std::vector<TH1D*> vec_arm_per_pl_result_trailing;

    for (UInt_t pl_i = 0; pl_i < CTPPS_DIAMOND_NUM_OF_PLANES; ++pl_i){
      std::vector<TH1D*> vec_ch_getLeading;
      std::vector<TH1D*> vec_ch_getTrailing;
      std::vector<TH1D*> vec_ch_deltat;

      sprintf(name,"result_leading_arm%i_pl%i", arm_i, pl_i);
      TH1D *histo_pl_result_leading = new TH1D(name,"; Channel; Signal Fit, Mean [ns]",16,0,16);
      vec_arm_per_pl_result_leading.push_back(histo_pl_result_leading);

      sprintf(name,"result_trailing_arm%i_pl%i", arm_i, pl_i);
      TH1D *histo_pl_result_trailing = new TH1D(name,"; Channel; Signal Fit, Mean [ns]",16,0,16);
      vec_arm_per_pl_result_trailing.push_back(histo_pl_result_trailing);

      for (UInt_t ch_i = 0; ch_i < CTPPS_DIAMOND_NUM_OF_CHANNELS; ++ch_i){
	sprintf(name,"getLeading_arm%i_pl%i_ch%i", arm_i, pl_i, ch_i);
	TH1D *histo_ch_getLeading = new TH1D(name,";[ns]; N events",1500,0,200);
	vec_ch_getLeading.push_back(histo_ch_getLeading);

	sprintf(name,"getTrailing_arm%i_pl%i_ch%i", arm_i, pl_i, ch_i);
	TH1D *histo_ch_getTrailing = new TH1D(name,";[ns]; N events",1500,0,200);
	vec_ch_getTrailing.push_back(histo_ch_getTrailing);

	sprintf(name,"deltat_arm%i_pl%i_ch%i", arm_i, pl_i, ch_i);
	TH1D *histo_ch_deltat = new TH1D(name,"; [ns]; N events",500,-50.,50.);
	vec_ch_deltat.push_back(histo_ch_deltat);
      }
      vec_arm_per_ch_getLeading.push_back(vec_ch_getLeading);
      vec_arm_per_ch_getTrailing.push_back(vec_ch_getTrailing);
      vec_arm_per_ch_deltat.push_back(vec_ch_deltat);
    }
    hVector_h_ch_getLeading.push_back( vec_arm_per_ch_getLeading);
    hVector_h_ch_getTrailing.push_back( vec_arm_per_ch_getTrailing);
    hVector_h_ch_deltat.push_back( vec_arm_per_ch_deltat);
    hVector_h_pl_result_leading.push_back( vec_arm_per_pl_result_leading );
    hVector_h_pl_result_trailing.push_back( vec_arm_per_pl_result_trailing );
  }

}

// ------------ method called once each job just after ending the event loop  ------------
  void 
CTPPSMonitor::endJob() 
{

  gROOT->SetBatch(kTRUE);
  gSystem->mkdir(path_.c_str());

  gSystem->mkdir((path_+"/LeadingEdge").c_str());
  gSystem->mkdir((path_+"/TrailingEdge").c_str());
  gSystem->mkdir((path_+"/TimeOverThreshold").c_str());
  gSystem->mkdir((path_+"/LeadingAndTrailingEdge").c_str());
  gSystem->mkdir((path_+"/Results").c_str());

  char *path_cmssw = std::getenv("CMSSW_BASE");

  gSystem->CopyFile((std::string(path_cmssw)+"/src/CTPPSDiamondAnalyzer/Skimmer/doc/index.php").c_str(),(path_+"/index.php").c_str());
  gSystem->CopyFile((std::string(path_cmssw)+"/src/CTPPSDiamondAnalyzer/Skimmer/doc/index.php").c_str(),(path_+"/LeadingEdge/index.php").c_str());
  gSystem->CopyFile((std::string(path_cmssw)+"/src/CTPPSDiamondAnalyzer/Skimmer/doc/index.php").c_str(),(path_+"/TrailingEdge/index.php").c_str());
  gSystem->CopyFile((std::string(path_cmssw)+"/src/CTPPSDiamondAnalyzer/Skimmer/doc/index.php").c_str(),(path_+"/TimeOverThreshold/index.php").c_str());
  gSystem->CopyFile((std::string(path_cmssw)+"/src/CTPPSDiamondAnalyzer/Skimmer/doc/index.php").c_str(),(path_+"/LeadingAndTrailingEdge/index.php").c_str());
  gSystem->CopyFile((std::string(path_cmssw)+"/src/CTPPSDiamondAnalyzer/Skimmer/doc/index.php").c_str(),(path_+"/Results/index.php").c_str());

  TCanvas *c1 = new TCanvas("Calibration","",200,10,600,400);

  for (UInt_t arm_i = 0; arm_i < CTPPS_NUM_OF_ARMS; ++arm_i){
    for (UInt_t pl_i = 0; pl_i < CTPPS_DIAMOND_NUM_OF_PLANES; ++pl_i){
      for (UInt_t ch_i = 0; ch_i < CTPPS_DIAMOND_NUM_OF_CHANNELS; ++ch_i){
	hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->Write();
	hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->Write();
	hVector_h_ch_deltat.at(arm_i).at(pl_i).at(ch_i)->Write();

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
	c1->SaveAs(Form("%s/LeadingAndTrailingEdge/arm%i_pl%i_ch%i.png",path_.c_str(),arm_i, pl_i, ch_i));
	c1->Modified();
	c1->Update();
	delete leg;

	if(hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetEntries()>0){
	  hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->SetStats(111111);
	  double max_x = hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->GetBinCenter(hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetMaximumBin());
	  hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->Fit("landau","","",max_x-0.02*max_x,max_x+0.02*max_x);
	  int migrad = getFitStatus((char *)gMinuit->fCstatu.Data())  ;
	  if(migrad>1){
	    hVector_h_pl_result_leading.at(arm_i).at(pl_i)->SetBinContent(ch_i+1,hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetFunction("landau")->GetParameter(1));
	    hVector_h_pl_result_leading.at(arm_i).at(pl_i)->SetBinError(ch_i+1,hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetFunction("landau")->GetParameter(2));
	  }
	}

	if(hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetEntries()>0){
	  hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->SetStats(111111);
	  double max_x = hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->GetBinCenter(hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetMaximumBin());
	  hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->Fit("landau","","",max_x-0.02*max_x,max_x+0.02*max_x);
	  int migrad = getFitStatus((char *)gMinuit->fCstatu.Data())  ;
	  if(migrad>1){
	    hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->SetBinContent(ch_i+1,hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetFunction("landau")->GetParameter(1));
	    hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->SetBinError(ch_i+1,hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetFunction("landau")->GetParameter(2));
	  }
	}

	if( hVector_h_ch_deltat.at(arm_i).at(pl_i).at(ch_i)->GetEntries()>0){
	  double max_x =  hVector_h_ch_deltat.at(arm_i).at(pl_i).at(ch_i)->GetXaxis()->GetBinCenter( hVector_h_ch_deltat.at(arm_i).at(pl_i).at(ch_i)->GetMaximumBin());
	  hVector_h_ch_deltat.at(arm_i).at(pl_i).at(ch_i)->Fit("landau","","",max_x-0.02*max_x,max_x+0.02*max_x);
	}

	hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->Draw();
	c1->SaveAs(Form("%s/LeadingEdge/%s.png", path_.c_str(), hVector_h_ch_getLeading.at(arm_i).at(pl_i).at(ch_i)->GetName()));
	c1->Modified();
	c1->Update();

	hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->Draw();
	c1->SaveAs(Form("%s/TrailingEdge/%s.png", path_.c_str(), hVector_h_ch_getTrailing.at(arm_i).at(pl_i).at(ch_i)->GetName()));
	c1->Modified();
	c1->Update();

	hVector_h_ch_deltat.at(arm_i).at(pl_i).at(ch_i)->Draw();
	c1->SaveAs(Form("%s//TimeOverThreshold/arm%i_pl%i_ch%i.png",path_.c_str(), arm_i, pl_i, ch_i));
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
      hVector_h_pl_result_leading.at(arm_i).at(pl_i)->SetMarkerColor(kBlack);
      hVector_h_pl_result_leading.at(arm_i).at(pl_i)->SetMarkerSize(2);
      hVector_h_pl_result_leading.at(arm_i).at(pl_i)->SetMarkerStyle(2);
      hVector_h_pl_result_leading.at(arm_i).at(pl_i)->Draw("E1P");
      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->GetYaxis()->SetRangeUser(0.,200.);
      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->SetStats(0);
      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->SetLineColor(kOrange);
      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->SetMarkerColor(kOrange);
      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->SetMarkerSize(2);
      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->SetMarkerStyle(2);
      hVector_h_pl_result_trailing.at(arm_i).at(pl_i)->Draw("ESAMEEX0");
      leg->Draw();
      c1->SaveAs(Form("%s/Results/fit_results_arm%i_pl_%i.png",path_.c_str(), arm_i, pl_i));
      c1->Modified();
      c1->Update();
      delete leg;
    }
  }

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
