#include "../src/get_efficiency_TH2D.h"
#include "../src/create_folder.h"

void data_mc_ratio()
{
	//string MuonId   = "trackerMuon";
	//string MuonId   = "standaloneMuon";
	string MuonId   = "globalMuon";

	//string sufix = "Nominal";
	//string sufix = "Systematic";
	string sufix = "Final";

	bool should_save = true;


	TFile* file0 = TFile::Open(string("../results/efficiencies/systematic_2D/Jpsi_Run_2011/Eta_Pt_" +MuonId+".root").c_str());
	TFile* file1 = TFile::Open(string("../results/efficiencies/systematic_2D/Jpsi_MC_2020/Eta_Pt_"+MuonId+".root").c_str());
	TH2D*  hall  = (TH2D*)file0->Get(string(MuonId + "_Eta_Pt_" + sufix + "_Efficiency").c_str());
	TH2D*  hpass = (TH2D*)file1->Get(string(MuonId + "_Eta_Pt_" + sufix + "_Efficiency").c_str());
	hall->Draw();
	TH2D*  heff = get_efficiency_TH2D(hall, hpass, "Pt", "Eta", MuonId);
	heff->SetTitle(string("Data/MC for " + MuonId + " (" + sufix + ")").c_str());

	TCanvas* c1 = new TCanvas("c1", "c1", 1280, 900);
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
		string output_folder_name = "../results/tests/data_mc_ratio/";
		create_folder(output_folder_name.c_str());
		c1->SaveAs(string(output_folder_name + MuonId + "_" + sufix + "_TH2D.png").c_str());
	}
}