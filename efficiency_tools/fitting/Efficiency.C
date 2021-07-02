#include "src/compare_efficiency.C"


//Change if you need
#include "src/dofits/DoFit_Upsilon.cpp"

#include "src/get_conditions.cpp"
#include "src/get_efficiency.cpp"
#include "src/change_bin.cpp"
#include "src/make_hist.cpp"
#include "src/McYield.cpp"


void createFolder(const char* folderPath, bool deleteOld = false)
{
	//Delete old directory for saving files
	if (deleteOld == true)
		gSystem->Exec((string("rm -r ") + string(folderPath)).c_str());

	//Check if dir exists and create
	if (gSystem->AccessPathName(folderPath))
		if (gSystem->mkdir(folderPath, true))
		{
			cerr << "\"" << folderPath << "\" path could not be found and could not be created ERROR\n";
			cerr << "Try to create manually this folder path\n";
			abort();
		}
		else
			cout << "\"" << folderPath << "\" directory created OK\n";
	else
		cout << "\"" << folderPath << "\" directory OK\n";
}

void Efficiency()
{
	//We start by declaring the nature of our dataset. (Is the data real or simulated?)
	bool DataIsMC   = false;

	//Which Muon Id do you want to study?
	string MuonId   = "trackerMuon";
	//string MuonId   = "standaloneMuon";
	//string MuonId   = "globalMuon";
	//Which quantity do you want to use?
	
	/*-----------------------------------I N S E R T    C O D E    H E R E-----------------------------------*/
	string quantity = "Pt";     double bins[] = {0., 2.0, 3.4, 4.0, 4.4, 4.7, 5.0, 5.6, 5.8, 6.0, 6.2, 6.4, 6.6, 6.8, 7.3, 9.5, 13.0, 17.0, 40.};
	//string quantity = "Eta";    double bins[] = {-2.4, -1.8, -1.4, -1.2, -1.0, -0.8, -0.5, -0.2, 0, 0.2, 0.5, 0.8, 1.0, 1.2, 1.4, 1.8, 2.4};
	//string quantity = "Phi";    double bins[] = {-3.0, -1.8, -1.6, -1.2, -1.0, -0.7, -0.4, -0.2, 0, 0.2, 0.4, 0.7, 1.0, 1.2, 1.6, 1.8, 3.0};
	int bin_n = sizeof(bins)/sizeof(*bins) - 1;
	 /*------------------------------------------------------------------------------------------------------*/
	
	string* conditions = get_conditions(bin_n, bins, "ProbeMuon_" + quantity);
	double ** yields_n_errs = new double*[bin_n];

	//-------------------------------------
	// Do Fit
	//-------------------------------------

	//Path where is going to save results 
	const char* directoryToSave = "fit_result/";
	createFolder(directoryToSave, true);
	
	// Loop for every bin and fit it
	for (int i = 0; i < bin_n; i++)
	{
		if (DataIsMC)
			yields_n_errs[i] = McYield(conditions[i], quantity);
		else
			yields_n_errs[i] = doFit(conditions[i], MuonId, quantity, directoryToSave);
			//doFit returns: [yield_all, yield_pass, err_all, err_pass]
	}
	
	TH1F *yield_ALL  = make_hist("ALL" , yields_n_errs, 0, bin_n, bins, DataIsMC);
	TH1F *yield_PASS = make_hist("PASS", yields_n_errs, 1, bin_n, bins, DataIsMC);
	
	//----------------------SAVING RESULTS TO Histograms.root--------------------//
	//useful if we require to change the fit on a specific set of bins
	TFile* EfficiencyFile = TFile::Open("Histograms.root","RECREATE");
	yield_ALL->SetDirectory(gDirectory);
	yield_PASS->SetDirectory(gDirectory);
	EfficiencyFile->Write();
	//-----------------------------------------------------------------//
	
	//If all of the fits seem correct we can proceed to generate the efficiency
	get_efficiency(yield_ALL, yield_PASS, quantity, MuonId, DataIsMC);
	 
	//In case you want to change the fit on a specific, comment the loop and "result saving" code and uncomment the following function
	//change_bin(/*bin number you want to redo*/, /*condition (you can copy the title from the generated fit .pdf)*/, MuonId, quantity, DataIsMC, init_conditions);
	//bins start on 1
	
	//Once we've calculated the efficiency for both data sets, we can generate
	//a plot that combines both results
	//compare_efficiency(quantity, "Efficiency Result/" + quantity + "/Efficiency_MC.root", "Efficiency Result/" + quantity + "/Efficiency_Run2011.root");

	cout << "\n";
	cout << MuonId << " - " << quantity << "\n";
}
