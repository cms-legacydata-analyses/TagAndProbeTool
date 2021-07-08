#include "src/create_folder.cpp"

TEfficiency* read_TEfficiency(const char* folder_path, const char* file_name, const char* TEfficiency_path = "trackerMuon_Efficiency")
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
	const char* folder_name = "efficiency_result/Jpsi_Run_2011/";

	/*
	const char* nominal = "nominal_trackerMuon.root";
	const char* fit[]   = {"2xgaus_trackerMuon.root"};
	const char* mass[]  = {"mass_2p75_3p35trackerMuon.root", "mass_2p85_3p25trackerMuon.root"};
	const char* bins[]  = {"binfit95_trackerMuon.root",      "binfit105_trackerMuon.root"};
	const char** errors[] = {fit, mass, bins};
	*/

	TCanvas* c1 = new TCanvas("systematic_efficiency", "Systematic Efficiency");

	TEfficiency* pEffNoninal	= read_TEfficiency(folder_name, "nominal_trackerMuon.root");
	TEfficiency* pEff2Gauss		= read_TEfficiency(folder_name, "2xgaus_trackerMuon.root");
	TEfficiency* pEffMassUP		= read_TEfficiency(folder_name, "mass_2p85_3p25trackerMuon.root");
	TEfficiency* pEffMassDown	= read_TEfficiency(folder_name, "mass_2p75_3p35trackerMuon.root");
	TEfficiency* pEffBinUp		= read_TEfficiency(folder_name, "binfit105_trackerMuon.root");
	TEfficiency* pEffBinDown	= read_TEfficiency(folder_name, "binfit95_trackerMuon.root");

	//const TEfficiency* sys_efficiency = {pEff2Gauss, pEffMassUP, pEffMassDown, pEffBinUp, pEffBinDown};

	/*
	TList* list = new TList();
	list->Add(pEff2Gauss);
	list->Add(pEffMassUP);
	list->Add(pEffMassDown);
	list->Add(pEffBinUp);
	list->Add(pEffBinDown);

	// Draw others plotsZ
	for (int i = 0; i < 5; i++)
	{
		auto temp = list->At(i);

		//temp->SetFillStyle(3004);
		//temp->SetFillColor(kRed);
		temp->Draw("a4");
	}
	*/

	/*
	//pEff2Gauss->SetFillStyle(3004);
	//pEffMassUP->SetFillStyle(3004);
	//pEffMassDown->SetFillStyle(3004);
	//pEffBinUp->SetFillStyle(3004);
	//pEffBinDown->SetFillStyle(3004);

	pEff2Gauss		->SetFillColor(kRed);
	pEffMassUP		->SetFillColor(kRed);
	pEffMassDown	->SetFillColor(kRed);
	pEffBinUp		->SetFillColor(kRed);
	pEffBinDown		->SetFillColor(kRed);
	*/

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
	graph->SetMinimum(0.95);
	graph->SetMaximum(1.02);
	gPad->Update();

	pEff2Gauss		->Draw("same");
	pEffMassUP		->Draw("same");
	pEffMassDown	->Draw("same");
	pEffBinUp		->Draw("same");
	pEffBinDown		->Draw("same");

	// Draw Nominal Plot
	//pEffNoninal->SetFillStyle(3004);
	//pEffNoninal->SetFillColor(kRed);
	pEffNoninal->Draw("same");

	//TLegend* tl = new TLegend(0.70,0.86,0.96,0.92);
	TLegend* tl = new TLegend(0.70,0.86,0.96,0.5);
	tl->SetTextSize(0.04);
	tl->AddEntry(pEffNoninal, 	"Nominal", 		"elp");
	tl->AddEntry(pEff2Gauss, 	"2x Gauss", 	"elp");
	tl->AddEntry(pEffMassUP, 	"Mass Up", 		"elp");
	tl->AddEntry(pEffMassDown, 	"Mass Down", 	"elp");
	tl->AddEntry(pEffBinUp, 	"Bin Up", 		"elp");
	tl->AddEntry(pEffBinDown, 	"Bin Down", 	"elp");
	//tl->SetY1(tl->GetY1() - tl->GetTextSize()*tl->GetNRows());
	//tl->SetY1(tl->GetY2() - (tl->GetTextSize()+0.02)*tl->GetNRows());
	tl->Draw();

	c1->SetLogx();

	/*
	// Create a histogram based on Tefficiency
	TH1* hEff = pEff0->GetCopyTotalHisto();
	//TH1* hEff = pEff0->GetCopyPassedHisto();
	//hEff->Divide(pEff0->GetCopyTotalHisto());

	for (int i = 0; i < hEff->GetNbinsX(); i++)
	{
		double content = pEff0->GetEfficiency(i);
		double error   = max(pEff0->GetEfficiencyErrorUp(i), pEff0->GetEfficiencyErrorLow(i));

		hEff->SetBinContent(i, content);
		hEff->SetBinError(i, error);
		//hEff->SetBinError(i, 0);
	}

	//Set style
	hEff->SetLineWidth(1);
	hEff->SetLineColor(kBlack);
	hEff->SetMarkerStyle(21);
	hEff->SetMarkerSize(0.5);
	hEff->SetMarkerColor(kBlack);

	//Set range in y axis
	hEff->SetMinimum(0.0);
	hEff->SetMaximum(1.05);
	hEff->SetMinimum(0.96);
	hEff->SetMaximum(1.01);

	//Set range in x axis
	hEff->GetXaxis()->SetRangeUser(0., 17.);
	
	gStyle->SetOptStat(0);
	hEff->Draw();
	*/

	/*
	TCanvas* c2 = new TCanvas();
	pEff0->Draw();
	//Set range in y axis
	gPad->Update();
	auto graph = pEff0->GetPaintedGraph();
	graph->SetMinimum(0.0);
	graph->SetMaximum(1.05);
	graph->SetMinimum(0.96);
	graph->SetMaximum(1.01);
	gPad->Update();
	pEff0->Draw();
	*/

	//TGraphAsymmErrors* tGraph = pEff0->CreateGraph();
	//tGraph->Draw("ap");

	


	//c1->SaveAs("systematic_efficiency.png");
}