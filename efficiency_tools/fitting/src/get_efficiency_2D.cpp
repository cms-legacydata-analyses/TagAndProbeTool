
TEfficiency* get_efficiency_2D(TH2D* all, TH2D* pass, string xquantity, string yquantity, string MuonId, string prefix_name = "", bool shouldWrite = false)
{
	TEfficiency* pEff = new TEfficiency();
	pEff->SetPassedHistogram(*pass, "f");
	pEff->SetTotalHistogram (*all,  "f");

	//Set plot config
	if (prefix_name != "")
	{
		pEff->SetName(string(MuonId + "_" + xquantity + "_" + yquantity + "_" + prefix_name + "_Efficiency").c_str());
		pEff->SetTitle(string("Efficiency for " + MuonId + " (" + prefix_name + ")").c_str());
	}
	else
	{
		pEff->SetName(string(MuonId + "_" + xquantity + "_" + yquantity + "_Efficiency").c_str());
		pEff->SetTitle(string("Efficiency for " + MuonId).c_str());
	}

	pEff->SetLineColor(kBlack);
	pEff->SetMarkerStyle(21);
	pEff->SetMarkerSize(0.5);
	pEff->SetMarkerColor(kBlack);
	
	if (shouldWrite)
		pEff->Write();
	
	TCanvas* oi = new TCanvas();
	oi->cd();
	pEff->Draw("colz");
	
	/*
	gPad->Update();
	auto graph = pEff->GetPaintedGraph();
	graph->SetMinimum(0.8);
	graph->SetMaximum(1.2);
	gPad->Update();
	*/
	
	return pEff;
}
