

TEfficiency* get_efficiency(TH1F* ALL, TH1F* PASS, string quantity, string MuonId, bool DataIsMc)
{
	//Path where is going to save efficiency 
	const char* directoryToSave = "efficiency_result/";

	//Check if dir exists and create
	if (gSystem->AccessPathName(directoryToSave))
		if (gSystem->mkdir(directoryToSave, true))
		{
			cerr << "\"" << directoryToSave << "\" path could not be found and could not be created ERROR\n";
			cerr << "Try to create manually this folder path\n";
			abort();
		}
		else
			cout << "\"" << directoryToSave << "\" directory created OK\n";
	else
		cout << "\"" << directoryToSave << "\" directory OK\n";

	//Create name of output file
	string file_path = string(directoryToSave);
	if (DataIsMc == true)
		file_path += string("Run2011_");
	else
		file_path += string("MC_");
	file_path += MuonId.substr(7, -1) + ".root";
	
	TFile* pFile = new TFile(file_path.c_str(),"recreate");
	TEfficiency* pEff = new TEfficiency();
	pEff->SetName("Efficiency");
	pEff->SetPassedHistogram(*PASS, "f");
	pEff->SetTotalHistogram (*ALL,"f");
	
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
	
	return pEff;
}
