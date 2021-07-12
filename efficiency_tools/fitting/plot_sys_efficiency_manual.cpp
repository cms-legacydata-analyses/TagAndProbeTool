#include "src/compare_efficiency.C"

//Change if you need
#include "src/dofits/DoFit_Jpsi.cpp"
#include "src/dofits/DoFit_Jpsi_2xGaus_newsys.cpp"

#include "src/create_folder.cpp"
#include "src/get_efficiency.cpp"
#include "src/change_bin.cpp"
#include "src/make_hist.cpp"

//Which Muon Id do you want to study?
string MuonId   = "trackerMuon";
//string MuonId   = "standaloneMuon";
//string MuonId   = "globalMuon";

//bool should_loop_muon_id  = false;

//Which quantity do you want to use?
string quantity = "Pt";     double bins[] = {0., 2.0, 3.4, 4.0, 4.4, 4.7, 5.0, 5.6, 5.8, 6.0, 6.2, 6.4, 6.6, 6.8, 7.3, 9.5, 13.0, 17.0, 40.};
//string quantity = "Eta";    double bins[] = {-2.4, -1.8, -1.4, -1.2, -1.0, -0.8, -0.5, -0.2, 0, 0.2, 0.5, 0.8, 1.0, 1.2, 1.4, 1.8, 2.4};
//string quantity = "Phi";    double bins[] = {-3.0, -1.8, -1.6, -1.2, -1.0, -0.7, -0.4, -0.2, 0, 0.2, 0.4, 0.7, 1.0, 1.2, 1.6, 1.8, 3.0};

void plot_sys_efficiency_manual()
{
	//Path where is going to save results png for every bin 
	const char* path_bins_fit_folder = "results/bins_fit/";
	create_folder(path_bins_fit_folder, true);

	// Loop for every bin and fit it
	int bin_n = sizeof(bins)/sizeof(*bins) - 1;

	//Creates variables to store values and error of each passed and total bin
	//Stores [yield_all, yield_pass, err_all, err_pass]
	double** yields_n_errs_Nominal  = new double*[bin_n];
	double** yields_n_errs_2Gauss   = new double*[bin_n];
	double** yields_n_errs_MassUp   = new double*[bin_n];
	double** yields_n_errs_MassDown = new double*[bin_n];
	double** yields_n_errs_BinUp    = new double*[bin_n];
	double** yields_n_errs_BinDown  = new double*[bin_n];
	double** yields_n_errs = new double*[bin_n];

	for (int i = 0; i < bin_n; i++)
	{
		//Creates conditions
		string conditions = string(    "ProbeMuon_" + quantity + ">" + to_string(bins[i]  ));
		conditions +=       string(" && ProbeMuon_" + quantity + "<" + to_string(bins[i+1]));

		double default_min = _mmin;
		double default_max = _mmax;
		string mmin_string = "";
		string mmax_string = "";

		//Nominal
		_mmin = default_min;
		_mmax = default_max;
		fit_bins = 100;
		prefix_file_name = "nominal_";
		yields_n_errs_Nominal[i] = doFit(conditions, MuonId, quantity, string(path_bins_fit_folder + prefix_file_name).c_str());

		//2Gauss
		_mmin = default_min;
		_mmax = default_max;
		fit_bins = 100;
		prefix_file_name = "2xgaus_";
		yields_n_errs_2Gauss[i] = doFit2xGaus(conditions, MuonId, quantity, string(path_bins_fit_folder + prefix_file_name).c_str());

		//MassUp
		_mmin = default_min - 0.05;
		_mmax = default_max + 0.05;
		fit_bins = 100;
		mmin_string = to_string(_mmin);
		mmax_string = to_string(_mmax);
		replace(mmin_string.begin(), mmin_string.end(), '.', 'p');
		replace(mmax_string.begin(), mmax_string.end(), '.', 'p');
		prefix_file_name  = string("mass_") + mmin_string.substr(0, mmin_string.length()-4) + string("_");
		prefix_file_name +=                   mmax_string.substr(0, mmax_string.length()-4) + string("_");
		yields_n_errs_MassUp[i] = doFit(conditions, MuonId, quantity, string(path_bins_fit_folder + prefix_file_name).c_str());

		//MassDown
		_mmin = default_min + 0.05;
		_mmax = default_max - 0.05;
		fit_bins = 100;
		mmin_string = to_string(_mmin);
		mmax_string = to_string(_mmax);
		replace(mmin_string.begin(), mmin_string.end(), '.', 'p');
		replace(mmax_string.begin(), mmax_string.end(), '.', 'p');
		prefix_file_name  = string("mass_") + mmin_string.substr(0, mmin_string.length()-4) + string("_");
		prefix_file_name +=                   mmax_string.substr(0, mmax_string.length()-4) + string("_");
		yields_n_errs_MassDown[i] = doFit(conditions, MuonId, quantity, string(path_bins_fit_folder + prefix_file_name).c_str());

		//BinUp
		_mmin = default_min;
		_mmax = default_max;
		fit_bins = 105;
		prefix_file_name = "binfit105_";
		yields_n_errs_BinUp[i] = doFit(conditions, MuonId, quantity, string(path_bins_fit_folder + prefix_file_name).c_str());

		//BinDown
		_mmin = default_min;
		_mmax = default_max;
		fit_bins = 95;
		prefix_file_name = "binfit95_";
		yields_n_errs_BinDown[i] = doFit(conditions, MuonId, quantity, string(path_bins_fit_folder + prefix_file_name).c_str());


		//Calculates the result
		yields_n_errs[i][0] = yields_n_errs_Nominal[i][0];
		yields_n_errs[i][1] = yields_n_errs_Nominal[i][1];
		yields_n_errs[i][2] = sqrt(yields_n_errs_Nominal[i][2] + yields_n_errs_2Gauss[i][2] + yields_n_errs_MassUp[i][2] + yields_n_errs_MassUp[i][2] + yields_n_errs_BinUp[i][2] + yields_n_errs_BinDown[i][2]);
		yields_n_errs[i][3] = sqrt(yields_n_errs_Nominal[i][3] + yields_n_errs_2Gauss[i][3] + yields_n_errs_MassUp[i][3] + yields_n_errs_MassUp[i][3] + yields_n_errs_BinUp[i][3] + yields_n_errs_BinDown[i][3]);
	}
	
	TH1F *yield_ALL  = make_hist("all" , yields_n_errs, 0, bin_n, bins);
	TH1F *yield_PASS = make_hist("pass", yields_n_errs, 1, bin_n, bins);
	
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
}

/*
void loop_muon_id()
{
	for (int i = 0; i <= 2; i++)
	{
		switch(i)
		{
			case 0:
				MuonId   = "trackerMuon";
				break;
			case 1:
				MuonId   = "standaloneMuon";
				break;
			case 2:
				MuonId   = "globalMuon";
				break;
		}		
		
		plot_sys_efficiency_manual();
	}
}
*/