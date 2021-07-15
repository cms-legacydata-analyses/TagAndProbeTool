

TEfficiency* get_efficiency(TH1D* ALL, TH1D* PASS, string quantity, string MuonId, string prefix_name = "",  bool shouldWrite = false)
{
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
	if (prefix_name != "")
	{
		pEff->SetName(string(MuonId + "_" + quantity + "_" + prefix_name + "_Efficiency").c_str());
		pEff->SetTitle(string("Efficiency for " + MuonId + " " + quantity + " (" + prefix_name + ")").c_str());
	}
	else
	{
		pEff->SetName(string(MuonId + "_" + quantity + "_Efficiency").c_str());
		pEff->SetTitle(string("Efficiency for " + MuonId + " " + quantity).c_str());
	}

	pEff->SetLineColor(kBlack);
	pEff->SetMarkerStyle(21);
	pEff->SetMarkerSize(0.5);
	pEff->SetMarkerColor(kBlack);
	
	if (shouldWrite)
		pEff->Write();
	
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
