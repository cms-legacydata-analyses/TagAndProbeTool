using namespace RooFit;

//We start by declaring the nature of our dataset. (Is the data real or simulated?)
const char* output_folder_name = "Jpsi_Run_2011";

//Header of this function
const char* fit_functions = "Gaussian + CrystalBall + Chebychev";
double _mmin = 2.8;  double _mmax = 3.3;
double fit_bins = 0;
string prefix_file_name = "";

double* doFit(string condition, string MuonId, string quant1, string quant2, const char* savePath = NULL) // RETURNS ARRAY WITH [yield_all, yield_pass, err_all, err_pass]    ->   OUTPUT ARRAY
{
	string MuonId_str = "";
	if (MuonId == "trackerMuon")
		MuonId_str = "PassingProbeTrackingMuon";
	if (MuonId == "standaloneMuon")
		MuonId_str = "PassingProbeStandAloneMuon";
	if (MuonId == "globalMuon")
		MuonId_str = "PassingProbeGlobalMuon";
	
	TFile *file0       = TFile::Open("DATA/TagAndProbe_Jpsi_Run2011.root");
	TTree *DataTree    = (TTree*)file0->Get(("tagandprobe"));
	
	RooRealVar MuonId_var(MuonId_str.c_str(), MuonId_str.c_str(), 0, 1); //Muon_Id
	
	RooRealVar InvariantMass("InvariantMass", "InvariantMass", _mmin, _mmax);
	if (fit_bins != 0)
		InvariantMass.setBins(fit_bins);
	fit_bins = InvariantMass.getBinning().numBins();
	
	RooRealVar quantityPt("ProbeMuon_Pt", "ProbeMuon_Pt", 0., 40.);
	RooRealVar quantityEta("ProbeMuon_Eta", "ProbeMuon_Eta", -2.1, 2.1);
	RooRealVar quantityPhi("ProbeMuon_Phi", "ProbeMuon_Phi", -3., 3.);

	//RooRealVar quantity(("ProbeMuon_" + quant).c_str(), ("ProbeMuon_" + quant).c_str(), limits[0], limits[1]);
	
	cout << "Conditions: " << condition << endl;
	RooFormulaVar* redeuce = new RooFormulaVar("PPTM", condition.c_str(), RooArgList(quantityPt, quantityEta));
	RooDataSet *Data_ALL    = new RooDataSet("DATA", "DATA", DataTree, RooArgSet(InvariantMass, MuonId_var, quantityPt, quantityEta),*redeuce);

	RooFormulaVar* cutvar = new RooFormulaVar("PPTM", (condition + " && " + MuonId_str + " == 1").c_str(), RooArgList(MuonId_var, quantityPt, quantityEta));
	RooDataSet *Data_PASSING = new RooDataSet("DATA_PASS", "DATA_PASS", DataTree, RooArgSet(InvariantMass, MuonId_var, quantityPt, quantityEta), *cutvar);
	
	cout << "cloning1\n";
	RooDataHist* dh_ALL     = Data_ALL->binnedClone();
	cout << "cloning2\n";
	RooDataHist* dh_PASSING = Data_PASSING->binnedClone();
	cout << "end\n";
	
	TCanvas* c_all  = new TCanvas;
	TCanvas* c_pass = new TCanvas;
	
	RooPlot *frame = InvariantMass.frame(RooFit::Title("Invariant Mass"));
	   
	// SIGNAL VARIABLES
	RooRealVar mean("mean","mean",3.094, 3.07, 3.1);
	RooRealVar sigma("sigma","sigma",0.05*(_mmax-_mmin),0.,0.5*(_mmax-_mmin));
	RooRealVar sigma_cb("sigma_cb","sigma_cb", 0.038);
	RooRealVar alpha("alpha", "alpha", 1.71);
	RooRealVar n("n", "n", 3.96);
	n.setConstant(kTRUE);
	   
	//FIT FUNCTIONS
	RooGaussian gaussian("GS","GS",InvariantMass,mean,sigma);
	RooCBShape crystalball("CB", "CB", InvariantMass, mean, sigma_cb, alpha, n);

	// BACKGROUND VARIABLES
	RooRealVar a0("a0", "a0", 0, -10, 10);
	RooRealVar a1("a1", "a1", 0, -10, 10);

	// BACKGROUND FUNCTION
	RooChebychev background("background","background", InvariantMass, RooArgList(a0,a1));
	
	double n_signal_initial_total = 50000;
	double n_back_initial = 10000;
	
	RooRealVar frac1("frac1","frac1",0.5);

	RooAddPdf* signal;
	
	signal      = new RooAddPdf("signal", "signal", RooArgList(gaussian, crystalball), RooArgList(frac1));
	
	RooRealVar n_signal_total("n_signal_total","n_signal_total",n_signal_initial_total,0.,Data_ALL->sumEntries());
	RooRealVar n_signal_total_pass("n_signal_total_pass","n_signal_total_pass",n_signal_initial_total,0.,Data_PASSING->sumEntries());

	RooRealVar n_back("n_back","n_back",n_back_initial,0.,Data_ALL->sumEntries());
	RooRealVar n_back_pass("n_back_pass","n_back_pass",n_back_initial,0.,Data_PASSING->sumEntries());
	
	RooAddPdf* model;
	RooAddPdf* model_pass;
	
	model      = new RooAddPdf("model","model", RooArgList(*signal, background),RooArgList(n_signal_total, n_back));
	model_pass = new RooAddPdf("model_pass", "model_pass", RooArgList(*signal, background),RooArgList(n_signal_total_pass, n_back_pass));
	
	// SIMULTANEOUS FIT
	RooCategory sample("sample","sample") ;
	sample.defineType("All") ;
	sample.defineType("PASSING") ;
	
	RooDataHist combData("combData","combined data",InvariantMass,Index(sample),Import("ALL",*dh_ALL),Import("PASSING",*dh_PASSING));
	
	RooSimultaneous simPdf("simPdf","simultaneous pdf",sample) ;
	
	simPdf.addPdf(*model,"ALL");
	simPdf.addPdf(*model_pass,"PASSING");
	
	RooFitResult* fitres = new RooFitResult;
	fitres = simPdf.fitTo(combData, RooFit::Save());
	
	// OUTPUT ARRAY
	double* output = new double[4];
	
	RooRealVar* yield_ALL = (RooRealVar*) fitres->floatParsFinal().find("n_signal_total");
	RooRealVar* yield_PASS = (RooRealVar*) fitres->floatParsFinal().find("n_signal_total_pass");
	
	output[0] = yield_ALL->getVal();
	output[1] = yield_PASS->getVal();
	
	output[2] = yield_ALL->getError();
	output[3] = yield_PASS->getError();
	
	frame->SetTitle("ALL");
	frame->SetXTitle("#mu^{+}#mu^{-} invariant mass [GeV/c^{2}]");
	Data_ALL->plotOn(frame);
	
	model->plotOn(frame);
	model->plotOn(frame,RooFit::Components("GS"),RooFit::LineStyle(kDashed),RooFit::LineColor(kGreen));
	model->plotOn(frame,RooFit::Components("CB"),RooFit::LineStyle(kDashed),RooFit::LineColor(kMagenta - 5));
	model->plotOn(frame,RooFit::Components("background"),RooFit::LineStyle(kDashed),RooFit::LineColor(kRed));
	
	c_all->cd();
	frame->Draw("");
	
	RooPlot *frame_pass = InvariantMass.frame(RooFit::Title("Invariant Mass"));
	
	c_pass->cd();
	
	frame_pass->SetTitle("PASSING");
	frame_pass->SetXTitle("#mu^{+}#mu^{-} invariant mass [GeV/c^{2}]");
	Data_PASSING->plotOn(frame_pass);
	
	model_pass->plotOn(frame_pass);
	model_pass->plotOn(frame_pass,RooFit::Components("GS"),RooFit::LineStyle(kDashed),RooFit::LineColor(kGreen));
	model_pass->plotOn(frame_pass,RooFit::Components("CB"),RooFit::LineStyle(kDashed),RooFit::LineColor(kMagenta - 5));
	model_pass->plotOn(frame_pass,RooFit::Components("background"),RooFit::LineStyle(kDashed),RooFit::LineColor(kRed));
	
	frame_pass->Draw();
	
	c_pass->SetLogy();
	c_all->SetLogy();

	if (savePath != NULL)
	{
		c_pass->SaveAs((string(savePath) + condition + "_ALL.png").c_str());
		c_all->SaveAs ((string(savePath) + condition + "_PASS.png").c_str());
	}
		
	// DELETING ALLOCATED MEMORY
	//delete[] limits;
	//
	delete file0;
	//
	delete Data_ALL;
	delete Data_PASSING;
	//
	delete dh_ALL;
	delete dh_PASSING;
	//
	delete cutvar;
	delete redeuce;
	//
	delete signal;
	//
	delete c_all;
	delete c_pass;
	//
	delete model;
	delete model_pass;
	delete fitres;
	
	return output;
}