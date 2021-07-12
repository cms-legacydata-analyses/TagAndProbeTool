#include "src/compare_efficiency.C"

//Change if you need
#include "src/dofits/DoFit_Jpsi.cpp"
//#include "src/dofits/DoFit_Jpsi_2xGaus.cpp"
double default_min = _mmin;
double default_max = _mmax;

#include "src/get_conditions.cpp"
#include "src/create_folder.cpp"
#include "src/get_efficiency.cpp"
#include "src/change_bin.cpp"
#include "src/make_hist.cpp"

//Which Muon Id do you want to study?
//string MuonId   = "trackerMuon";
//string MuonId   = "standaloneMuon";
string MuonId   = "globalMuon";
bool should_loop_muon_id  = false;
bool should_loop_settings = false;

//Which quantity do you want to use?
string quantity = "Pt";     double bins[] = {0., 2.0, 3.4, 4.0, 4.4, 4.7, 5.0, 5.6, 5.8, 6.0, 6.2, 6.4, 6.6, 6.8, 7.3, 9.5, 13.0, 17.0, 40.};
//string quantity = "Eta";    double bins[] = {-2.4, -1.8, -1.4, -1.2, -1.0, -0.8, -0.5, -0.2, 0, 0.2, 0.5, 0.8, 1.0, 1.2, 1.4, 1.8, 2.4};
//string quantity = "Phi";    double bins[] = {-3.0, -1.8, -1.6, -1.2, -1.0, -0.7, -0.4, -0.2, 0, 0.2, 0.4, 0.7, 1.0, 1.2, 1.6, 1.8, 3.0};

void efficiency_table()
{
	//_mmin = 2.75;
	//_mmax = 3.35;
	//fit_bins = 95;
	//prefix_file_name = "mass_2p75_3p35_";
	//prefix_file_name = "2xgaus_";

	//Path where is going to save results png for every bin 
	const char* path_bins_fit_folder = "results/bins_fit/";
	create_folder(path_bins_fit_folder, true);
	
	//Creaete condition for every bin
	int bin_n = sizeof(bins)/sizeof(*bins) - 1;
	string* conditions = get_conditions(bin_n, bins, "ProbeMuon_" + quantity);
	double ** yields_n_errs = new double*[bin_n];
	
	// Loop for every bin and fit it
	for (int i = 0; i < bin_n; i++)
	{
		yields_n_errs[i] = doFit(conditions[i], MuonId, quantity, path_bins_fit_folder);
		//doFit returns: [yield_all, yield_pass, err_all, err_pass]
	}
	
	TH1F *yield_ALL  = make_hist("ALL" , yields_n_errs, 0, bin_n, bins);
	TH1F *yield_PASS = make_hist("PASS", yields_n_errs, 1, bin_n, bins);
	
	//----------------------SAVING RESULTS TO Histograms.root--------------------//
	//useful if we require to change the fit on a specific set of bins
	TFile* EfficiencyFile = TFile::Open((string(path_bins_fit_folder) + "histograms.root").c_str(),"RECREATE");
	yield_ALL->SetDirectory(gDirectory);
	yield_PASS->SetDirectory(gDirectory);
	EfficiencyFile->Write();
	//-----------------------------------------------------------------//
	
	//If all of the fits seem correct we can proceed to generate the efficiency
	get_efficiency(yield_ALL, yield_PASS, quantity, MuonId, prefix_file_name);
	 
	//In case you want to change the fit on a specific, comment the loop and "result saving" code and uncomment the following function
	//change_bin(/*bin number you want to redo*/, /*condition (you can copy the title from the generated fit .png)*/, MuonId, quantity, init_conditions);
	//bins start on 1
	
	//Once we've calculated the efficiency for both data sets, we can generate
	//a plot that combines both results
	//compare_efficiency(quantity, "Efficiency Result/" + quantity + "/Efficiency_MC.root", "Efficiency Result/" + quantity + "/Efficiency_Run2011.root");

	cout << "\n[Settings]\n";
	cout << output_folder_name << " "<< MuonId << " " << quantity << "\n";
	cout << "Fitting:     " << fit_functions << "\n";
	cout << "Fit between: " << _mmin << " and " << _mmax << " GeV\n";
	cout << "Bins:        " << fit_bins << "\n";
}