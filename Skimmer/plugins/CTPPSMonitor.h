#ifndef CTPPSDiamondAnalyzer_CTPPSMonitor_h
#define CTPPSDiamondAnalyzer_CTPPSMonitor_h

// System Inputs
#include <fstream>
#include <memory>
#include <string>

// CMSSW Inputs
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RegexMatch.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// CMSSW Data Formats
#include "DataFormats/CTPPSDigi/interface/TotemVFATStatus.h"
#include "DataFormats/CTPPSDigi/interface/TotemFEDInfo.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSDiamondDigi.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondLocalTrack.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

// ROOT
#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TDirectory.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TMinuit.h>
#include <TF1.h>
#include <TLegend.h>
#include <TStyle.h>

// Constants
#define SEC_PER_LUMI_SECTION 23.31
#define CHANNEL_OF_VFAT_CLOCK 30
#define DISPLAY_RESOLUTION_FOR_HITS_MM 0.1
#define HPTDC_BIN_WIDTH_NS 25./1024
#define CTPPS_NUM_OF_ARMS 2
#define CTPPS_DIAMOND_STATION_ID 1
#define CTPPS_DIAMOND_RP_ID 6
#define CTPPS_NEAR_RP_ID 2
#define CTPPS_FAR_RP_ID 3
#define CTPPS_DIAMOND_NUM_OF_PLANES 4
#define CTPPS_DIAMOND_NUM_OF_CHANNELS 12
#define CTPPS_FED_ID_56 582
#define CTPPS_FED_ID_45 583

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class CTPPSMonitor : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
  public:
    explicit CTPPSMonitor(const edm::ParameterSet&);
    ~CTPPSMonitor();
    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:

    TFile* fs;
    TFile* fs_tree;

    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;
    virtual int  getFitStatus(char *Migrad);
    std::string tagType_;
    edm::EDGetTokenT< edm::DetSetVector<TotemVFATStatus> > tokenStatus_;
    edm::EDGetTokenT< edm::DetSetVector<TotemRPLocalTrack> > tokenLocalTrack_;
    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondDigi> > tokenDigi_;
    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondRecHit> > tokenDiamondHit_;
    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondLocalTrack> > tokenDiamondTrack_;
    edm::EDGetTokenT< std::vector<TotemFEDInfo> > tokenFEDInfo_;
    edm::EDGetTokenT< edm::View<reco::Vertex> > tokenVertexCollection_;
    std::vector<int> bx_;
    unsigned int verbosity_;
    unsigned int runnumber_;
    double ufirstHisto_;
    double ulastHisto_;
    unsigned int reducedPlots_;
    bool createNtuple_;

    bool valid;
    double minlimit;
    double maxlimit;

    unsigned int runnumber;
    int bx_cms;
    int lumi_section;
    int orbit;

    TTree *tree_;
    int brunnumber_;
    int barm_;
    int bplane_;
    int bchannel_;
    double bmean_;
    double bsigma_;
    double bchi2_;

    std::vector<std::vector<std::vector<TProfile*> > > hVector_h_ch_mean_vertexz_lumisection;
    std::vector<std::vector<std::vector<TProfile*> > > hVector_h_ch_mean_getLeading_vertexz;
    std::vector<std::vector<std::vector<TH2F*> > > hVector_h_ch_vertexz_lumisection;

    std::vector<std::vector<std::vector<TProfile*> > > hVector_h_ch_mean_nvertex_lumisection;
    std::vector<std::vector<std::vector<TH2F*> > > hVector_h_ch_nvertex_lumisection;

    std::vector<std::vector<std::vector<TProfile*> > > hVector_h_ch_mean_getLeading_lumisection;
    std::vector<std::vector<std::vector<TH2F*> > > hVector_h_ch_getLeading_lumisection;

    std::vector<std::vector<std::vector<TH2F*> > > hVector_h_ch_getLeading_deltat;
    std::vector<std::vector<std::vector<TH1D*> > > hVector_h_ch_getLeading;
    std::vector<std::vector<std::vector<TH1D*> > > hVector_h_ch_getTrailing;
    std::vector<std::vector<std::vector<TH1D*> > > hVector_h_ch_deltat;

    std::vector<std::vector<TH1D*> >  hVector_h_pl_result_leading;
    std::vector<std::vector<TH1D*> >  hVector_h_pl_result_trailing;
    std::vector<std::vector<TH1D*> >  hVector_h_clock_leading;
    std::vector<std::vector<TH1D*> >  hVector_h_clock_trailing;

    std::vector<TH2F*> hVector_combination2D;
    std::vector<TH1F*> hVector_combination1D;

    std::vector<std::vector<std::vector<TH2F*> > > hVector_h_ch_getLeading_vertexz;

    TProfile* h_mean_leading_nvertex;

    TProfile* h_mean_vertexz_lumisection;
    TProfile* h_mean_nvertex_lumisection;
    TProfile* h_mean_leading_lumisection;
    TProfile* h_mean_leading_vertexz;

    TH2* h_vertexz_lumisection;
    TH2* h_nvertex_lumisection;

    TH2* h_leading_vertexz;
    TH2* h_leading_nvertex;

    TH2F* h_NVertex_NArm0;
    TH2F* h_NVertex_NArm1;
    TH2F* h_NArm0_NArm1;

    TProfile* h_mean_NArm0_lumisection;
    TProfile* h_mean_NArm1_lumisection;

};

#endif
