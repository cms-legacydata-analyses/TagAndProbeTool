// Recreates DATA .root file as exactly fitting method uses
void simplify_data()
{

	TFile *file0  = TFile::Open("DATA/Run2011AMuOnia_mergeNtuple.root");
	TTree *TreeAT = (TTree*)file0->Get(("tagandprobe/AnalysisTree"));
	TTree *TreePC = (TTree*)file0->Get(("tagandprobe/PlotControl"));

	TFile *fileIO = TFile::Open("DATA/TagAndProbe_Jpsi_Run2011.root","RECREATE");
	TTree *treeIO	 = new TTree("tagandprobe", "Tag And Probe");
	
	//Create variables
	double ProbeMuon_Pt;
	double ProbeMuon_Eta;
	double ProbeMuon_Phi;
	double TagMuon_Pt;
	double TagMuon_Eta;
	double TagMuon_Phi;
	double InvariantMass;
	int PassingProbeTrackingMuon;
	int PassingProbeStandAloneMuon;
	int PassingProbeGlobalMuon;

	//Assign variables
	TreePC->SetBranchAddress("ProbeMuon_Pt",						&ProbeMuon_Pt);
	TreePC->SetBranchAddress("ProbeMuon_Eta",						&ProbeMuon_Eta);
	TreePC->SetBranchAddress("ProbeMuon_Phi",						&ProbeMuon_Phi);
	TreePC->SetBranchAddress("TagMuon_Pt",							&TagMuon_Pt);
	TreePC->SetBranchAddress("TagMuon_Eta",						&TagMuon_Eta);
	TreePC->SetBranchAddress("TagMuon_Phi",						&TagMuon_Phi);
	TreeAT->SetBranchAddress("InvariantMass",						&InvariantMass);
	TreeAT->SetBranchAddress("PassingProbeTrackingMuon",		&PassingProbeTrackingMuon);
	TreeAT->SetBranchAddress("PassingProbeStandAloneMuon",	&PassingProbeStandAloneMuon);
	TreeAT->SetBranchAddress("PassingProbeGlobalMuon",			&PassingProbeGlobalMuon);

	//Assign new variables
	treeIO->Branch("InvariantMass",					&InvariantMass);
	treeIO->Branch("ProbeMuon_Pt",					&ProbeMuon_Pt);
	treeIO->Branch("ProbeMuon_Eta",					&ProbeMuon_Eta);
	treeIO->Branch("ProbeMuon_Phi",					&ProbeMuon_Phi);
	treeIO->Branch("TagMuon_Pt",						&TagMuon_Pt);
	treeIO->Branch("TagMuon_Eta",						&TagMuon_Eta);
	treeIO->Branch("TagMuon_Phi",						&TagMuon_Phi);
	treeIO->Branch("PassingProbeTrackingMuon",	&PassingProbeTrackingMuon);
	treeIO->Branch("PassingProbeStandAloneMuon",	&PassingProbeStandAloneMuon);
	treeIO->Branch("PassingProbeGlobalMuon",		&PassingProbeGlobalMuon);

	long long numberEntries = TreePC->GetEntries();
	//numberEntries = 1000;

	//Prepare for showing progress
	string progressFormat = "Progress: %05.2f%% %0"+to_string(strlen(to_string(numberEntries).data()))+"lld/%lld\r";
	auto lastTime = std::chrono::steady_clock::now();
	auto start    = std::chrono::steady_clock::now();

	//Loop between the components
	for (long long i = 0; i < numberEntries; i++)
	{
		//Select particle pair
		TreePC->GetEntry(i);
		TreeAT->GetEntry(i);

		//Show progress on screen
		if (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - lastTime).count() >= 1000 || i == numberEntries - 1)
		{
			printf(progressFormat.data(), (float)(i+1)/(float)numberEntries*100, i+1, numberEntries);
			lastTime = chrono::steady_clock::now();
		}

		treeIO->Fill();
	}

	cout << endl;
	cout << "Numbers should match:\n";
	cout << TreeAT->GetEntries() << endl;
	cout << TreePC->GetEntries() << endl;
	cout << treeIO->GetEntries() << endl;
	treeIO->Write("", TObject::kOverwrite);
}