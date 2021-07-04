

TEfficiency* get_efficiency(TH1F* ALL, TH1F* PASS, string quantity, string MuonId)
{
	//Path where is going to save efficiency 
	string directoryToSave = string("efficiency_result/") + output_folder_name + string("/");
	create_folder(directoryToSave.c_str());

	//Path to output file
	string file_path = directoryToSave + MuonId + ".root";
	
	TEfficiency* pEff = new TEfficiency();
	pEff->SetName("Efficiency");
	pEff->SetPassedHistogram(*PASS, "f");
	pEff->SetTotalHistogram (*ALL,"f");
	
	TFile* pFile = new TFile(file_path.c_str(),"recreate");
	pEff->SetDirectory(gDirectory);
	PASS->SetDirectory(gDirectory);
	ALL->SetDirectory (gDirectory);
	pFile->Write();
	
	TCanvas* oi = new TCanvas();
	oi->cd();
	pEff->Draw();
	
	gPad->Update();

	auto graph = pEff->GetPaintedGraph();
	graph->SetMinimum(0.8);
	graph->SetMaximum(1.2);
	gPad->Update();
	
	return pEff;
}
