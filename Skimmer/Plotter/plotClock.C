#include <stdio.h>
#include <string.h>

void plotClock(){

  gStyle->SetOptFit(11111111);
  gStyle->SetStatX(0.88);
  gStyle->SetStatY(0.35);

  TFile *f = new TFile("ntuple_clock.root");
  TTree *cal = (TTree*)f->Get("calibration");

  Double_t w = 1500;
  Double_t h = 500;

  int arm = 0;
  int plane = 0;

  TCanvas *canvas_per_plane = new TCanvas("clock","clock", w, h);
  canvas_per_plane->SetWindowSize(w + (w - canvas_per_plane->GetWw()), h + (h - canvas_per_plane->GetWh()));
  canvas_per_plane->Divide(6,2);

  TCanvas *canvas_results = new TCanvas("results","results", 500, 500);

  TH1F *h1 = new TH1F("results",";p1 from fit; Entries",100,-0.005,0.005);

  for(int i=0; i<12; i++){
    canvas_per_plane->cd(i+1);
    char name[300];
    sprintf(name," arm==%i && plane == %i && channel == %i", arm, plane, i);
    TCut selection = name;
    cal->Draw("run_number:mean:sigma", selection,"*");
    TGraphErrors *gr = new TGraphErrors(calibration->GetSelectedRows(), calibration->GetV1(), calibration->GetV2(), 0, calibration->GetV3());

    double *x = gr->GetX();
    int n = gr->GetN();

    for(int j = 0; j<n;j++){
      if(x[j]==303970) x[j] = 0;
      if(x[j]==303971) x[j] = 72;
      if(x[j]==303972) x[j] = 72*2;
      if(x[j]==303973) x[j] = 73*3;
      if(x[j]==303974) x[j] = 72*4;
      if(x[j]==303975) x[j] = 72*5;
      if(x[j]==303976) x[j] = 72*6;
      if(x[j]==303977) x[j] = 72*7;
      if(x[j]==303978) x[j] = 72*8;
      if(x[j]==303979) x[j] = 72*9;
      if(x[j]==303980) x[j] = 72*10;
      if(x[j]==303981) x[j] = -72*10;
      if(x[j]==303982) x[j] = -72*30;
    }

    TGraphErrors *gr_mod = new TGraphErrors(calibration->GetSelectedRows(), x, calibration->GetV2(), 0, calibration->GetV3());
    char title[300];
    sprintf(title,"Arm %i, Plane %i, Ch %i", arm, plane, i);
    gr_mod->SetTitle(title);
    gr_mod->GetYaxis()->SetTitle("Mean [ns]");
    gr_mod->GetXaxis()->SetTitle("Clock Phase Shift [ps]");
    gr_mod->SetMarkerColor(i);
    gr_mod->SetMarkerStyle(7);
    if(gr_mod->GetN()>0){
      TFitResultPtr r = gr_mod->Fit("pol1","S");
      gr_mod->Draw("AP");
      h1->Fill(r->Parameter(1)); 
    }
  }

  canvas_results->cd();
  h1->Draw();

}
