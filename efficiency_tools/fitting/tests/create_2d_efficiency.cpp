
void create_2d_efficiency()
{
	const char* filepath = "../results/efficiencies/Jpsi_Run_2011/Pt_Eta_trackerMuon.root"

	const char* histpassed = "histograms/Pass Final"
	const char* histpassed = "histograms/Pass Final"

	pEff->SetStatisticOption(TEfficiency::kBBayesian);
	pEff->SetConfidenceLevel(0.68);

	TEfficiency* pEff = new TEfficiency();
	pEff->SetPassedHistogram(*PASS, "f");
	pEff->SetTotalHistogram (*ALL,"f");
}