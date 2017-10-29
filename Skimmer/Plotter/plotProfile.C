#include <stdio.h>
#include <string.h>

void plotProfile(){

  gStyle->SetOptStat(0);

  TFile *f = new TFile("distributions.root");
  TIter next(f->GetListOfKeys());
  TKey *key;

  Double_t w = 1000;
  Double_t h = 800;

  TCanvas *tsector_45 = new TCanvas("sector_45","sector_45", w, h);
  tsector_45->SetWindowSize(w + (w - tsector_45->GetWw()), h + (h - tsector_45->GetWh()));
  tsector_45->Divide(2,2);

  TCanvas *tsector_56 = new TCanvas("sector_56","sector_56", w, h);
  tsector_56->SetWindowSize(w + (w - tsector_56->GetWw()), h + (h - tsector_56->GetWh()));
  tsector_56->Divide(2,2);

  TLegend* leg_pl0 = new TLegend(0.7,0.4,1.,1.,NULL,"brNDC");
  TLegend* leg_pl1 = new TLegend(0.7,0.4,1.,1.,NULL,"brNDC");
  TLegend* leg_pl2 = new TLegend(0.7,0.4,1.,1.,NULL,"brNDC");
  TLegend* leg_pl3 = new TLegend(0.7,0.4,1.,1.,NULL,"brNDC");
  TLegend* leg_pl10 = new TLegend(0.7,0.4,1.,1.,NULL,"brNDC");
  TLegend* leg_pl11 = new TLegend(0.7,0.4,1.,1.,NULL,"brNDC");
  TLegend* leg_pl12 = new TLegend(0.7,0.4,1.,1.,NULL,"brNDC");
  TLegend* leg_pl13 = new TLegend(0.7,0.4,1.,1.,NULL,"brNDC");

  while ((key = (TKey*)next())) {
    TH1* histo=0;
    f->GetObject(key->GetName(), histo);
    string name(key->GetName());

    histo->SetMarkerStyle(1);

    if(name.find("channel0") != string::npos){
      histo->SetMarkerColor(20);
      histo->SetFillColor(20);
      histo->SetLineColor(20);
    }
    if(name.find("channel1") != string::npos){
      histo->SetMarkerColor(kRed);
      histo->SetFillColor(kRed);
      histo->SetLineColor(kRed);
    }
    if(name.find("channel2") != string::npos){
      histo->SetMarkerColor(49);
      histo->SetFillColor(49);
      histo->SetLineColor(49);
    }
    if(name.find("channel3") != string::npos){
      histo->SetMarkerColor(kGreen-10);
      histo->SetLineColor(kGreen-10);
      histo->SetFillColor(kGreen-10);
    }
    if(name.find("channel4") != string::npos){
      histo->SetMarkerColor(kCyan);
      histo->SetLineColor(kCyan);
      histo->SetFillColor(kCyan);
    }
    if(name.find("channel5") != string::npos){
      histo->SetMarkerColor(9);
      histo->SetLineColor(9);
      histo->SetFillColor(9);
    }
    if(name.find("channel6") != string::npos){
      histo->SetMarkerColor(kGray);
      histo->SetLineColor(kGray);
      histo->SetFillColor(kGray);
    }
    if(name.find("channel7") != string::npos){
      histo->SetMarkerColor(kMagenta);
      histo->SetLineColor(kMagenta);
      histo->SetFillColor(kMagenta);
    }
    if(name.find("channel8") != string::npos){
      histo->SetMarkerColor(kAzure);
      histo->SetLineColor(kAzure);
      histo->SetFillColor(kAzure);
    }
    if(name.find("channel9") != string::npos){
      histo->SetMarkerColor(kBlack);
      histo->SetLineColor(kBlack);
      histo->SetFillColor(kBlack);
    }
    if(name.find("channel10") != string::npos){
      histo->SetMarkerColor(kOrange);
      histo->SetLineColor(kOrange);
      histo->SetFillColor(kOrange);
    }
    if(name.find("channel11") != string::npos){
      histo->SetMarkerColor(kSpring);
      histo->SetLineColor(kSpring);
      histo->SetFillColor(kSpring);
    }

    if(histo->GetEntries()==0) continue;

    if(name.find("xy, plane0, ") != string::npos){
      tsector_45->cd(1);
      leg_pl0->AddEntry(histo,name.c_str(), "F");
      histo->GetXaxis()->SetTitle("x [mm]");
      histo->GetYaxis()->SetTitle("y [mm]");
      histo->Draw("SAME");
    }

    if(name.find("xy, plane1, ") != string::npos){
      tsector_45->cd(2);
      leg_pl1->AddEntry(histo,name.c_str(), "F");
      histo->GetXaxis()->SetTitle("x [mm]");
      histo->GetYaxis()->SetTitle("y [mm]");
      histo->Draw("SAME");
    }

    if(name.find("xy, plane2, ") != string::npos){
      tsector_45->cd(3);
      leg_pl2->AddEntry(histo,name.c_str(), "F");
      histo->GetXaxis()->SetTitle("x [mm]");
      histo->GetYaxis()->SetTitle("y [mm]");
      histo->Draw("SAME");
    }

    if(name.find("xy, plane3, ") != string::npos){
      tsector_45->cd(4);
      leg_pl3->AddEntry(histo,name.c_str(), "F");
      histo->GetXaxis()->SetTitle("x [mm]");
      histo->GetYaxis()->SetTitle("y [mm]");
      histo->Draw("SAME");
    }

    if(name.find("xy, plane10, ") != string::npos){
      tsector_56->cd(1);
      leg_pl10->AddEntry(histo,name.c_str(), "F");
      histo->GetXaxis()->SetTitle("x [mm]");
      histo->GetYaxis()->SetTitle("y [mm]");
      histo->Draw("SAME");
    }
    if(name.find("xy, plane11, ") != string::npos){
      tsector_56->cd(2);
      leg_pl11->AddEntry(histo,name.c_str(), "F");
      histo->GetXaxis()->SetTitle("x [mm]");
      histo->GetYaxis()->SetTitle("y [mm]");
      histo->Draw("SAME");
    }

    if(name.find("xy, plane12, ") != string::npos){
      tsector_56->cd(3);
      leg_pl12->AddEntry(histo,name.c_str(), "F");
      histo->GetXaxis()->SetTitle("x [mm]");
      histo->GetYaxis()->SetTitle("y [mm]");
      histo->Draw("SAME");
    }

    if(name.find("xy, plane13, ") != string::npos){
      tsector_56->cd(4);
      leg_pl13->AddEntry(histo,name.c_str(), "F");
      histo->GetXaxis()->SetTitle("x [mm]");
      histo->GetYaxis()->SetTitle("y [mm]");
      histo->Draw("SAME");
    }

  }

  tsector_45->cd(1);
  leg_pl0->Draw();

  tsector_45->cd(2);
  leg_pl1->Draw();

  tsector_45->cd(3);
  leg_pl2->Draw();

  tsector_45->cd(4);
  leg_pl3->Draw();

  tsector_56->cd(1);
  leg_pl10->Draw();

  tsector_56->cd(2);
  leg_pl11->Draw();

  tsector_56->cd(3);
  leg_pl12->Draw();

  tsector_56->cd(4);
  leg_pl13->Draw();

}
