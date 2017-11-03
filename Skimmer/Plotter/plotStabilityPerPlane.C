#include <stdio.h>
#include <string.h>

void plotStabilityPerPlane(){

  gStyle->SetOptStat(0);
  gStyle->SetFrameBorderSize(0);

  TFile *f = new TFile("output_test.root");
  TIter next(f->GetListOfKeys());
  TKey *key;

  Double_t w = 1000;
  Double_t h = 800;

  int bin = 5;

  int xmin0, xmin1, xmin2, xmin3;
  int xmax0, xmax1, xmax2, xmax3;

  int ymin0, ymin1, ymin2, ymin3;
  int ymax0, ymax1, ymax2, ymax3;

  for(int arm_id=0; arm_id<2; arm_id++){
    for(int ch_id=0; ch_id<12; ch_id++){

      TCanvas *canvas_per_arm = new TCanvas("Stability","Stability", w, h);
      std::string histoname_pl0 = Form("meanGetLeadingVslumisectionArm%iPl0Ch%i", arm_id, ch_id);
      std::string histoname_pl1 = Form("meanGetLeadingVslumisectionArm%iPl1Ch%i", arm_id, ch_id);
      std::string histoname_pl2 = Form("meanGetLeadingVslumisectionArm%iPl2Ch%i", arm_id, ch_id);
      std::string histoname_pl3 = Form("meanGetLeadingVslumisectionArm%iPl3Ch%i", arm_id, ch_id);

      canvas_per_arm->SetWindowSize(w + (w - canvas_per_arm->GetWw()), h + (h - canvas_per_arm->GetWh()));
      canvas_per_arm->Divide(2,2);

      std::string title0 = Form("Arm %i, Plane 0, Channel %i", arm_id, ch_id);
      std::string title1 = Form("Arm %i, Plane 1, Channel %i", arm_id, ch_id);
      std::string title2 = Form("Arm %i, Plane 2, Channel %i", arm_id, ch_id);
      std::string title3 = Form("Arm %i, Plane 3, Channel %i", arm_id, ch_id);

      TH1* h0 = 0;
      TH1* h1 = 0;
      TH1* h2 = 0;
      TH1* h3 = 0;

      f->GetObject(histoname_pl0.c_str(), h0);
      f->GetObject(histoname_pl1.c_str(), h1);
      f->GetObject(histoname_pl2.c_str(), h2);
      f->GetObject(histoname_pl3.c_str(), h3);

      if(h0->GetMaximum(100)>ymax0) ymax0 = h0->GetMaximum(100)+0.2*h0->GetMaximum(100);
      if(h0->GetMinimum(0.1)<ymin0) ymin0 = h0->GetMinimum(0.1);
      xmin0 = h0->GetXaxis()->GetBinCenter(h0->FindFirstBinAbove(1));
      xmax0 = h0->GetXaxis()->GetBinCenter(h0->FindLastBinAbove(1));

      if(h1->GetMaximum(100)>ymax1) ymax1 = h1->GetMaximum(100)+0.2*h1->GetMaximum(100);
      if(h1->GetMinimum(0.1)<ymin1) ymin1 = h1->GetMinimum(0.1);
      xmin1 = h1->GetXaxis()->GetBinCenter(h1->FindFirstBinAbove(1));
      xmax1 = h1->GetXaxis()->GetBinCenter(h1->FindLastBinAbove(1));

      if(h2->GetMaximum(100)>ymax2) ymax2 = h2->GetMaximum(100)+0.2*h2->GetMaximum(100);
      if(h2->GetMinimum(0.1)<ymin2) ymin2 = h2->GetMinimum(0.1);
      xmin2 = h2->GetXaxis()->GetBinCenter(h2->FindFirstBinAbove(1));
      xmax2 = h2->GetXaxis()->GetBinCenter(h2->FindLastBinAbove(1));

      if(h3->GetMaximum(100)>ymax3) ymax3 = h3->GetMaximum(100)+0.2*h3->GetMaximum(100);
      if(h3->GetMinimum(0.1)<ymin3) ymin3 = h3->GetMinimum(0.1);
      xmin3 = h3->GetXaxis()->GetBinCenter(h3->FindFirstBinAbove(1));
      xmax3 = h3->GetXaxis()->GetBinCenter(h3->FindLastBinAbove(1));

      h0->SetMarkerStyle(20);
      h0->Rebin(bin);
      h0->GetYaxis()->SetTitleOffset(1.2);
      h0->GetYaxis()->SetTitle("Mean [ns]");
      h0->GetYaxis()->SetRangeUser(ymin0,ymax0);
      h0->GetXaxis()->SetRangeUser(xmin0,xmax0);
      h0->SetMarkerSize(.7);
      h0->SetMarkerColor(kBlue);

      h1->SetMarkerStyle(20);
      h1->Rebin(bin);
      h1->GetYaxis()->SetTitleOffset(1.2);
      h1->GetYaxis()->SetTitle("Mean [ns]");
      h1->GetYaxis()->SetRangeUser(ymin1,ymax1);
      h1->GetXaxis()->SetRangeUser(xmin1,xmax1);
      h1->SetMarkerSize(.7);
      h1->SetMarkerColor(kOrange);

      h2->SetMarkerStyle(20);
      h2->Rebin(bin);
      h2->GetYaxis()->SetTitleOffset(1.2);
      h2->GetYaxis()->SetTitle("Mean [ns]");
      h2->GetYaxis()->SetRangeUser(ymin2,ymax2);
      h2->GetXaxis()->SetRangeUser(xmin2,xmax2);
      h2->SetMarkerSize(.7);
      h2->SetMarkerColor(kSpring);

      h3->SetMarkerStyle(20);
      h3->Rebin(bin);
      h3->GetYaxis()->SetTitleOffset(1.2);
      h3->GetYaxis()->SetTitle("Mean [ns]");
      h3->GetYaxis()->SetRangeUser(ymin3,ymax3);
      h3->GetXaxis()->SetRangeUser(xmin3,xmax3);
      h3->SetMarkerSize(.7);
      h3->SetMarkerColor(kRed);

      canvas_per_arm->cd(1);
      h0->SetTitle(title0.c_str());
      h0->Draw("E1 X0 P");

      canvas_per_arm->cd(2);
      h1->SetTitle(title1.c_str());
      h1->Draw("E1 X0 P");

      canvas_per_arm->cd(3);
      h2->SetTitle(title2.c_str());
      h2->Draw("E1 X0 P");

      canvas_per_arm->cd(4);
      h3->SetTitle(title3.c_str());
      h3->Draw("E1 X0 P");

      canvas_per_arm->Update();

      std::string pngname = Form("meanGetLeadingVsLuminosity_PerPlanesArm%iCh%i.png",  arm_id, ch_id);
      canvas_per_arm->SaveAs(pngname.c_str());

      delete canvas_per_arm;

    }
  }

}
