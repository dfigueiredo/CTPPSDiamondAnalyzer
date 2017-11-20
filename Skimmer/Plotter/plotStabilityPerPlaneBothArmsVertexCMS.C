#include <stdio.h>
#include <string.h>
#include <TFile.h>

// How to Run
// root -l 'plotStabilityPerPlaneBothArmsVertexCMS.C("filename.root")'

void plotStabilityPerPlaneBothArmsVertexCMS(std::string filename){

  gStyle->SetLegendBorderSize(0);

  TFile *f = new TFile(filename.c_str());
  TIter next(f->GetListOfKeys());
  TKey *key;

  Double_t w = 1000;
  Double_t h = 800;

  int bin = 10;
  std::string pic_format="pdf";

  double xmin0, xmin1, xmin2, xmin3;
  double xmax0, xmax1, xmax2, xmax3;
  double ymin0, ymin1, ymin2, ymin3;
  double ymax0, ymax1, ymax2, ymax3;

  bool do_rebin = true;

  for(int ch_id=0; ch_id<12; ch_id++){

    std::string histoname_vertexz = "meanVertexz";
    TH1D* h_vertexz = 0;
    TH1D* h_max_value = new TH1D("max","max", 100, 0, 100);
    TH1D* h_error = new TH1D("statistics","statistics", 100, 0., 0.5);
    f->GetObject(histoname_vertexz.c_str(), h_vertexz);

    if(do_rebin){
      h_vertexz->Rebin(bin);
      h_vertexz->SetMarkerStyle(20);
      h_vertexz->SetMarkerSize(.7);
      h_vertexz->SetMarkerColor(kRed);
      h_vertexz->SetLineColor(kRed);
    }
    do_rebin = false;

    TCanvas *canvas_per_arm = new TCanvas("Stability","Stability", w, h);
    TCanvas *canvas_error = new TCanvas("Error","Error", w, h);

    TPad *pad1_h0 = new TPad("pad1_h0","",0,0,1,1);
    TPad *pad2_h0 = new TPad("pad2_h0","",0,0,1,1);
    TPad *pad1_h1 = new TPad("pad1_h1","",0,0,1,1);
    TPad *pad2_h1 = new TPad("pad2_h1","",0,0,1,1);
    TPad *pad1_h2 = new TPad("pad1_h2","",0,0,1,1);
    TPad *pad2_h2 = new TPad("pad2_h2","",0,0,1,1);
    TPad *pad1_h3 = new TPad("pad1_h3","",0,0,1,1);
    TPad *pad2_h3 = new TPad("pad2_h3","",0,0,1,1);

    pad2_h0->SetFillStyle(4000);
    pad2_h1->SetFillStyle(4000);
    pad2_h2->SetFillStyle(4000);
    pad2_h3->SetFillStyle(4000);

    TLegend* leg = new TLegend(0.55,0.75,0.88,0.865);
    leg->SetFillStyle(0);
    leg->SetTextSize(.05);

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

    TH1D* h0_arm0 = 0;
    TH1D* h1_arm0 = 0;
    TH1D* h2_arm0 = 0;
    TH1D* h3_arm0 = 0;

    TH1D* h0_arm1 = 0;
    TH1D* h1_arm1 = 0;
    TH1D* h2_arm1 = 0;
    TH1D* h3_arm1 = 0;

    f->GetObject(histoname_arm0_pl0.c_str(), h0_arm0);
    f->GetObject(histoname_arm0_pl1.c_str(), h1_arm0);
    f->GetObject(histoname_arm0_pl2.c_str(), h2_arm0);
    f->GetObject(histoname_arm0_pl3.c_str(), h3_arm0);

    f->GetObject(histoname_arm1_pl0.c_str(), h0_arm1);
    f->GetObject(histoname_arm1_pl1.c_str(), h1_arm1);
    f->GetObject(histoname_arm1_pl2.c_str(), h2_arm1);
    f->GetObject(histoname_arm1_pl3.c_str(), h3_arm1);

    h0_arm0->Rebin(bin);
    h1_arm0->Rebin(bin);
    h2_arm0->Rebin(bin);
    h3_arm0->Rebin(bin);

    h0_arm1->Rebin(bin);
    h1_arm1->Rebin(bin);
    h2_arm1->Rebin(bin);
    h3_arm1->Rebin(bin);

    if(h0_arm0->GetMinimum(1)<h0_arm1->GetMinimum(1)){ ymin0 = h0_arm0->GetMinimum(1)-0.05*h0_arm0->GetMinimum(1);}
    else{
      ymin0 = h0_arm1->GetMinimum(1)-0.05*h0_arm1->GetMinimum(1);
    }

    if(h0_arm0->GetMaximum()>h0_arm1->GetMaximum()){ ymax0 = h0_arm0->GetMaximum()+0.05*h0_arm0->GetMaximum();}
    else{
      ymax0 = h0_arm1->GetMaximum()+0.05*h0_arm1->GetMaximum();
    }

    if(h1_arm0->GetMinimum(1)<h1_arm1->GetMinimum(1)){ ymin1 = h1_arm0->GetMinimum(1)-0.05*h1_arm0->GetMinimum(1);}
    else{
      ymin1 = h1_arm1->GetMinimum(1)-0.05*h1_arm1->GetMinimum();
    }

    if(h1_arm0->GetMaximum()>h1_arm1->GetMaximum()){ ymax1 = h1_arm0->GetMaximum()+0.05*h1_arm0->GetMaximum();}
    else{
      ymax1 = h1_arm1->GetMaximum()+0.05*h1_arm1->GetMaximum();
    }

    if(h2_arm0->GetMinimum(1)<h2_arm1->GetMinimum(1)){ ymin2 = h2_arm0->GetMinimum(1)-0.05*h2_arm0->GetMinimum(1);}
    else{
      ymin2 = h2_arm1->GetMinimum(1)-0.05*h2_arm1->GetMinimum(1);
    }

    if(h2_arm0->GetMaximum()>h2_arm1->GetMaximum()){ ymax2 = h2_arm0->GetMaximum()+0.05*h2_arm0->GetMaximum();}
    else{
      ymax2 = h2_arm1->GetMaximum()+0.05*h2_arm1->GetMaximum();
    }

    if(h3_arm0->GetMinimum(1)<h3_arm1->GetMinimum(1)){ ymin3 = h3_arm0->GetMinimum(1)-0.05*h3_arm0->GetMinimum(1);}
    else{
      ymin3 = h3_arm1->GetMinimum(1)-0.05*h3_arm1->GetMinimum(1);
    }

    if(h3_arm0->GetMaximum()>h3_arm1->GetMaximum()){ ymax3 = h3_arm0->GetMaximum()+0.05*h3_arm0->GetMaximum();}
    else{
      ymax3 = h3_arm1->GetMaximum()+0.05*h3_arm1->GetMaximum();
    }

    if(h0_arm0->GetXaxis()->GetBinCenter(h0_arm0->FindLastBinAbove(0))>h0_arm1->GetXaxis()->GetBinCenter(h0_arm0->FindLastBinAbove(0))){ 
      xmax0 = h0_arm0->GetXaxis()->GetBinCenter(h0_arm0->FindLastBinAbove(0));
      h_max_value->SetBinContent(1,xmax0+h0_arm0->GetBinWidth(0)/2.);
    }
    else{
      xmax0 = h0_arm1->GetXaxis()->GetBinCenter(h0_arm0->FindLastBinAbove(0));
      h_max_value->SetBinContent(2,xmax0+h0_arm1->GetBinWidth(0)/2.);
    }

    if(h1_arm0->GetXaxis()->GetBinCenter(h1_arm0->FindLastBinAbove(0))>h1_arm1->GetXaxis()->GetBinCenter(h1_arm0->FindLastBinAbove(0))){ 
      xmax1 = h1_arm0->GetXaxis()->GetBinCenter(h1_arm0->FindLastBinAbove(0));
      h_max_value->SetBinContent(3,xmax1+h1_arm0->GetBinWidth(0)/2.);
    }
    else{
      xmax1 = h1_arm1->GetXaxis()->GetBinCenter(h1_arm0->FindLastBinAbove(0));
      h_max_value->SetBinContent(4,xmax1+h1_arm1->GetBinWidth(0)/2.);
    }

    if(h2_arm0->GetXaxis()->GetBinCenter(h2_arm0->FindLastBinAbove(0))>h2_arm1->GetXaxis()->GetBinCenter(h2_arm0->FindLastBinAbove(0))){
      xmax2 = h2_arm0->GetXaxis()->GetBinCenter(h2_arm0->FindLastBinAbove(0));
      h_max_value->SetBinContent(5,xmax2+h2_arm0->GetBinWidth(0)/2.);
    }
    else{
      xmax2 = h2_arm1->GetXaxis()->GetBinCenter(h2_arm0->FindLastBinAbove(0));
      h_max_value->SetBinContent(6,xmax2+h2_arm1->GetBinWidth(0)/2.);
    }

    if(h3_arm0->GetXaxis()->GetBinCenter(h3_arm0->FindLastBinAbove(0))>h3_arm1->GetXaxis()->GetBinCenter(h3_arm0->FindLastBinAbove(0))){
      xmax3 = h3_arm0->GetXaxis()->GetBinCenter(h3_arm0->FindLastBinAbove(0));
      h_max_value->SetBinContent(7,xmax3+h3_arm0->GetBinWidth(0)/2.);
    }
    else{
      xmax3 = h3_arm1->GetXaxis()->GetBinCenter(h3_arm0->FindLastBinAbove(0));
      h_max_value->SetBinContent(8,xmax3+h3_arm1->GetBinWidth(0)/2.);
    }

    xmin0 = 0;
    xmin1 = 0;
    xmin2 = 0;
    xmin3 = 0;

    double xmin_vertex = 0;
    double xmax_vertex = h_vertexz->GetXaxis()->GetBinCenter(h_vertexz->FindLastBinAbove(0));

    h_vertexz->GetXaxis()->SetRangeUser(xmin_vertex,xmax_vertex);

    h0_arm0->SetStats(0);
    h0_arm0->SetMarkerStyle(20);
    h0_arm0->GetYaxis()->SetTitleOffset(1.2);
    h0_arm0->GetYaxis()->SetTitle("Mean Leading Edge [ns]");
    h0_arm0->GetYaxis()->SetRangeUser(ymin0,ymax0);
    h0_arm0->GetXaxis()->SetRangeUser(xmin0,xmax0);
    h0_arm0->SetMarkerSize(.7);
    h0_arm0->SetMarkerColor(kBlue);
    leg->AddEntry(h0_arm0,"Sector 4-5","p");

    h0_arm1->SetStats(0);
    h0_arm1->SetMarkerStyle(20);
    h0_arm1->GetYaxis()->SetTitleOffset(1.2);
    h0_arm1->GetYaxis()->SetTitle("Mean Leading Edge [ns]");
    h0_arm1->GetYaxis()->SetRangeUser(ymin0,ymax0);
    h0_arm1->GetXaxis()->SetRangeUser(xmin0,xmax0);
    h0_arm1->SetMarkerSize(.7);
    h0_arm1->SetMarkerColor(kGreen-6);
    leg->AddEntry(h0_arm1,"Sector 5-6","p");

    h1_arm0->SetStats(0);
    h1_arm0->SetMarkerStyle(20);
    h1_arm0->GetYaxis()->SetTitleOffset(1.2);
    h1_arm0->GetYaxis()->SetTitle("Mean Leading Edge [ns]");
    h1_arm0->GetYaxis()->SetRangeUser(ymin1,ymax1);
    h1_arm0->GetXaxis()->SetRangeUser(xmin1,xmax1);
    h1_arm0->SetMarkerSize(.7);
    h1_arm0->SetMarkerColor(kBlue);

    h1_arm1->SetStats(0);
    h1_arm1->SetMarkerStyle(20);
    h1_arm1->GetYaxis()->SetTitleOffset(1.2);
    h1_arm1->GetYaxis()->SetTitle("Mean Leading Edge [ns]");
    h1_arm1->GetYaxis()->SetRangeUser(ymin1,ymax1);
    h1_arm1->GetXaxis()->SetRangeUser(xmin1,xmax1);
    h1_arm1->SetMarkerSize(.7);
    h1_arm1->SetMarkerColor(kGreen-6);

    h2_arm0->SetStats(0);
    h2_arm0->SetMarkerStyle(20);
    h2_arm0->GetYaxis()->SetTitleOffset(1.2);
    h2_arm0->GetYaxis()->SetTitle("Mean Leading Edge [ns]");
    h2_arm0->GetYaxis()->SetRangeUser(ymin2,ymax2);
    h2_arm0->GetXaxis()->SetRangeUser(xmin2,xmax2);
    h2_arm0->SetMarkerSize(.7);
    h2_arm0->SetMarkerColor(kBlue);

    h2_arm1->SetStats(0);
    h2_arm1->SetMarkerStyle(20);
    h2_arm1->GetYaxis()->SetTitleOffset(1.2);
    h2_arm1->GetYaxis()->SetTitle("Mean Leading Edge [ns]");
    h2_arm1->GetYaxis()->SetRangeUser(ymin2,ymax2);
    h2_arm1->GetXaxis()->SetRangeUser(xmin2,xmax2);
    h2_arm1->SetMarkerSize(.7);
    h2_arm1->SetMarkerColor(kGreen-6);

    h3_arm0->SetStats(0);
    h3_arm0->SetMarkerStyle(20);
    h3_arm0->GetYaxis()->SetTitleOffset(1.2);
    h3_arm0->GetYaxis()->SetTitle("Mean Leading Edge [ns]");
    h3_arm0->GetYaxis()->SetRangeUser(ymin3,ymax3);
    h3_arm0->GetXaxis()->SetRangeUser(xmin3,xmax3);
    h3_arm0->SetMarkerSize(.7);
    h3_arm0->SetMarkerColor(kBlue);

    h3_arm1->SetStats(0);
    h3_arm1->SetMarkerStyle(20);
    h3_arm1->GetYaxis()->SetTitleOffset(1.2);
    h3_arm1->GetYaxis()->SetTitle("Mean Leading Edge [ns]");
    h3_arm1->GetYaxis()->SetRangeUser(ymin3,ymax3);
    h3_arm1->GetXaxis()->SetRangeUser(xmin3,xmax3);
    h3_arm1->SetMarkerSize(.7);
    h3_arm1->SetMarkerColor(kGreen-6);

    //compute the pad range with suitable margins
    Double_t ymin = -1;
    Double_t ymax = 3;
    Double_t dy = (ymax-ymin)/0.8; //10 per cent margins top and bottom
    Double_t xmin = 0;
    Double_t xmax = h_max_value->GetMaximum();
    Double_t dx = (xmax-xmin)/0.8; //10 per cent margins left and right
    TGaxis *axis = new TGaxis(xmax,ymin,xmax,ymax,ymin,ymax,510,"+L");

    // Compute Pearson Coeff. Factor
    TH2F* correlation_pl0_arm0 = new TH2F("correlation_pl0_arm0",";Mean CMS longitudinal vertex position [cm]; Leading Edge, Sector 4-5 [ns]", h_vertexz->GetNbinsX(), xmin_vertex, xmax_vertex, h0_arm0->GetNbinsX(), xmin0, xmax0);
    TH2F* correlation_pl1_arm0 = new TH2F("correlation_pl1_arm0",";Mean CMS longitudinal vertex position [cm]; Leading Edge, Sector 4-5 [ns]", h_vertexz->GetNbinsX(), xmin_vertex, xmax_vertex, h1_arm0->GetNbinsX(), xmin1, xmax1);
    TH2F* correlation_pl2_arm0 = new TH2F("correlation_pl2_arm0",";Mean CMS longitudinal vertex position [cm]; Leading Edge, Sector 4-5 [ns]", h_vertexz->GetNbinsX(), xmin_vertex, xmax_vertex, h2_arm0->GetNbinsX(), xmin2, xmax2);
    TH2F* correlation_pl3_arm0 = new TH2F("correlation_pl3_arm0",";Mean CMS longitudinal vertex position [cm]; Leading Edge, Sector 4-5 [ns]", h_vertexz->GetNbinsX(), xmin_vertex, xmax_vertex, h3_arm0->GetNbinsX(), xmin3, xmax3);

    TH2F* correlation_pl0_arm1 = new TH2F("correlation_pl0_arm1",";Mean CMS longitudinal vertex position [cm]; Leading Edge, Sector 5-6 [ns]", h_vertexz->GetNbinsX(), xmin_vertex, xmax_vertex, h0_arm1->GetNbinsX(), xmin0, xmax0);
    TH2F* correlation_pl1_arm1 = new TH2F("correlation_pl1_arm1",";Mean CMS longitudinal vertex position [cm]; Leading Edge, Sector 5-6 [ns]", h_vertexz->GetNbinsX(), xmin_vertex, xmax_vertex, h1_arm1->GetNbinsX(), xmin1, xmax1);
    TH2F* correlation_pl2_arm1 = new TH2F("correlation_pl2_arm1",";Mean CMS longitudinal vertex position [cm]; Leading Edge, Sector 5-6 [ns]", h_vertexz->GetNbinsX(), xmin_vertex, xmax_vertex, h2_arm1->GetNbinsX(), xmin2, xmax2);
    TH2F* correlation_pl3_arm1 = new TH2F("correlation_pl3_arm1",";Mean CMS longitudinal vertex position [cm]; Leading Edge, Sector 5-6[ns]", h_vertexz->GetNbinsX(), xmin_vertex, xmax_vertex, h3_arm1->GetNbinsX(), xmin3, xmax3);

    for(int i=1;i<=h_vertexz->GetNbinsX();i++){
      correlation_pl0_arm0->Fill(h_vertexz->GetBinContent(i), h0_arm0->GetBinContent(i));
      correlation_pl1_arm0->Fill(h_vertexz->GetBinContent(i), h1_arm0->GetBinContent(i));
      correlation_pl2_arm0->Fill(h_vertexz->GetBinContent(i), h2_arm0->GetBinContent(i));
      correlation_pl3_arm0->Fill(h_vertexz->GetBinContent(i), h3_arm0->GetBinContent(i));

      correlation_pl0_arm1->Fill(h_vertexz->GetBinContent(i), h0_arm1->GetBinContent(i));
      correlation_pl1_arm1->Fill(h_vertexz->GetBinContent(i), h1_arm1->GetBinContent(i));
      correlation_pl2_arm1->Fill(h_vertexz->GetBinContent(i), h2_arm1->GetBinContent(i));
      correlation_pl3_arm1->Fill(h_vertexz->GetBinContent(i), h3_arm1->GetBinContent(i));

      if(h0_arm0->GetBinError(i) > 0) h_error->Fill(h0_arm0->GetBinError(i));
      if(h1_arm0->GetBinError(i) > 0) h_error->Fill(h1_arm0->GetBinError(i));
      if(h2_arm0->GetBinError(i) > 0) h_error->Fill(h2_arm0->GetBinError(i));
      if(h3_arm0->GetBinError(i) > 0) h_error->Fill(h3_arm0->GetBinError(i));

      if(h0_arm1->GetBinError(i) > 0) h_error->Fill(h0_arm1->GetBinError(i));
      if(h1_arm1->GetBinError(i) > 0) h_error->Fill(h1_arm1->GetBinError(i));
      if(h2_arm1->GetBinError(i) > 0) h_error->Fill(h2_arm1->GetBinError(i));
      if(h3_arm1->GetBinError(i) > 0) h_error->Fill(h3_arm1->GetBinError(i));
    }

    TText *text=new TText(500.,-0.5,"");
    char result_corr[300];

    // Canvas 1
    canvas_per_arm->cd(1);
    pad1_h0->Draw();
    pad1_h0->cd();
    h0_arm0->SetTitle(title0.c_str());
    h0_arm0->Draw("E1 X0 P");
    h0_arm1->Draw("E1 X0 P SAME");
    pad2_h0->Range(xmin-0.1*dx,ymin-0.1*dy,xmax+0.1*dx,ymax+0.1*dy);
    pad2_h0->Draw();
    pad2_h0->cd();
    h_vertexz->Draw("E1 X0 P SAME");
    axis->SetLabelColor(kRed);
    axis->SetTitleOffset(1.3);
    axis->SetTitle("Mean CMS longitudinal vertex position [cm]");
    axis->Draw();
    sprintf(result_corr,"Pearson Coeff. 4-5: %.3f",correlation_pl0_arm0->GetCorrelationFactor());
    text->DrawText(xmax-0.7*xmax,-0.5,result_corr);
    sprintf(result_corr,"Pearson Coeff. 5-6: %.3f",correlation_pl0_arm1->GetCorrelationFactor());
    text->DrawText(xmax-0.7*xmax,-0.8,result_corr);
    leg->Draw();

    // Canvas 2
    canvas_per_arm->cd(2);
    pad1_h1->Draw();
    pad1_h1->cd();
    h1_arm0->SetTitle(title1.c_str());
    h1_arm0->Draw("E1 X0 P");
    h1_arm1->Draw("E1 X0 P SAME");
    pad2_h1->Range(xmin-0.1*dx,ymin-0.1*dy,xmax+0.1*dx,ymax+0.1*dy);
    pad2_h1->Draw();
    pad2_h1->cd();
    h_vertexz->Draw("E1 X0 P SAME");
    axis->SetLabelColor(kRed);
    axis->Draw();
    sprintf(result_corr,"Pearson Coeff. 4-5: %.3f",correlation_pl1_arm0->GetCorrelationFactor());
    text->DrawText(xmax-0.7*xmax,-0.5,result_corr);
    sprintf(result_corr,"Pearson Coeff. 5-6: %.3f",correlation_pl1_arm1->GetCorrelationFactor());
    text->DrawText(xmax-0.7*xmax,-0.8,result_corr);
    leg->Draw();

    // Canvas 3
    canvas_per_arm->cd(3);
    pad1_h2->Draw();
    pad1_h2->cd();
    h2_arm0->SetTitle(title2.c_str());
    h2_arm0->Draw("E1 X0 P");
    h2_arm1->Draw("E1 X0 P SAME");
    pad2_h2->Range(xmin-0.1*dx,ymin-0.1*dy,xmax+0.1*dx,ymax+0.1*dy);
    pad2_h2->Draw();
    pad2_h2->cd();
    h_vertexz->Draw("E1 X0 P SAME");
    axis->SetLabelColor(kRed);
    axis->Draw();
    sprintf(result_corr,"Pearson Coeff. 4-5: %.3f",correlation_pl2_arm0->GetCorrelationFactor());
    text->DrawText(xmax-0.7*xmax,-0.5,result_corr);
    sprintf(result_corr,"Pearson Coeff. 5-6: %.3f",correlation_pl2_arm1->GetCorrelationFactor());
    text->DrawText(xmax-0.7*xmax,-0.8,result_corr);
    leg->Draw();

    // Canvas 4
    canvas_per_arm->cd(4);
    pad1_h3->Draw();
    pad1_h3->cd();
    h3_arm0->SetTitle(title3.c_str());
    h3_arm0->Draw("E1 X0 P");
    h3_arm1->Draw("E1 X0 P SAME");
    pad2_h3->Range(xmin-0.1*dx,ymin-0.1*dy,xmax+0.1*dx,ymax+0.1*dy);
    pad2_h3->Draw();
    pad2_h3->cd();
    h_vertexz->Draw("E1 X0 P SAME");
    axis->SetTextColor(kRed);
    axis->SetLabelColor(kRed);
    axis->Draw();
    sprintf(result_corr,"Pearson Coeff. 4-5: %.3f",correlation_pl3_arm0->GetCorrelationFactor());
    text->DrawText(xmax-0.7*xmax,-0.5,result_corr);
    sprintf(result_corr,"Pearson Coeff. 5-6: %.3f",correlation_pl3_arm1->GetCorrelationFactor());
    text->DrawText(xmax-0.7*xmax,-0.8,result_corr);
    leg->Draw();

    canvas_per_arm->Update();

    std::string picturename = Form("Stability_meanGetLeadingVsLuminosity_PerPlanesBothArmsCh%iVertexCMS.%s", ch_id, pic_format.c_str());
    canvas_per_arm->SaveAs(picturename.c_str());

    canvas_error->cd();
    std::string title_error = Form("Channel %i", ch_id);
    h_error->SetTitle(title_error.c_str());
    h_error->GetYaxis()->SetTitle("N Events");
    h_error->GetXaxis()->SetTitle("Leading Edge standard error per LS [ns]");
    h_error->Draw();
    canvas_error->Update();

    picturename = Form("Stability_Error_AllPlanesArmsCh%iVertexCMS.%s", ch_id, pic_format.c_str());
    canvas_error->SaveAs(picturename.c_str());

    delete correlation_pl0_arm0;
    delete correlation_pl1_arm0;
    delete correlation_pl2_arm0;
    delete correlation_pl3_arm0;

    delete correlation_pl0_arm1;
    delete correlation_pl1_arm1;
    delete correlation_pl2_arm1;
    delete correlation_pl3_arm1;

    delete h_max_value;
    delete h_error;
    delete canvas_per_arm;
    delete canvas_error;
    delete leg;

  }

  gSystem->Exit(kTRUE);

}
