#include <stdio.h>
#include <string.h>

void plotStabilityPerPlaneBothArms(){

  gStyle->SetOptStat(0);

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

  for(int ch_id=0; ch_id<12; ch_id++){

    TCanvas *canvas_per_arm = new TCanvas("Stability","Stability", w, h);
    TLegend* leg = new TLegend(0.75,0.75,0.88,0.865);

    std::string histoname_arm0_pl0 = Form("meanGetLeadingVslumisectionArm0Pl0Ch%i", ch_id);
    std::string histoname_arm0_pl1 = Form("meanGetLeadingVslumisectionArm0Pl1Ch%i", ch_id);
    std::string histoname_arm0_pl2 = Form("meanGetLeadingVslumisectionArm0Pl2Ch%i", ch_id);
    std::string histoname_arm0_pl3 = Form("meanGetLeadingVslumisectionArm0Pl3Ch%i", ch_id);

    std::string histoname_arm1_pl0 = Form("meanGetLeadingVslumisectionArm1Pl0Ch%i", ch_id);
    std::string histoname_arm1_pl1 = Form("meanGetLeadingVslumisectionArm1Pl1Ch%i", ch_id);
    std::string histoname_arm1_pl2 = Form("meanGetLeadingVslumisectionArm1Pl2Ch%i", ch_id);
    std::string histoname_arm1_pl3 = Form("meanGetLeadingVslumisectionArm1Pl3Ch%i", ch_id);

    canvas_per_arm->SetWindowSize(w + (w - canvas_per_arm->GetWw()), h + (h - canvas_per_arm->GetWh()));
    canvas_per_arm->Divide(2,2);

    std::string title0 = Form("Plane 0, Channel %i", ch_id);
    std::string title1 = Form("Plane 1, Channel %i", ch_id);
    std::string title2 = Form("Plane 2, Channel %i", ch_id);
    std::string title3 = Form("Plane 3, Channel %i", ch_id);

    TH1* h0_arm0 = 0;
    TH1* h1_arm0 = 0;
    TH1* h2_arm0 = 0;
    TH1* h3_arm0 = 0;

    TH1* h0_arm1 = 0;
    TH1* h1_arm1 = 0;
    TH1* h2_arm1 = 0;
    TH1* h3_arm1 = 0;

    f->GetObject(histoname_arm0_pl0.c_str(), h0_arm0);
    f->GetObject(histoname_arm0_pl1.c_str(), h1_arm0);
    f->GetObject(histoname_arm0_pl2.c_str(), h2_arm0);
    f->GetObject(histoname_arm0_pl3.c_str(), h3_arm0);

    f->GetObject(histoname_arm1_pl0.c_str(), h0_arm1);
    f->GetObject(histoname_arm1_pl1.c_str(), h1_arm1);
    f->GetObject(histoname_arm1_pl2.c_str(), h2_arm1);
    f->GetObject(histoname_arm1_pl3.c_str(), h3_arm1);

    if(h0_arm0->GetMaximum(100)>ymax0) ymax0 = h0_arm0->GetMaximum(100)+0.2*h0_arm0->GetMaximum(100);
    if(h0_arm0->GetMinimum(0.1)<ymin0) ymin0 = h0_arm0->GetMinimum(0.1);
    xmin0 = h0_arm0->GetXaxis()->GetBinCenter(h0_arm0->FindFirstBinAbove(1));
    xmax0 = h0_arm0->GetXaxis()->GetBinCenter(h0_arm0->FindLastBinAbove(1));

    if(h1_arm0->GetMaximum(100)>ymax1) ymax1 = h1_arm0->GetMaximum(100)+0.2*h1_arm0->GetMaximum(100);
    if(h1_arm0->GetMinimum(0.1)<ymin1) ymin1 = h1_arm0->GetMinimum(0.1);
    xmin1 = h1_arm0->GetXaxis()->GetBinCenter(h1_arm0->FindFirstBinAbove(1));
    xmax1 = h1_arm0->GetXaxis()->GetBinCenter(h1_arm0->FindLastBinAbove(1));

    if(h2_arm0->GetMaximum(100)>ymax2) ymax2 = h2_arm0->GetMaximum(100)+0.2*h2_arm0->GetMaximum(100);
    if(h2_arm0->GetMinimum(0.1)<ymin2) ymin2 = h2_arm0->GetMinimum(0.1);
    xmin2 = h2_arm0->GetXaxis()->GetBinCenter(h2_arm0->FindFirstBinAbove(1));
    xmax2 = h2_arm0->GetXaxis()->GetBinCenter(h2_arm0->FindLastBinAbove(1));

    if(h3_arm0->GetMaximum(100)>ymax3) ymax3 = h3_arm0->GetMaximum(100)+0.2*h3_arm0->GetMaximum(100);
    if(h3_arm0->GetMinimum(0.1)<ymin3) ymin3 = h3_arm0->GetMinimum(0.1);
    xmin3 = h3_arm0->GetXaxis()->GetBinCenter(h3_arm0->FindFirstBinAbove(1));
    xmax3 = h3_arm0->GetXaxis()->GetBinCenter(h3_arm0->FindLastBinAbove(1));

    h0_arm0->SetMarkerStyle(20);
    h0_arm0->Rebin(bin);
    h0_arm0->GetYaxis()->SetTitleOffset(1.2);
    h0_arm0->GetYaxis()->SetTitle("Mean [ns]");
    h0_arm0->GetYaxis()->SetRangeUser(ymin0,ymax0);
    h0_arm0->GetXaxis()->SetRangeUser(xmin0,xmax0);
    h0_arm0->SetMarkerSize(.7);
    h0_arm0->SetMarkerColor(kBlue);
    leg->AddEntry(h0_arm0,"Arm 0","p");

    h0_arm1->SetMarkerStyle(23);
    h0_arm1->Rebin(bin);
    h0_arm1->GetYaxis()->SetTitleOffset(1.2);
    h0_arm1->GetYaxis()->SetTitle("Mean [ns]");
    h0_arm1->GetYaxis()->SetRangeUser(ymin0,ymax0);
    h0_arm1->GetXaxis()->SetRangeUser(xmin0,xmax0);
    h0_arm1->SetMarkerSize(.7);
    h0_arm1->SetMarkerColor(kRed);
    leg->AddEntry(h0_arm1,"Arm 1","p");

    h1_arm0->SetMarkerStyle(20);
    h1_arm0->Rebin(bin);
    h1_arm0->GetYaxis()->SetTitleOffset(1.2);
    h1_arm0->GetYaxis()->SetTitle("Mean [ns]");
    h1_arm0->GetYaxis()->SetRangeUser(ymin1,ymax1);
    h1_arm0->GetXaxis()->SetRangeUser(xmin1,xmax1);
    h1_arm0->SetMarkerSize(.7);
    h1_arm0->SetMarkerColor(kBlue);

    h1_arm1->SetMarkerStyle(23);
    h1_arm1->Rebin(bin);
    h1_arm1->GetYaxis()->SetTitleOffset(1.2);
    h1_arm1->GetYaxis()->SetTitle("Mean [ns]");
    h1_arm1->GetYaxis()->SetRangeUser(ymin1,ymax1);
    h1_arm1->GetXaxis()->SetRangeUser(xmin1,xmax1);
    h1_arm1->SetMarkerSize(.7);
    h1_arm1->SetMarkerColor(kRed);

    h2_arm0->SetMarkerStyle(20);
    h2_arm0->Rebin(bin);
    h2_arm0->GetYaxis()->SetTitleOffset(1.2);
    h2_arm0->GetYaxis()->SetTitle("Mean [ns]");
    h2_arm0->GetYaxis()->SetRangeUser(ymin2,ymax2);
    h2_arm0->GetXaxis()->SetRangeUser(xmin2,xmax2);
    h2_arm0->SetMarkerSize(.7);
    h2_arm0->SetMarkerColor(kBlue);

    h2_arm1->SetMarkerStyle(23);
    h2_arm1->Rebin(bin);
    h2_arm1->GetYaxis()->SetTitleOffset(1.2);
    h2_arm1->GetYaxis()->SetTitle("Mean [ns]");
    h2_arm1->GetYaxis()->SetRangeUser(ymin2,ymax2);
    h2_arm1->GetXaxis()->SetRangeUser(xmin2,xmax2);
    h2_arm1->SetMarkerSize(.7);
    h2_arm1->SetMarkerColor(kRed);

    h3_arm0->SetMarkerStyle(20);
    h3_arm0->Rebin(bin);
    h3_arm0->GetYaxis()->SetTitleOffset(1.2);
    h3_arm0->GetYaxis()->SetTitle("Mean [ns]");
    h3_arm0->GetYaxis()->SetRangeUser(ymin3,ymax3);
    h3_arm0->GetXaxis()->SetRangeUser(xmin3,xmax3);
    h3_arm0->SetMarkerSize(.7);
    h3_arm0->SetMarkerColor(kBlue);

    h3_arm1->SetMarkerStyle(23);
    h3_arm1->Rebin(bin);
    h3_arm1->GetYaxis()->SetTitleOffset(1.2);
    h3_arm1->GetYaxis()->SetTitle("Mean [ns]");
    h3_arm1->GetYaxis()->SetRangeUser(ymin3,ymax3);
    h3_arm1->GetXaxis()->SetRangeUser(xmin3,xmax3);
    h3_arm1->SetMarkerSize(.7);
    h3_arm1->SetMarkerColor(kRed);

    canvas_per_arm->cd(1);
    h0_arm0->SetTitle(title0.c_str());
    h0_arm0->Draw("E1 X0 P");
    h0_arm1->Draw("E1 X0 P SAME");
    leg->Draw();

    canvas_per_arm->cd(2);
    h1_arm0->SetTitle(title1.c_str());
    h1_arm0->Draw("E1 X0 P");
    h1_arm1->Draw("E1 X0 P SAME");
    leg->Draw();

    canvas_per_arm->cd(3);
    h2_arm0->SetTitle(title2.c_str());
    h2_arm0->Draw("E1 X0 P");
    h2_arm1->Draw("E1 X0 P SAME");
    leg->Draw();

    canvas_per_arm->cd(4);
    h3_arm0->SetTitle(title3.c_str());
    h3_arm0->Draw("E1 X0 P");
    h3_arm1->Draw("E1 X0 P SAME");
    leg->Draw();

    canvas_per_arm->Update();

    std::string pngname = Form("meanGetLeadingVsLuminosity_PerPlanesBothArmsCh%i.png", ch_id);
    canvas_per_arm->SaveAs(pngname.c_str());

    delete canvas_per_arm;
    delete leg;

  }

  exit();

}
