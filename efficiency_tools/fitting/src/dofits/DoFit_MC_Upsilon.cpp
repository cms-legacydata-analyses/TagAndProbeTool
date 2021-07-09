using namespace RooFit;

//We start by declaring the nature of our dataset. (Is the data real or simulated?)
const char* output_folder_name = "Upsilon1S_MC_2020";

//Header of this function
const char* fit_functions = "Gaussian + CrystalBall";
double _mmin = 9;  double _mmax = 10.8;
double fit_bins = 0;

double* doFit(string condition, string MuonId, string quant, const char* savePath = NULL) // RETURNS ARRAY WITH [yield_all, yield_pass, err_all, err_pass]    ->   OUTPUT ARRAY
{
	string MuonId_str = "";
	if (MuonId == "trackerMuon")
		MuonId_str = "PassingProbeTrackingMuon";
	if (MuonId == "standaloneMuon")
		MuonId_str = "PassingProbeStandAloneMuon";
	if (MuonId == "globalMuon")
		MuonId_str = "PassingProbeGlobalMuon";
	
	TFile *file0       = TFile::Open("DATA/TagAndProbe_Upsilon_MC.root");
	TTree *DataTree    = (TTree*)file0->Get(("tagandprobe"));
	
	RooRealVar MuonId_var(MuonId_str.c_str(), MuonId_str.c_str(), 0, 1); //Muon_Id
	
	RooRealVar InvariantMass("InvariantMass", "InvariantMass", _mmin, _mmax);
	if (fit_bins != 0)
		InvariantMass.setBins(fit_bins);
	fit_bins = InvariantMass.getBinning().numBins();
	
	double* limits = new double[2];
	if (quant == "Pt") {
		limits[0] = 0;
		limits[1] = 40;
	}
	if (quant == "Eta") {
		limits[0] = -3;
		limits[1] = 3;
	}
	if (quant == "Phi") {
		limits[0] = -2;
		limits[1] = 2;
	}
	RooRealVar quantity(("ProbeMuon_" + quant).c_str(), ("ProbeMuon_" + quant).c_str(), limits[0], limits[1]);
	
	RooFormulaVar* redeuce = new RooFormulaVar("PPTM", condition.c_str(), RooArgList(quantity));
	RooDataSet *Data_ALL    = new RooDataSet("DATA", "DATA", DataTree, RooArgSet(InvariantMass, MuonId_var, quantity),*redeuce);
	RooFormulaVar* cutvar = new RooFormulaVar("PPTM", (condition + " && " + MuonId_str + " == 1").c_str() , RooArgList(MuonId_var, quantity));

	RooDataSet *Data_PASSING = new RooDataSet("DATA_PASS", "DATA_PASS", DataTree, RooArgSet(InvariantMass, MuonId_var, quantity), *cutvar);
	
	RooDataHist* dh_ALL     = Data_ALL->binnedClone();
	RooDataHist* dh_PASSING = Data_PASSING->binnedClone();
	
	TCanvas* c_all  = new TCanvas;
	TCanvas* c_pass = new TCanvas;
	
	RooPlot *frame = InvariantMass.frame(RooFit::Title("Invariant Mass"));
	   
	// GAUSSIAN VARIABLES
	RooRealVar mean("mean","mean",9.46030, 9.3, 9.6);
	RooRealVar sigma("sigma","sigma",0.05, 0., 0.18);
	RooRealVar sigma_cb("sigma_cb","sigma_cb", 0.138, 0.01, 0.18);
	RooRealVar alpha("alpha", "alpha", 1.71, -5., 5.);
	RooRealVar n("n", "n", 3.96, 3., 10.);
	   
	//FIT FUNCTIONS
	RooGaussian gaussian("GS","GS",InvariantMass,mean,sigma);
	RooCBShape crystalball("CB", "CB", InvariantMass, mean, sigma_cb, alpha, n);
	
	double n_signal_initial_total = 50000;
	
	RooRealVar frac1("frac1","frac1",0.5);

	RooAddPdf* signal;
	
	signal      = new RooAddPdf("signal", "signal", RooArgList(gaussian, crystalball), RooArgList(frac1));
	
	RooRealVar n_signal_total("n_signal_total","n_signal_total",n_signal_initial_total,0.,Data_ALL->sumEntries());
	RooRealVar n_signal_total_pass("n_signal_total_pass","n_signal_total_pass",n_signal_initial_total,0.,Data_PASSING->sumEntries());
	
	RooAddPdf* model;
	RooAddPdf* model_pass;
	
	model      = new RooAddPdf("model","model", RooArgList(*signal),RooArgList(n_signal_total));
	model_pass = new RooAddPdf("model_pass", "model_pass", RooArgList(*signal),RooArgList(n_signal_total_pass));
	
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
	
	frame_pass->Draw();

	if (savePath != NULL)
	{
		c_pass->SaveAs((string(savePath) + condition + "_ALL.png").c_str());
		c_all->SaveAs ((string(savePath) + condition + "_PASS.png").c_str());
	}
		
	// DELETING ALLOCATED MEMORY
	delete[] limits;
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
