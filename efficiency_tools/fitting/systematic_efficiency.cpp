#include "src/create_folder.cpp"

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

void systematic_efficiency()
{
	const char* folder_name = "results/efficiencies/Jpsi_Run_2011/";

	//string MuonId   = "trackerMuon";
	string MuonId   = "standaloneMuon";
	//string MuonId   = "globalMuon";

	string quantity = "Pt";
	//string quantity = "Eta";
	//string quantity = "Phi";

	string TEfficiency_path = MuonId+"_"+quantity+"_Efficiency";
	string append_file_name = quantity+"_"+MuonId+".root";
	TEfficiency* pEffNominal	= read_TEfficiency(folder_name, (string("nominal_"       )+append_file_name).c_str(), TEfficiency_path.c_str());
	TEfficiency* pEff2Gauss		= read_TEfficiency(folder_name, (string("2xgaus_"        )+append_file_name).c_str(), TEfficiency_path.c_str());
	TEfficiency* pEffMassUP		= read_TEfficiency(folder_name, (string("mass_2p75_3p35_")+append_file_name).c_str(), TEfficiency_path.c_str());
	TEfficiency* pEffMassDown	= read_TEfficiency(folder_name, (string("mass_2p85_3p25_")+append_file_name).c_str(), TEfficiency_path.c_str());
	TEfficiency* pEffBinUp		= read_TEfficiency(folder_name, (string("binfit105_"     )+append_file_name).c_str(), TEfficiency_path.c_str());
	TEfficiency* pEffBinDown	= read_TEfficiency(folder_name, (string("binfit95_"      )+append_file_name).c_str(), TEfficiency_path.c_str());
	
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
	pEff2Gauss->Draw();
	gPad->Update();
	auto graph = pEff2Gauss->GetPaintedGraph();
	graph->SetMinimum(0.6);
	graph->SetMaximum(1.2);
	gPad->Update();

	pEff2Gauss		->Draw("same");
	pEffMassUP		->Draw("same");
	pEffMassDown	->Draw("same");
	pEffBinUp		->Draw("same");
	pEffBinDown		->Draw("same");

	pEffNominal     ->Draw("same");

	TLegend* tl = new TLegend(0.70,0.86,0.96,0.92);
	tl->SetTextSize(0.04);
	tl->AddEntry(pEffNominal, 	"Nominal", 		"elp");
	tl->AddEntry(pEff2Gauss, 	"2x Gauss", 	"elp");
	tl->AddEntry(pEffMassUP, 	"Mass Up", 		"elp");
	tl->AddEntry(pEffMassDown, 	"Mass Down", 	"elp");
	tl->AddEntry(pEffBinUp, 	"Bin Up", 		"elp");
	tl->AddEntry(pEffBinDown, 	"Bin Down", 	"elp");
	tl->SetY1(tl->GetY1() - tl->GetTextSize()*tl->GetNRows());
	tl->Draw();

	c1->SetLogx();

	c1->SaveAs(string("results/" + quantity + "_" + MuonId + "systematic_efficiency.png").c_str());
}