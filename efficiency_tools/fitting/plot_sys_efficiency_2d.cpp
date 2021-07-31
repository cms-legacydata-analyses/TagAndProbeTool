//Change if you need
#include "src/dofits/DoFit_Jpsi.cpp"
#include "src/dofits/DoFit_Jpsi_2xGaus_for_systematic.cpp"

#include "src/create_folder.cpp"
#include "src/make_TH2D.cpp"
#include "src/get_efficiency_2D.cpp"

//Which Muon Id do you want to study?
string MuonId   = "trackerMuon";
//string MuonId   = "standaloneMuon";
//string MuonId   = "globalMuon";

// Bins to study
string yquantity = "Pt";
//double ybins[] = {0.0, 3.4, 4.0, 5.0, 6.0, 8.0, 40.};
double ybins[] = {0.0, 5.0, 40.};
string xquantity = "Eta";
//double xbins[] = {0.0, 0.4, 0.6, 0.95, 1.2, 1.4, 2.4};
double xbins[] = {0.0, 0.95, 2.4};

void plot_sys_efficiency_2d()
{
	//Path where is going to save results png for every bin 
	const char* path_bins_fit_folder = "results/bins_fit/systematic_2D/";
	create_folder(path_bins_fit_folder, true);

	// Loop for every bin and fit it
	const int nbinsy = sizeof(ybins)/sizeof(*ybins) - 1;
	const int nbinsx = sizeof(xbins)/sizeof(*xbins) - 1;

	//Creates variables to store values and error of each passed and total bin
	//Stores [yield_all, yield_pass, err_all, err_pass]
	double* yields_n_errs_Nominal  [nbinsx][nbinsy] = {0};
	double* yields_n_errs_2Gauss   [nbinsx][nbinsy] = {0};
	double* yields_n_errs_MassUp   [nbinsx][nbinsy] = {0};
	double* yields_n_errs_MassDown [nbinsx][nbinsy] = {0};
	double* yields_n_errs_BinUp    [nbinsx][nbinsy] = {0};
	double* yields_n_errs_BinDown  [nbinsx][nbinsy] = {0};


	/*
	double yields_final_pass[nbinsx][nbinsy];
	double yields_final_all [nbinsx][nbinsy];
	double errors_final_pass[nbinsx][nbinsy];
	double errors_final_all [nbinsx][nbinsy];
	*/

	double** yields_final_pass = new double*[nbinsx];
	double** yields_final_all  = new double*[nbinsx];
	double** errors_final_pass = new double*[nbinsx];
	double** errors_final_all  = new double*[nbinsx];

	for (int i = 0; i < nbinsx; i++)
	{
		yields_final_pass[i] = new double[nbinsy];
		yields_final_all [i] = new double[nbinsy];
		errors_final_pass[i] = new double[nbinsy];
		errors_final_all [i] = new double[nbinsy];

		for (int j = 0; j < nbinsy; j++)
		{
			//Creates conditions
			string conditions = string(    "ProbeMuon_" + yquantity + ">=" + to_string(ybins[j]  ));
			conditions +=       string(" && ProbeMuon_" + yquantity + "< " + to_string(ybins[j+1]));
			conditions +=       string(" && abs(ProbeMuon_" + xquantity + ")>=" + to_string(xbins[i]  ));
			conditions +=       string(" && abs(ProbeMuon_" + xquantity + ")< " + to_string(xbins[i+1]));

			const double default_min = _mmin;
			const double default_max = _mmax;
			string mmin_string;
			string mmax_string;

			//Nominal
			_mmin = default_min;
			_mmax = default_max;
			fit_bins = 100;
			prefix_file_name = "nominal_";
			yields_n_errs_Nominal[i][j] = doFit(conditions, MuonId, string(path_bins_fit_folder + prefix_file_name).c_str());

			//2Gauss
			_mmin = default_min;
			_mmax = default_max;
			fit_bins = 100;
			prefix_file_name = "2xgaus_";
			yields_n_errs_2Gauss[i][j] = doFit2xGaus(conditions, MuonId, string(path_bins_fit_folder + prefix_file_name).c_str());

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
			yields_n_errs_MassUp[i][j] = doFit(conditions, MuonId, string(path_bins_fit_folder + prefix_file_name).c_str());

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
			yields_n_errs_MassDown[i][j] = doFit(conditions, MuonId, string(path_bins_fit_folder + prefix_file_name).c_str());

			//BinUp
			_mmin = default_min;
			_mmax = default_max;
			fit_bins = 105;
			prefix_file_name = "binfit105_";
			yields_n_errs_BinUp[i][j] = doFit(conditions, MuonId, string(path_bins_fit_folder + prefix_file_name).c_str());

			//BinDown
			_mmin = default_min;
			_mmax = default_max;
			fit_bins = 95;
			prefix_file_name = "binfit95_";
			yields_n_errs_BinDown[i][j] = doFit(conditions, MuonId, string(path_bins_fit_folder + prefix_file_name).c_str());

			//Calculates the result
			yields_final_all [i][j] = yields_n_errs_Nominal[i][j][0];
			yields_final_pass[i][j] = yields_n_errs_Nominal[i][j][1];
			errors_final_all [i][j] = sqrt(pow(yields_n_errs_Nominal[i][j][2],2) + pow(yields_n_errs_2Gauss[i][j][2],2) + pow(yields_n_errs_MassUp[i][j][2],2) + pow(yields_n_errs_MassUp[i][j][2],2) + pow(yields_n_errs_BinUp[i][j][2],2) + pow(yields_n_errs_BinDown[i][j][2],2));
			errors_final_pass[i][j] = sqrt(pow(yields_n_errs_Nominal[i][j][3],2) + pow(yields_n_errs_2Gauss[i][j][3],2) + pow(yields_n_errs_MassUp[i][j][3],2) + pow(yields_n_errs_MassUp[i][j][3],2) + pow(yields_n_errs_BinUp[i][j][3],2) + pow(yields_n_errs_BinDown[i][j][3],2));

		}
	}

	//Path where is going to save efficiency
	string directoryToSave = string("results/efficiencies/systematic_2D/") + output_folder_name + string("/");
	create_folder(directoryToSave.c_str());

	//Create file
	string file_path = directoryToSave + xquantity + "_" + yquantity + "_" + MuonId + ".root";
	TFile* generatedFile = new TFile(file_path.c_str(),"recreate");
	generatedFile->mkdir("histograms/");
	generatedFile->   cd("histograms/");
	
	TH2D* hist_all  = make_TH2D("all_systematic",  "All Systematic",  xquantity, yquantity, nbinsx, nbinsy, xbins, ybins, yields_final_all,  errors_final_all);
	TH2D* hist_pass = make_TH2D("pass_systematic", "Pass Systematic", xquantity, yquantity, nbinsx, nbinsy, xbins, ybins, yields_final_pass, errors_final_pass);

	generatedFile->   cd("/");
	get_efficiency_2D(hist_all, hist_pass, xquantity, yquantity, MuonId, "", true);

	generatedFile->Write();

	cout << "\n------------------------\n";
	cout << "Output: " << file_path;
	cout << "\n------------------------\n";
}