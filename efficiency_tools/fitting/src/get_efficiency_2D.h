
TEfficiency* get_efficiency_2D(TH2D* all, TH2D* pass, string xquantity, string yquantity, string MuonId, string prefix_name = "", bool shouldWrite = false)
{
	pass->SetDefaultSumw2();
	all ->SetDefaultSumw2();

	TEfficiency* pEff = new TEfficiency();
	pEff->SetPassedHistogram(*pass, "f");
	pEff->SetTotalHistogram (*all,  "f");

	//Set plot config
	if (prefix_name != "")
	{
		pEff->SetName(string(MuonId + "_" + yquantity + "_" + xquantity + "_" + prefix_name + "_Efficiency").c_str());
		pEff->SetTitle(string("Efficiency for " + MuonId + " (" + prefix_name + ")").c_str());
	}
	else
	{
		pEff->SetName(string(MuonId + "_" + yquantity + "_" + xquantity + "_Efficiency").c_str());
		pEff->SetTitle(string("Efficiency for " + MuonId).c_str());
	}

	/*
	pEff->SetLineColor(kBlack);
	pEff->SetMarkerStyle(21);
	pEff->SetMarkerSize(0.5);
	pEff->SetMarkerColor(kBlack);
	*/
	pEff->SetStatisticOption(TEfficiency::kBBayesian);
	pEff->SetConfidenceLevel(0.68);
	
	TCanvas* c1 = new TCanvas();
	c1->cd();
	pEff->Draw("colztexte");

	c1->SetLogx();
	
	if (shouldWrite)
		pEff->Write();
	
	/*
	gPad->Update();
	auto graph = pEff->GetPaintedGraph();
	graph->SetMinimum(0.8);
	graph->SetMaximum(1.2);
	gPad->Update();
	*/
	
	return pEff;
}
