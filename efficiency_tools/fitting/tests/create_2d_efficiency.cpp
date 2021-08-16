#include "../src/get_efficiency_TH2D.h"
#include "../src/create_folder.h"

void create_2d_efficiency()
{
	//string foldername = "Jpsi_Run_2011";
	string foldername = "Jpsi_MC_2020";

	//string MuonId   = "trackerMuon";
	//string MuonId   = "standaloneMuon";
	string MuonId   = "globalMuon";

	//string sufix = "_nominal";
	//string sufix = "_systematic";
	string sufix = "_final";

	bool should_save = true;


	TFile *file0 = TFile::Open(string("../results/efficiencies/systematic_2D/"+foldername+"/Eta_Pt_"+MuonId+".root").c_str());
	TH2D *hall  = (TH2D*)file0->Get(string("histograms/all"  + sufix).c_str());
	TH2D *hpass = (TH2D*)file0->Get(string("histograms/pass" + sufix).c_str());
	TH2D* heff = get_efficiency_TH2D(hall, hpass, "Pt", "Eta", MuonId);

	TCanvas* c1 = new TCanvas();
	c1->SetCanvasSize(1600, 900);
	c1->cd();
	heff->Draw("colztexte");

	//heff->SetMaximum(1.);
	//heff->SetMinimum(0.88);
	heff->SetMaximum(1.06);
	heff->SetMinimum(0.4);
	gStyle->SetOptStat(0);
	c1->SetLogx();

	if (should_save)
	{
		string output_folder_name = "../results/tests/"+foldername+"/";
		create_folder(output_folder_name.c_str());
		c1->SaveAs(string(output_folder_name + MuonId + sufix + "_TH2D.png").c_str());
	}
}