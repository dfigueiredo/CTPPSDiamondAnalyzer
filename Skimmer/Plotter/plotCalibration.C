#include <stdio.h>
#include <string.h>

void plotCalibration(){

  gStyle->SetOptStat(0);

  TFile *f1 = new TFile("outputfile303942.root");
  TFile *f2 = new TFile("outputfile303957.root");
  TFile *f3 = new TFile("outputfile303964.root");
  TFile *f4 = new TFile("outputfile303966.root");
  TFile *f5 = new TFile("outputfile303969.root");
  TFile *f6 = new TFile("outputfile303970.root");
  TFile *f7 = new TFile("outputfile303971.root");
  TFile *f8 = new TFile("outputfile303972.root");
  TFile *f9 = new TFile("outputfile303973.root");
  TFile *f10 = new TFile("outputfile303974.root");
  TFile *f11 = new TFile("outputfile303975.root");
  TFile *f12 = new TFile("outputfile303976.root");
  TFile *f13 = new TFile("outputfile303977.root");
  TFile *f14 = new TFile("outputfile303978.root");
  TFile *f15 = new TFile("outputfile303979.root");
  TFile *f16 = new TFile("outputfile303980.root");
  TFile *f17 = new TFile("outputfile303981.root");
  TFile *f18 = new TFile("outputfile303982.root");

  auto c = new TCanvas("c", "c", 1000,800);

  TString histoname = "getLeading_arm1_pl1_ch1"; 
  TH1F * h1 = (TH1F*)f1->Get(histoname);
  h1->SetName("303942"); 
  h1->SetLineColor(1);
  h1->SetLineWidth(2);
 
  TH1F * h2 = (TH1F*)f2->Get(histoname);
  h2->SetName("303957");
  h2->SetLineColor(2);
  h2->SetLineWidth(2);

  TH1F * h3 = (TH1F*)f3->Get(histoname); 
  h3->SetName("303964");
  h3->SetLineColor(3);

  TH1F * h4 = (TH1F*)f4->Get(histoname); 
  h4->SetName("303966");
  h4->SetLineColor(4);

  TH1F * h5 = (TH1F*)f5->Get(histoname); 
  h5->SetName("303969");
  h5->SetLineColor(5);

  TH1F * h6 = (TH1F*)f6->Get(histoname); 
  h6->SetName("303970");
  h6->SetLineColor(6);

  TH1F * h7 = (TH1F*)f7->Get(histoname); 
  h7->SetName("303971");
  h7->SetLineColor(7);

  TH1F * h8 = (TH1F*)f8->Get(histoname); 
  h8->SetName("303972");
  h8->SetLineColor(8);

  TH1F * h9 = (TH1F*)f9->Get(histoname); 
  h9->SetName("303973");
  h9->SetLineColor(9);

  TH1F * h10 = (TH1F*)f10->Get(histoname); 
  h10->SetName("303974");
  h10->SetLineColor(1);
  h10->SetLineStyle(3);

  TH1F * h11 = (TH1F*)f11->Get(histoname); 
  h11->SetName("303975");
  h11->SetLineColor(2);
  h11->SetLineStyle(3);

  TH1F * h12 = (TH1F*)f12->Get(histoname); 
  h12->SetName("303976");
  h12->SetLineColor(3);
  h12->SetLineStyle(3);

  TH1F * h13 = (TH1F*)f13->Get(histoname); 
  h13->SetName("303977");
  h13->SetLineColor(4);
  h13->SetLineStyle(3);

  TH1F * h14 = (TH1F*)f14->Get(histoname); 
  h14->SetName("303978");
  h14->SetLineColor(5);
  h14->SetLineStyle(3);

  TH1F * h15 = (TH1F*)f15->Get(histoname); 
  h15->SetName("303979");
  h15->SetLineColor(6);
  h15->SetLineStyle(3);

  TH1F * h16 = (TH1F*)f16->Get(histoname); 
  h16->SetName("303980");
  h16->SetLineColor(7);
  h16->SetLineStyle(3);

  TH1F * h17 = (TH1F*)f17->Get(histoname); 
  h17->SetName("303981");
  h17->SetLineColor(8);
  h17->SetLineStyle(3);

  TH1F * h18 = (TH1F*)f18->Get(histoname); 
  h18->SetName("303982");
  h18->SetLineColor(9);
  h18->SetLineStyle(3);

  //h1->Draw();
  h2->Draw();
  h3->Draw("same");
  h4->Draw("same");
  h5->Draw("same");
  h6->Draw("same");
  h7->Draw("same");
  h8->Draw("same");
  h9->Draw("same");
  h10->Draw("same");
  h11->Draw("same");
  h12->Draw("same");
  h13->Draw("same");
  h14->Draw("same");
  h15->Draw("same");
  h16->Draw("same");
  h17->Draw("same");
  h18->Draw("same");

  c->BuildLegend();

}
