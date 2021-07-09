

TEfficiency* get_efficiency(TH1F* ALL, TH1F* PASS, string quantity, string MuonId, string prefix_file_name = "")
{
	//Path where is going to save efficiency 
	string directoryToSave = string("results/efficiencies/") + output_folder_name + string("/");
	create_folder(directoryToSave.c_str());

	//Path to output file
	string file_path = directoryToSave + prefix_file_name + quantity + "_" + MuonId + ".root";

	//Set axis title for efficiency plot
	ALL->GetYaxis()->SetTitle("Efficiency");
	if (quantity == "Pt")
	{
		ALL->GetXaxis()->SetTitle("p_{t} [GeV/c]");
	}
	else if (quantity == "Eta")
	{
		ALL->GetXaxis()->SetTitle("#eta");
	}
	else if (quantity == "Phi")
	{
		ALL->GetXaxis()->SetTitle("rad");
	}
	
	TEfficiency* pEff = new TEfficiency();
	pEff->SetPassedHistogram(*PASS, "f");
	pEff->SetTotalHistogram (*ALL,"f");

	//Set plot config
	pEff->SetName((MuonId + string("_") + quantity + string("_Efficiency") ).c_str());
	pEff->SetTitle((string("Efficiency for ") + MuonId + string(" ") + quantity).c_str());
	pEff->SetLineColor(kBlack);
	pEff->SetMarkerStyle(21);
	pEff->SetMarkerSize(0.5);
	pEff->SetMarkerColor(kBlack);
	
	TFile* pFile = new TFile(file_path.c_str(),"recreate");
	pEff->SetDirectory(gDirectory);
	pFile->Write();
	
	TCanvas* oi = new TCanvas();
	oi->cd();
	pEff->Draw();
	
	gPad->Update();
	auto graph = pEff->GetPaintedGraph();
	graph->SetMinimum(0.8);
	graph->SetMaximum(1.2);
	gPad->Update();

	cout << "\n------------------------\n";
	cout << "Output: " << file_path << "\n";
	cout << "------------------------\n";
	
	return pEff;
}
