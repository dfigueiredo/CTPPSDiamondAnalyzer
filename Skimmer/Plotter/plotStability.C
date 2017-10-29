#include <stdio.h>
#include <string.h>

void plotStability(){

  gStyle->SetOptStat(0);

  TFile *f = new TFile("outputfile_Run304447.root");
  TIter next(f->GetListOfKeys());
  TKey *key;

  double xmin = 0;
  double xmax = 1000;

  for (int pl_id = 0; pl_id < 4; pl_id++){

    for (int ch_id = 0; ch_id < 12; ch_id++){
      double ymin = 100;
      double ymax = 0;
      TCanvas *canvas = new TCanvas("Stability","Stability", 800, 600);

      for (int arm_id = 0; arm_id < 2; arm_id++){
	std::string histoname_pl = Form("mean_getLeadingVslumisection_arm%i_pl%i_ch%i", arm_id, pl_id, ch_id);
	TH1* h = 0;
	f->GetObject(histoname_pl.c_str(), h);
	if(h->GetMaximum(100)>ymax) ymax = h->GetMaximum(100);
	if(h->GetMinimum(0.1)<ymin) ymin = h->GetMinimum(0.1);
      }

      TLegend* leg = new TLegend(0.69,0.65,0.88,0.865);


      for (int arm_id = 0; arm_id < 2; arm_id++){

	std::string histoname_pl = Form("mean_getLeadingVslumisection_arm%i_pl%i_ch%i", arm_id, pl_id, ch_id);
	std::string title0 = Form("Plane %i, Channel %i", pl_id, ch_id);
	TH1* h0 = 0;

	f->GetObject(histoname_pl.c_str(), h0);

	h0->SetMarkerStyle(20);
	h0->GetYaxis()->SetTitle("Mean [ns]");
	h0->SetMarkerSize(.7);
	h0->Rebin(10);
	h0->SetTitle(title0.c_str());
	h0->GetXaxis()->SetRangeUser(xmin,xmax);
	h0->GetYaxis()->SetRangeUser(ymin-0.3,ymax+0.3);

	if (arm_id==0){
	  h0->SetMarkerColor(kBlue);
	  leg->AddEntry(h0,"Arm 0","p");
	  h0->Draw("E1 X0 P");
	} else {
	  h0->SetMarkerColor(kRed);
	  leg->AddEntry(h0,"Arm 1","p");
	  h0->Draw("SAME E1 X0 P");  
	}

      }

      leg->Draw();
      std::string pngname = Form("mean_getLeadingVslumisection_pl%i_ch%i.png",  pl_id, ch_id);  
      canvas->SaveAs(pngname.c_str());
      delete leg;
      delete canvas;

    }
  }

}

