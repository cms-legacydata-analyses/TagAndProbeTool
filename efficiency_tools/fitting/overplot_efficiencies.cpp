#include "src/create_folder.h"

TEfficiency* read_TEfficiency(const char* folder_path, const char* file_name, const char* TEfficiency_path)
{
	string file_path = string(folder_path) + string(file_name);
	TFile *file0 = TFile::Open(file_path.c_str());
	if (file0 == NULL)
	{
		std::cerr << "Could not find \"" << file_path << "\" file.\n";
		std::cerr << "ABORTING...\n";
		abort();
	}

	TEfficiency* pEff0 = (TEfficiency*)file0->Get(TEfficiency_path);
	if (pEff0== NULL)
	{
		std::cerr << "Could not find the histogram path \"" << TEfficiency_path << "\" in \"" << file_path << "\" file.\n";
		std::cerr << "ABORTING...\n";
		abort();
	}

	return pEff0;
}

void overplot_efficiencies()
{
	const char*  input_folder_name = "results/efficiencies/systematic_1D/Jpsi_Run_2011/";
	const char* output_folder_name = "results/overplots/";

	string MuonId   = "trackerMuon";
	//string MuonId   = "standaloneMuon";
	//string MuonId   = "globalMuon";

	string quantity = "Pt";
	//string quantity = "Eta";
	//string quantity = "Phi";

	string file_name = quantity+"_"+MuonId+".root";
	TEfficiency* pEffNominal	= read_TEfficiency(input_folder_name, file_name.c_str(), string(MuonId + "_" + quantity + "_Nominal"  + "_Efficiency").c_str());
	TEfficiency* pEff2Gauss		= read_TEfficiency(input_folder_name, file_name.c_str(), string(MuonId + "_" + quantity + "_2xGauss"  + "_Efficiency").c_str());
	TEfficiency* pEffMassUP		= read_TEfficiency(input_folder_name, file_name.c_str(), string(MuonId + "_" + quantity + "_MassUp"   + "_Efficiency").c_str());
	TEfficiency* pEffMassDown	= read_TEfficiency(input_folder_name, file_name.c_str(), string(MuonId + "_" + quantity + "_MassDown" + "_Efficiency").c_str());
	TEfficiency* pEffBinUp		= read_TEfficiency(input_folder_name, file_name.c_str(), string(MuonId + "_" + quantity + "_BinUp"    + "_Efficiency").c_str());
	TEfficiency* pEffBinDown	= read_TEfficiency(input_folder_name, file_name.c_str(), string(MuonId + "_" + quantity + "_BinDown"  + "_Efficiency").c_str());

	TCanvas* c1 = new TCanvas("systematic_efficiency", "Systematic Efficiency");

	pEff2Gauss		->SetLineColor(kGreen+1);
	pEffMassUP		->SetLineColor(kRed);
	pEffMassDown	->SetLineColor(kOrange);
	pEffBinUp		->SetLineColor(kMagenta);
	pEffBinDown		->SetLineColor(kBlue);

	pEff2Gauss		->SetMarkerColor(kGreen+1);
	pEffMassUP		->SetMarkerColor(kRed);
	pEffMassDown	->SetMarkerColor(kOrange);
	pEffBinUp		->SetMarkerColor(kMagenta);
	pEffBinDown		->SetMarkerColor(kBlue);

	//Set range in y axis
	pEffNominal->Draw();
	pEffNominal->SetTitle((string(pEffNominal->GetTitle()).substr(0, string(pEffNominal->GetTitle()).size()-10)).c_str());
	gPad->Update();
	auto graph = pEffNominal->GetPaintedGraph();
	graph->SetMinimum(0.66);
	graph->SetMaximum(1.);

	if (quantity == "Pt")
	{
		graph->SetMinimum(0.66);
		graph->SetMaximum(1.);
	}
	else if (quantity == "Eta")
	{
		graph->SetMinimum(0.9);
		graph->SetMaximum(1.);
	}

	gPad->Update();

	pEff2Gauss		->Draw("same");
	pEffMassUP		->Draw("same");
	pEffMassDown	->Draw("same");
	pEffBinUp		->Draw("same");
	pEffBinDown		->Draw("same");
	pEffNominal     ->Draw("same");

	//TLegend* tl = new TLegend(0.70,0.86,0.96,0.92);
	//TLegend* tl = new TLegend(0.2,0.86,0.4,0.92);
	TLegend* tl = new TLegend(0.15,0.45,0.35, 0.2);
	if (MuonId == "trackerMuon" && quantity != "Eta")
		tl->SetY2(0.8);
	tl->SetTextSize(0.04);
	tl->AddEntry(pEffNominal, 	"Nominal", 		"elp");
	tl->AddEntry(pEff2Gauss, 	"2x Gauss", 	"elp");
	tl->AddEntry(pEffMassUP, 	"Mass Up", 		"elp");
	tl->AddEntry(pEffMassDown, 	"Mass Down", 	"elp");
	tl->AddEntry(pEffBinUp, 	"Bin Up", 		"elp");
	tl->AddEntry(pEffBinDown, 	"Bin Down", 	"elp");
	tl->SetY1(tl->GetY2() - tl->GetTextSize()*1.2*tl->GetNRows());
	tl->Draw();

	if (quantity == "Pt")
		c1->SetLogx();

	//if (quantity == "Eta")
		//c1->SetLogy();

	create_folder(output_folder_name);
	c1->SaveAs(string(output_folder_name + quantity + "_" + MuonId + "_systematic_efficiency.png").c_str());
}