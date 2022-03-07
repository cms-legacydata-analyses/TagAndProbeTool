#ifndef DOFIT_HEADER
#define DOFIT_HEADER
//We start by declaring the nature of our dataset. (Is the data real or simulated?)
const char* output_folder_name = "Upsilon1S_MC_2020";

//Header of this function
double _mmin = 9;
double _mmax = 10.8;
double fit_bins = 0; //Let it 0 if dont want to change

//Information for output at the end of run
const char* fit_functions = "Gaussian + CrystalBall";
string prefix_file_name = "";
#endif
using namespace RooFit;

//Returns array with [yield_all, yield_pass, err_all, err_pass]
#define DEFAULT_FUCTION_NAME_USED
double* doFit(string condition, string MuonId, const char* savePath = NULL)
{
	cout << "----- Fitting data on bin -----\n";
	cout << "Conditions: " << condition << "\n";
	cout << "-------------------------------\n";

	string MuonId_str = "";
	if      (MuonId == "trackerMuon")    MuonId_str = "PassingProbeTrackingMuon";
	else if (MuonId == "standaloneMuon") MuonId_str = "PassingProbeStandAloneMuon";
	else if (MuonId == "globalMuon")     MuonId_str = "PassingProbeGlobalMuon";
	
	TFile* file0    = TFile::Open("DATA/TagAndProbe_Upsilon_MC.root");
	TTree* DataTree = (TTree*)file0->Get(("tagandprobe"));
	
	RooCategory MuonId_var(MuonId_str.c_str(), MuonId_str.c_str());
	MuonId_var.defineType("Passing", 1);
	MuonId_var.defineType("Failing", 0);
	RooRealVar  InvariantMass("InvariantMass", "InvariantMass", _mmin, _mmax);
	RooRealVar  ProbeMuon_Pt ("ProbeMuon_Pt",  "ProbeMuon_Pt",  0., 40.);
	RooRealVar  ProbeMuon_Eta("ProbeMuon_Eta", "ProbeMuon_Eta", -2.4, 2.4);
	RooRealVar  ProbeMuon_Phi("ProbeMuon_Phi", "ProbeMuon_Phi", -TMath::Pi(), TMath::Pi());
	RooRealVar  TagMuon_Pt   ("TagMuon_Pt",    "TagMuon_Pt",    0., 40.);
	RooRealVar  TagMuon_Eta  ("TagMuon_Eta",   "TagMuon_Eta",   -2.4, 2.4);
	RooRealVar  TagMuon_Phi  ("TagMuon_Phi",   "TagMuon_Phi",   -TMath::Pi(), TMath::Pi());

	if (fit_bins > 0) InvariantMass.setBins(fit_bins);
	fit_bins = InvariantMass.getBinning().numBins();

	RooFormulaVar* fv_CUT   = new RooFormulaVar("tag_cut", "TagMuon_Pt >= 7.0 && fabs(TagMuon_Eta) <= 2.4", RooArgList(TagMuon_Pt, TagMuon_Eta, TagMuon_Phi));
	RooDataSet*    Data_CUT = new RooDataSet("data_cut", "data_cut", DataTree, RooArgSet(InvariantMass, MuonId_var, ProbeMuon_Pt, ProbeMuon_Eta, ProbeMuon_Phi), *fv_CUT);

	RooFormulaVar* fv_ALL   = new RooFormulaVar("probe_on_bin", condition.c_str(), RooArgList(ProbeMuon_Pt, ProbeMuon_Eta, ProbeMuon_Phi));
	RooDataSet*    Data_ALL = new RooDataSet("data_all", "data_all", Data_CUT, RooArgSet(InvariantMass, MuonId_var, ProbeMuon_Pt, ProbeMuon_Eta, ProbeMuon_Phi), *fv_ALL);

	RooFormulaVar* fv_PASS      = new RooFormulaVar("passing_probe_on_bin", (MuonId_str + "==1").c_str(), RooArgList(MuonId_var));
	RooDataSet*    Data_PASSING = new RooDataSet("data_pass", "data_pass", Data_ALL, RooArgSet(InvariantMass, MuonId_var, ProbeMuon_Pt, ProbeMuon_Eta, ProbeMuon_Phi), *fv_PASS);
	
	RooDataHist* dh_ALL     = new RooDataHist(Data_ALL->GetName(),    Data_ALL->GetTitle(),     RooArgSet(InvariantMass), *Data_ALL);
	RooDataHist* dh_PASSING = new RooDataHist(Data_PASSING->GetName(),Data_PASSING->GetTitle(), RooArgSet(InvariantMass), *Data_PASSING);
	
	TCanvas* c_all  = new TCanvas;
	TCanvas* c_pass = new TCanvas;

	RooPlot* frame = InvariantMass.frame(RooFit::Title("Invariant Mass"));

	// GAUSSIAN VARIABLES
	RooRealVar mean("mean","mean",9.46030, 9.3, 9.6);
	RooRealVar sigma("sigma","sigma",0.05, 0., 0.18);
	RooRealVar sigma_cb("sigma_cb","sigma_cb", 0.138, 0.01, 0.18);
	RooRealVar alpha("alpha", "alpha", 1.71, -5., 5.);
	RooRealVar n("n", "n", 3.96, 3., 10.);
	   
	//FIT FUNCTIONS
	RooGaussian gaussian("GS","GS",InvariantMass,mean,sigma);
	RooCBShape crystalball("CB", "CB", InvariantMass, mean, sigma_cb, alpha, n);
	
	RooRealVar frac1("frac1","frac1",0.5);

	RooAddPdf* signal = new RooAddPdf("signal", "signal", RooArgList(gaussian, crystalball), RooArgList(frac1));
	
	RooRealVar n_signal_total("n_signal_total","n_signal_total",Data_ALL->sumEntries()/2,0.,Data_ALL->sumEntries());
	RooRealVar n_signal_total_pass("n_signal_total_pass","n_signal_total_pass",Data_PASSING->sumEntries()/2,0.,Data_PASSING->sumEntries());
	
	RooAddPdf* model      = new RooAddPdf("model","model", RooArgList(*signal),RooArgList(n_signal_total));
	RooAddPdf* model_pass = new RooAddPdf("model_pass", "model_pass", RooArgList(*signal),RooArgList(n_signal_total_pass));
	
	// SIMULTANEOUS FIT
	RooCategory sample("sample","sample") ;
	sample.defineType("All") ;
	sample.defineType("Passing") ;
	
	RooDataHist combData("combData","combined data",InvariantMass,Index(sample),Import("ALL",*dh_ALL),Import("PASSING",*dh_PASSING));
	
	RooSimultaneous simPdf("simPdf","simultaneous pdf",sample) ;
	
	simPdf.addPdf(*model,"ALL");
	simPdf.addPdf(*model_pass,"PASSING");
	
	RooFitResult* fitres = new RooFitResult;
	fitres = simPdf.fitTo(combData, RooFit::Save());
	
	// OUTPUT ARRAY
	double* output = new double[4];
	
	RooRealVar* yield_all = (RooRealVar*) fitres->floatParsFinal().find("n_signal_total");
	RooRealVar* yield_pass = (RooRealVar*) fitres->floatParsFinal().find("n_signal_total_pass");
	
	output[0] = yield_all->getVal();
	output[1] = yield_pass->getVal();
	
	output[2] = yield_all->getError();
	output[3] = yield_pass->getError();
	
	frame->SetTitle("ALL");
	frame->SetXTitle("#mu^{+}#mu^{-} invariant mass [GeV/c^{2}]");
	Data_ALL->plotOn(frame);
	
	model->plotOn(frame);
	model->plotOn(frame,RooFit::Components("GS"),RooFit::LineStyle(kDashed),RooFit::LineColor(kGreen));
	model->plotOn(frame,RooFit::Components("CB"),RooFit::LineStyle(kDashed),RooFit::LineColor(kMagenta - 5));
	
	c_all->cd();
	frame->Draw("");
	
	RooPlot* frame_pass = InvariantMass.frame(RooFit::Title("Invariant Mass"));
	
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
		c_pass->SaveAs((string(savePath) + condition + "_PASS.png").c_str());
		c_all->SaveAs ((string(savePath) + condition + "_ALL.png").c_str());
	}

	cout << "-------------------------------\n";

	// Deleting allocated memory
	delete file0;

	delete fv_CUT;
	delete fv_ALL;
	delete fv_PASS;
	delete Data_CUT;
	delete Data_ALL;
	delete Data_PASSING;
	delete dh_ALL;
	delete dh_PASSING;

	delete c_all;
	delete c_pass;

	delete model;
	delete model_pass;

	delete signal;
	delete fitres;
	
	return output;
}
