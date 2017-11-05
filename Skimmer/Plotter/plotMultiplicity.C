#include <stdio.h>
#include <string.h>

void plotMultiplicity(){

	gStyle->SetOptStat(0);
	gStyle->SetFrameBorderSize(0);

	char* hist[] = {"NVertex_NArm0", "NVertex_NArm1", "NArm0_NArm1"};
	char* files[] = {"output_run303819","output_run304508","output_run304625","output_run304663","output_run304671","output_run305207","output_run305208","output_run305237"};

	size_t Nhist = sizeof(hist)/sizeof(hist[0]);
	size_t Nfiles = sizeof(files)/sizeof(files[0]);
	for (int j = 0; j < Nfiles; j++) {
		char* nameFile = Form("%s.root", files[j]);
		TFile *f = new TFile(nameFile);
		TIter next(f->GetListOfKeys());
		TKey *key;
		for (int i = 0; i < Nhist; i++) {


			TCanvas *c = new TCanvas("Stability","Stability", 1000, 800);
			TPad *pad = new TPad("pad", "pad",0.05,1000,0.55,800);
			pad->Draw(); 
			pad->cd();

			TH2* h0 = 0;

			f->GetObject(hist[i], h0);

			h0->GetYaxis()->SetTitleOffset(1.8);
			h0->GetXaxis()->SetTitleOffset(1.8);

			h0->Draw("LEGO 0");
			pad->SetPhi(210);
			pad->SetLogz();

			c->Update();

			char* pngname = Form("%s_%s.png",files[j] ,hist[i]);
			c->SaveAs(pngname);

			delete c;

		}
		delete f;
	}
}
