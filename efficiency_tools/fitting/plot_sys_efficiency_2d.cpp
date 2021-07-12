//Change if you need
#include "src/dofits/DoFit_Jpsi_newsys.cpp"
#include "src/dofits/DoFit_Jpsi_2xGaus_newsys.cpp"

#include "src/create_folder.cpp"
#include "src/get_efficiency.cpp"
#include "src/make_hist.cpp"

//Which Muon Id do you want to study?
string MuonId   = "trackerMuon";
//string MuonId   = "standaloneMuon";
//string MuonId   = "globalMuon";

//bool should_loop_muon_id  = false;

//Which quantity do you want to use?
string quantity1 = "Pt";
//double bins1[] = {0., 2.0, 3.4, 4.0, 4.4, 4.7, 5.0, 5.6, 5.8, 6.0, 6.2, 6.4, 6.6, 6.8, 7.3, 9.5, 13.0, 17.0, 40.};
double bins1[] = {0.0, 2.0, 3.4, 4.0, 5.0, 6.0, 8.0, 10.0, 40.};
string quantity2 = "Eta";
double bins2[] = {0.0, 0.4, 0.6, 0.95, 1.2, 1.4, 1.6, 1.8, 2.1};
//double bins2[] = {0.0, 2., 3.};
//double bins2[] = {-2.4, -1.8, -1.4, -1.2, -1.0, -0.8, -0.5, -0.2, 0, 0.2, 0.5, 0.8, 1.0, 1.2, 1.4, 1.8, 2.4};
//string quantity = "Phi";    double bins[] = {-3.0, -1.8, -1.6, -1.2, -1.0, -0.7, -0.4, -0.2, 0, 0.2, 0.4, 0.7, 1.0, 1.2, 1.6, 1.8, 3.0};

void plot_sys_efficiency_2d()
{
	//Path where is going to save results png for every bin 
	const char* path_bins_fit_folder = "results/bins_fit/";
	create_folder(path_bins_fit_folder, true);

	// Loop for every bin and fit it
	const int bin_n1 = sizeof(bins1)/sizeof(*bins1) - 1;
	const int bin_n2 = sizeof(bins2)/sizeof(*bins2) - 1;

	//Creates variables to store values and error of each passed and total bin
	//Stores [yield_all, yield_pass, err_all, err_pass]
	double* yields_n_errs_Nominal  [bin_n2][bin_n1] = {0};
	double* yields_n_errs_2Gauss   [bin_n2][bin_n1] = {0};
	double* yields_n_errs_MassUp   [bin_n2][bin_n1] = {0};
	double* yields_n_errs_MassDown [bin_n2][bin_n1] = {0};
	double* yields_n_errs_BinUp    [bin_n2][bin_n1] = {0};
	double* yields_n_errs_BinDown  [bin_n2][bin_n1] = {0};
	double* yields_n_errs          [bin_n2][bin_n1] = {0};

	cout << "Bins1: " << bin_n1 << "\n";
	cout << "Bins2: " << bin_n2 << "\n";

	for (int j = 0; j < bin_n2; j++)
	{
		for (int i = 0; i < bin_n1; i++)
		{
			cout << "Starting " << j << "," << i << "\n";
			//Creates conditions
			string conditions = string(    "ProbeMuon_" + quantity1 + ">" + to_string(bins1[i]  ));
			conditions +=       string(" && ProbeMuon_" + quantity1 + "<" + to_string(bins1[i+1]));
			conditions +=       string(" && abs(ProbeMuon_" + quantity2 + ")>" + to_string(bins2[j]  ));
			conditions +=       string(" && abs(ProbeMuon_" + quantity2 + ")<" + to_string(bins2[j+1]));

			const double default_min = _mmin;
			const double default_max = _mmax;
			string mmin_string;
			string mmax_string;

			//Nominal
			_mmin = default_min;
			_mmax = default_max;
			fit_bins = 100;
			prefix_file_name = "nominal_";
			yields_n_errs_Nominal[j][i] = doFit(conditions, MuonId, quantity1, quantity2, string(path_bins_fit_folder + prefix_file_name).c_str());

			//2Gauss
			_mmin = default_min;
			_mmax = default_max;
			fit_bins = 100;
			prefix_file_name = "2xgaus_";
			yields_n_errs_2Gauss[j][i] = doFit2xGaus(conditions, MuonId, quantity1, quantity2, string(path_bins_fit_folder + prefix_file_name).c_str());
			//yields_n_errs_2Gauss[j][i] = doFit(conditions, MuonId, quantity1, quantity2, string(path_bins_fit_folder + prefix_file_name).c_str());

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
			yields_n_errs_MassUp[j][i] = doFit(conditions, MuonId, quantity1, quantity2, string(path_bins_fit_folder + prefix_file_name).c_str());

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
			yields_n_errs_MassDown[j][i] = doFit(conditions, MuonId, quantity1, quantity2, string(path_bins_fit_folder + prefix_file_name).c_str());

			//BinUp
			_mmin = default_min;
			_mmax = default_max;
			fit_bins = 105;
			prefix_file_name = "binfit105_";
			yields_n_errs_BinUp[j][i] = doFit(conditions, MuonId, quantity1, quantity2, string(path_bins_fit_folder + prefix_file_name).c_str());

			//BinDown
			_mmin = default_min;
			_mmax = default_max;
			fit_bins = 95;
			prefix_file_name = "binfit95_";
			yields_n_errs_BinDown[j][i] = doFit(conditions, MuonId, quantity1, quantity2, string(path_bins_fit_folder + prefix_file_name).c_str());


			//Calculates the result
			yields_n_errs[j][i][0] = yields_n_errs_Nominal[j][i][0];
			yields_n_errs[j][i][1] = yields_n_errs_Nominal[j][i][1];
			yields_n_errs[j][i][2] = sqrt(pow(yields_n_errs_Nominal[j][i][2],2) + pow(yields_n_errs_2Gauss[j][i][2],2) + pow(yields_n_errs_MassUp[j][i][2],2) + pow(yields_n_errs_MassUp[j][i][2],2) + pow(yields_n_errs_BinUp[j][i][2],2) + pow(yields_n_errs_BinDown[j][i][2],2));
			yields_n_errs[j][i][3] = sqrt(pow(yields_n_errs_Nominal[j][i][3],2) + pow(yields_n_errs_2Gauss[j][i][3],2) + pow(yields_n_errs_MassUp[j][i][3],2) + pow(yields_n_errs_MassUp[j][i][3],2) + pow(yields_n_errs_BinUp[j][i][3],2) + pow(yields_n_errs_BinDown[j][i][3],2));
		}
	}

	cout << "{\n";
	//Print
	for (int j = 0; j < bin_n2; j++)
	{
		string conditions2 = string(" && abs(ProbeMuon_" + quantity2 + ")>" + to_string(bins2[j]  ));
		conditions2 +=       string(" && abs(ProbeMuon_" + quantity2 + ")<" + to_string(bins2[j+1]));
		cout << "\t\"" << conditions2 << "\": {\n";
		for (int i = 0; i < bin_n1; i++)
		{
			string conditions1 = string(    "ProbeMuon_" + quantity1 + ">" + to_string(bins1[i]  ));
			conditions1 +=       string(" && ProbeMuon_" + quantity1 + "<" + to_string(bins1[i+1]));
			cout << "\t\t{\"" << conditions1 << "\": [" << yields_n_errs[j][i][0] << ",";
			cout << yields_n_errs[j][i][1] << ",";
			cout << yields_n_errs[j][i][2] << ",";
			cout << yields_n_errs[j][i][3] << "]},\n";
		}
		cout << "\t},\n";
	}
}