

void merge_2d()
{
	const char* name0 = "results/efficiencies/Jpsi_Run_2011/Pt_trackerMuon.root";
	const char* name1 = "results/efficiencies/Jpsi_Run_2011/Eta_trackerMuon.root";
	const char* path0  = "trackerMuon_Pt_Nominal_Efficiency";
	const char* path1  = "trackerMuon_Eta_Efficiency";

	TFile *file0 = TFile::Open(name0);
	TFile *file1 = TFile::Open(name1);

	if (file0 == NULL || file1 == NULL)
	{
		std::cerr << "ABORTING...\n";
		abort();
	}

	TEfficiency* pEff0 = (TEfficiency*)file0->Get(path0);
	TEfficiency* pEff1 = (TEfficiency*)file1->Get(path1);

	if (pEff0 == NULL)
	{
		cerr << "Could not read the path in file0\n";
		abort();
	}

	if (pEff1 == NULL)
	{
		cerr << "Could not read the path in file1\n";
		abort();
	}

	int bins = pEff0->GetPassedHistogram()->GetNbinsX();
	for (int i = 0; i < bins; i++)
	{
		pEff0->GetEfficiency(i);
		pEff0->GetEfficiencyErrorLow(i);
		pEff0->GetEfficiencyErrorUp(i);
		cout << pEff0->GetEfficiency(i) << endl;
	}
}