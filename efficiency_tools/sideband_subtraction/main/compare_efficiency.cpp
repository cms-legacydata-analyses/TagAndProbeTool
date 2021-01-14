 /*
!--------------------------------
!Purpose: Compare efficiency of files
!--------------------------------	
!author: Allan Jales
!--------------------------------
*/

//CONFIGS

int useScheme = 2;
//Jpsi    Sideband Run vs Jpsi    Fitting  Run vs Monte Carlo
//Upsilon Sideband Run vs Upsilon Fitting  Run vs Monte Carlo

//Muon id anlyse
bool doTracker    = true;
bool doStandalone = true;
bool doGlobal     = true;

//Root files and paths for Tefficiency objects inside these files
const char* sid_run_upsilon[][2] = {
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Pt_Tracker_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Eta_Tracker_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Phi_Tracker_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Pt_Standalone_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Eta_Standalone_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Phi_Standalone_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Pt_Global_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Eta_Global_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Phi_Global_Probe_Efficiency"}
};

const char* sid_run_jpsi[][2] = {
	{"../results/Jpsi_Run_2011/generated_hist.root", "efficiency/plots/Muon_Pt_Tracker_Probe_Efficiency"},
	{"../results/Jpsi_Run_2011/generated_hist.root", "efficiency/plots/Muon_Eta_Tracker_Probe_Efficiency"},
	{"../results/Jpsi_Run_2011/generated_hist.root", "efficiency/plots/Muon_Phi_Tracker_Probe_Efficiency"},
	{"../results/Jpsi_Run_2011/generated_hist.root", "efficiency/plots/Muon_Pt_Standalone_Probe_Efficiency"},
	{"../results/Jpsi_Run_2011/generated_hist.root", "efficiency/plots/Muon_Eta_Standalone_Probe_Efficiency"},
	{"../results/Jpsi_Run_2011/generated_hist.root", "efficiency/plots/Muon_Phi_Standalone_Probe_Efficiency"},
	{"../results/Jpsi_Run_2011/generated_hist.root", "efficiency/plots/Muon_Pt_Global_Probe_Efficiency"},
	{"../results/Jpsi_Run_2011/generated_hist.root", "efficiency/plots/Muon_Eta_Global_Probe_Efficiency"},
	{"../results/Jpsi_Run_2011/generated_hist.root", "efficiency/plots/Muon_Phi_Global_Probe_Efficiency"}
};

const char* fit_run_upsilon[][2] = {
	{"../../result_thomas/Upsilon/Tracker/pT/Efficiency_Run2011.root",     "Efficiency"},
	{"../../result_thomas/Upsilon/Tracker/Eta/Efficiency_Run2011.root",    "Efficiency"},
	{"../../result_thomas/Upsilon/Tracker/Phi/Efficiency_Run2011.root",    "Efficiency"},
	{"../../result_thomas/Upsilon/Standalone/pT/Efficiency_Run2011.root",  "Efficiency"},
	{"../../result_thomas/Upsilon/Standalone/Eta/Efficiency_Run2011.root", "Efficiency"},
	{"../../result_thomas/Upsilon/Standalone/Phi/Efficiency_Run2011.root", "Efficiency"},
	{"../../result_thomas/Upsilon/Global/pT/Efficiency_Run2011.root",      "Efficiency"},
	{"../../result_thomas/Upsilon/Global/Eta/Efficiency_Run2011.root",     "Efficiency"},
	{"../../result_thomas/Upsilon/Global/Phi/Efficiency_Run2011.root",     "Efficiency"}
};

const char* fit_run_jpsi[][2] = {
	{"../../result_thomas/JPsi/Tracker/pT/Efficiency_Run2011.root",     "Efficiency"},
	{"../../result_thomas/JPsi/Tracker/Eta/Efficiency_Run2011.root",    "Efficiency"},
	{"../../result_thomas/JPsi/Tracker/Phi/Efficiency_Run2011.root",    "Efficiency"},
	{"../../result_thomas/JPsi/Standalone/pT/Efficiency_Run2011.root",  "Efficiency"},
	{"../../result_thomas/JPsi/Standalone/Eta/Efficiency_Run2011.root", "Efficiency"},
	{"../../result_thomas/JPsi/Standalone/Phi/Efficiency_Run2011.root", "Efficiency"},
	{"../../result_thomas/JPsi/Global/pT/Efficiency_Run2011.root",      "Efficiency"},
	{"../../result_thomas/JPsi/Global/Eta/Efficiency_Run2011.root",     "Efficiency"},
	{"../../result_thomas/JPsi/Global/Phi/Efficiency_Run2011.root",     "Efficiency"}
};

const char* mc_upsilon[][2] = {
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Pt_Tracker_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Eta_Tracker_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Phi_Tracker_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Pt_Standalone_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Eta_Standalone_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Phi_Standalone_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Pt_Global_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Eta_Global_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Phi_Global_Probe_Efficiency"}
};

const char* mc_jpsi[][2] = {
	{"../results/Jpsi_MC_2020/generated_hist.root", "efficiency/plots/Muon_Pt_Tracker_Probe_Efficiency"},
	{"../results/Jpsi_MC_2020/generated_hist.root", "efficiency/plots/Muon_Eta_Tracker_Probe_Efficiency"},
	{"../results/Jpsi_MC_2020/generated_hist.root", "efficiency/plots/Muon_Phi_Tracker_Probe_Efficiency"},
	{"../results/Jpsi_MC_2020/generated_hist.root", "efficiency/plots/Muon_Pt_Standalone_Probe_Efficiency"},
	{"../results/Jpsi_MC_2020/generated_hist.root", "efficiency/plots/Muon_Eta_Standalone_Probe_Efficiency"},
	{"../results/Jpsi_MC_2020/generated_hist.root", "efficiency/plots/Muon_Phi_Standalone_Probe_Efficiency"},
	{"../results/Jpsi_MC_2020/generated_hist.root", "efficiency/plots/Muon_Pt_Global_Probe_Efficiency"},
	{"../results/Jpsi_MC_2020/generated_hist.root", "efficiency/plots/Muon_Eta_Global_Probe_Efficiency"},
	{"../results/Jpsi_MC_2020/generated_hist.root", "efficiency/plots/Muon_Phi_Global_Probe_Efficiency"}
};

const char* sbs_mc_jpsi[][2] = {
	{"../../result_thomas/JPsi/Tracker/pT/Efficiency_Run2011.root",     "Efficiency"},
	{"../../result_thomas/JPsi/Tracker/Eta/Efficiency_Run2011.root",    "Efficiency"},
	{"../../result_thomas/JPsi/Tracker/Phi/Efficiency_Run2011.root",    "Efficiency"},
	{"../../result_thomas/JPsi/Standalone/pT/Efficiency_Run2011.root",  "Efficiency"},
	{"../../result_thomas/JPsi/Standalone/Eta/Efficiency_Run2011.root", "Efficiency"},
	{"../../result_thomas/JPsi/Standalone/Phi/Efficiency_Run2011.root", "Efficiency"},
	{"../../result_thomas/JPsi/Global/pT/Efficiency_Run2011.root",      "Efficiency"},
	{"../../result_thomas/JPsi/Global/Eta/Efficiency_Run2011.root",     "Efficiency"},
	{"../../result_thomas/JPsi/Global/Phi/Efficiency_Run2011.root",     "Efficiency"}
};

const char* fit_mc_upsilon[][2] = {
	{"../../result_thomas/MC_JPsi_fit/Tracker/pT/Efficiency_Run2011.root",     "Efficiency"},
	{"../../result_thomas/MC_JPsi_fit/Tracker/Eta/Efficiency_Run2011.root",    "Efficiency"},
	{"../../result_thomas/MC_JPsi_fit/Tracker/Phi/Efficiency_Run2011.root",    "Efficiency"},
	{"../../result_thomas/MC_JPsi_fit/Standalone/pT/Efficiency_Run2011.root",  "Efficiency"},
	{"../../result_thomas/MC_JPsi_fit/Standalone/Eta/Efficiency_Run2011.root", "Efficiency"},
	{"../../result_thomas/MC_JPsi_fit/Standalone/Phi/Efficiency_Run2011.root", "Efficiency"},
	{"../../result_thomas/MC_JPsi_fit/Global/pT/Efficiency_Run2011.root",      "Efficiency"},
	{"../../result_thomas/MC_JPsi_fit/Global/Eta/Efficiency_Run2011.root",     "Efficiency"},
	{"../../result_thomas/MC_JPsi_fit/Global/Phi/Efficiency_Run2011.root",     "Efficiency"}
};

//How comparisons will be saved
const char* resultNames[] = {
	"Muon_Pt_Tracker_Probe_Efficiency.pdf",
	"Muon_Eta_Tracker_Probe_Efficiency.pdf",
	"Muon_Phi_Tracker_Probe_Efficiency.pdf",
	"Muon_Pt_Standalone_Probe_Efficiency.pdf",
	"Muon_Eta_Standalone_Probe_Efficiency.pdf",
	"Muon_Phi_Standalone_Probe_Efficiency.pdf",
	"Muon_Pt_Global_Probe_Efficiency.pdf",
	"Muon_Eta_Global_Probe_Efficiency.pdf",
	"Muon_Phi_Global_Probe_Efficiency.pdf",
};

//Label of each comparison
const char* labelScheme[][3] = {
	{"J/#psi Sideband",       "J/#psi Fitting",       "Simulated data"},
	{"#Upsilon(1S) Sideband", "#Upsilon(1S) Fitting", "Simulated data"},
	{"#Upsilon(1S) Sideband", "#Upsilon(1S) Fitting", "#Upsilon(1S) MC Fitting"},
};

//Where to save each comparison
const char* directoriesToSave[] = {
	"../results/Comparison/Comparison_Jpsi/",
	"../results/Comparison/Comparison_Upsilon/",
	"../results/Comparison/Comparison_Upsilon/"
};

//Colors for each comparison
int colorScheme[][3] = {
	{kBlue + 1, kGreen - 2, kRed - 4},
	{kBlue + 1, kGreen - 2, kRed - 4},
	{kBlue + 1, kGreen - 2, kRed - 4}
};

//Choose a option between two
const char* chooseOption(int item, const char* option0, const char* option1, const char* option2)
{
	switch (item)
	{
		case 0:  return option0;
		case 1:  return option1;
		case 2:  return option2;
		default: return NULL;
	}
}

//Get a file path
const char* getFilePath(int index, int row, int col)
{
	switch (useScheme)
	{
		case 0:  return chooseOption(index, sid_run_jpsi   [row][col], fit_run_jpsi   [row][col], mc_jpsi   [row][col]);
		case 1:  return chooseOption(index, sid_run_upsilon[row][col], fit_run_upsilon[row][col], mc_upsilon[row][col]);
		case 2:  return chooseOption(index, sid_run_upsilon[row][col], fit_run_upsilon[row][col], fit_mc_upsilon[row][col]);
		default:
			std::cerr << "ERROR: useScheme not found\n";
			std::cerr << "ABORTING...\n";
			abort();
			return NULL;
	}
}

//Get TEfficiency from file_path
TEfficiency* get_eff_plot(const char* file_path, const char* hist_path)
{
	TFile *file0 = TFile::Open(file_path);

	if (file0 == NULL)
	{
		std::cerr << "Could not find \"" << file_path << "\" file.\n";
		std::cerr << "ABORTING...\n";
		abort();
	}

	TEfficiency* pEff0 = (TEfficiency*)file0->Get(hist_path);

	if (pEff0 == NULL)
	{
		std::cerr << "Could not find the histogram path \"" << hist_path << "\" in \"" << file_path << "\" file.\n";
		std::cerr << "ABORTING...\n";
		abort();
	}

	return pEff0;
}

//Compare two Tefficiencies and save
void compare_eff_plot(TEfficiency* pEff0, TEfficiency* pEff1, TEfficiency* pEff2, string resultName, const char* quantity = "")
{
	//Create canvas
	TCanvas* c1 = new TCanvas();
	//gStyle->SetOptTitle(0);
	c1->SetMargin(0.10, 0.03, 0.11, 0.07);

	//Plot
	pEff0->SetMarkerColor(colorScheme[useScheme][0]);
	pEff0->SetLineColor(colorScheme[useScheme][0]);
	pEff0->SetLineWidth(1);
	pEff0->Draw();

	pEff1->SetMarkerColor(colorScheme[useScheme][1]);
	pEff1->SetLineColor(colorScheme[useScheme][1]);
	pEff1->SetLineWidth(1);
	pEff1->Draw("same");

	pEff2->SetMarkerColor(colorScheme[useScheme][2]);
	pEff2->SetLineColor(colorScheme[useScheme][2]);
	pEff2->SetLineWidth(1);
	pEff2->Draw("same");
	
	//Set range in y axis
	gPad->Update();
	auto graph = pEff0->GetPaintedGraph(); 
	graph->SetMinimum(0.0);
	graph->SetMaximum(1.2);
	gPad->Update();

	//Set range if is pT
	if (strcmp(quantity, "Pt") == 0)
	{
		pEff0->GetPaintedGraph()->GetHistogram()->GetXaxis()->SetRangeUser(0.,40.);
		graph->SetMinimum(0.5);
		graph->SetMaximum(1.2);

		//pEff0->SetTitle("Efficiency of Tracker Muon Probe;p_{T} [GeV/c];Efficiency");
	}
	
	//Set range if is eta
	if (strcmp(quantity, "Eta") == 0)
	{
		pEff0->GetPaintedGraph()->GetHistogram()->GetXaxis()->SetRangeUser(-3.,3.);
		graph->SetMinimum(0.8);
		graph->SetMaximum(1.08);

		//pEff0->SetTitle("Efficiency of Tracker Muon Probe;#eta;Efficiency");
	}
	
	//Set range if is phi
	if (strcmp(quantity, "Phi") == 0)
	{
		graph->SetMinimum(0.8);
		graph->SetMaximum(1.08);

		//pEff0->SetTitle("Efficiency of Tracker Muon Probe;#phi [rad];Efficiency");
	}

	//Legenda
	TLegend* tl = new TLegend(0.68,0.73,0.94,0.88);
	tl->SetTextSize(0.04);
	tl->AddEntry(pEff0, labelScheme[useScheme][0], "lp");
	tl->AddEntry(pEff1, labelScheme[useScheme][1], "lp");
	tl->AddEntry(pEff2, labelScheme[useScheme][2], "lp");
	tl->Draw();

	//CMS Open Data
	TLatex* txCOD = new TLatex();
	txCOD->SetTextSize(0.04);
	txCOD->SetTextAlign(12);
	txCOD->SetTextFont(42);
	txCOD->SetNDC(kTRUE);
	txCOD->DrawLatex(0.14,0.85,Form("#bf{CMS Open Data}"));


	//Saving as png

	//Path where is going to save results 
	const char* directoryToSave = directoriesToSave[useScheme];

	//Check if dir exists and create
	if (gSystem->AccessPathName(directoryToSave))
	{
		if (gSystem->mkdir(directoryToSave, true))
		{
			cerr << "\"" << directoryToSave << "\" directory not found and could not be created ERROR\n";
			abort();
		}
		else
		{
			cout << "\"" << directoryToSave << "\" directory created OK\n";
		}
	}
	else
	{
		cout << "\"" << directoryToSave << "\" directory OK\n";
	}

	//Path of file
	string saveAs = string(directoryToSave) + resultName;

	c1->SaveAs(saveAs.data());
}

//Compare efficiency
void compare_efficiency()
{
	for (int i = 0; i < 9; i++)
	{
		if (!doTracker    && i < 3)
			continue;
		if (!doStandalone && i >= 3 && i < 6)
			continue;
		if (!doGlobal     && i >= 6)
			continue;

		//Get Tefficiency object
		TEfficiency* pEff0 = get_eff_plot(getFilePath(0,i,0), getFilePath(0,i,1));
		TEfficiency* pEff1 = get_eff_plot(getFilePath(1,i,0), getFilePath(1,i,1));
		TEfficiency* pEff2 = get_eff_plot(getFilePath(2,i,0), getFilePath(2,i,1));

		//Name of file
		string resultName = resultNames[i];

		std::cout << "\nEvaluating: " << resultName << "\n";
		std::cout << "----------------------------------\n";

		//Get which quantity is working on
		const char* quantity = "";

		if (regex_match(resultNames[i], regex(".*Pt.*")))
			quantity = "Pt";

		if (regex_match(resultNames[i], regex(".*Eta.*")))
			quantity = "Eta";

		if (regex_match(resultNames[i], regex(".*Phi.*")))
			quantity = "Phi";


		//Compare efficiency polts
		compare_eff_plot(pEff0, pEff1, pEff2, resultName, quantity);
		std::cout << "----------------------------------\n";
	}

	cout << "\nUse Scheme: " << useScheme << "\n";
	cout << "Done. All result files can be found at \"" << directoriesToSave[useScheme] << "\"\n";
}