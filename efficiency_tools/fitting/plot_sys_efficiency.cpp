//Change if you need
#include "src/dofits/DoFit_Jpsi.cpp"
#include "src/dofits/DoFit_Jpsi_2xGaus_for_systematic.cpp"

#include "src/create_folder.cpp"
#include "src/get_efficiency.cpp"
#include "src/make_TH1D.cpp"

//Which Muon Id do you want to study?
string MuonId   = "trackerMuon";
//string MuonId   = "standaloneMuon";
//string MuonId   = "globalMuon";

//Which quantity do you want to use?
string quantity = "Pt";     double bins[] = {0., 3.0, 3.6, 4.0, 4.4, 4.7, 5.0, 5.6, 5.8, 6.0, 6.2, 6.4, 6.6, 6.8, 7.3, 9.5, 13.0, 17.0, 40.};
//string quantity = "Eta";    double bins[] = {-2.4, -1.4, -1.2, -1.0, -0.8, -0.5, -0.2, 0, 0.2, 0.5, 0.8, 1.0, 1.2, 1.4, 2.4};
//string quantity = "Phi";    double bins[] = {-3.0, -1.8, -1.6, -1.2, -1.0, -0.7, -0.4, -0.2, 0, 0.2, 0.4, 0.7, 1.0, 1.2, 1.6, 1.8, 3.0};

//string quantity = "Pt";     double bins[] = {0.0, 2.0, 3.4, 4.0, 5.0, 6.0, 8.0, 10.0, 40.};
//string quantity = "Eta";    double bins[] = {0.0, 0.4, 0.6, 0.95, 1.2, 1.4, 1.6, 1.8, 2.4};

void plot_sys_efficiency()
{
	//First enable implicit multi-threading globally, so that the implicit parallelisation is on.
	//The parameter of the call specifies the number of threads to use.
	//int nthreads = 4;
	//ROOT::EnableImplicitMT(nthreads);

	//Path where is going to save results png for every bin 
	string path_bins_fit_folder = string("results/bins_fit/systematic_1D/") + output_folder_name + string("/") + quantity + string("/") + MuonId + string("/");
	create_folder(path_bins_fit_folder.c_str(), true);

	// Loop for every bin and fit it
	int nbins = sizeof(bins)/sizeof(*bins) - 1;

	//Creates variables to store values and error of each passed and total bin
	//Stores [yield_all, yield_pass, err_all, err_pass]
	double** yields_n_errs_Nominal  = new double*[nbins];
	double** yields_n_errs_2Gauss   = new double*[nbins];
	double** yields_n_errs_MassUp   = new double*[nbins];
	double** yields_n_errs_MassDown = new double*[nbins];
	double** yields_n_errs_BinUp    = new double*[nbins];
	double** yields_n_errs_BinDown  = new double*[nbins];
	double** yields_n_errs          = new double*[nbins];

	for (int i = 0; i < nbins; i++)
	{
		//Creates conditions
		string conditions = string(    "ProbeMuon_" + quantity + ">=" + to_string(bins[i]  ));
		conditions +=       string(" && ProbeMuon_" + quantity + "< " + to_string(bins[i+1]));

		double default_min = _mmin;
		double default_max = _mmax;
		string mmin_string = "";
		string mmax_string = "";

		//Nominal
		_mmin = default_min;
		_mmax = default_max;
		fit_bins = 100;
		prefix_file_name = "nominal_";
		yields_n_errs_Nominal[i] = doFit(conditions, MuonId, (path_bins_fit_folder + prefix_file_name).c_str());

		//2Gauss
		_mmin = default_min;
		_mmax = default_max;
		fit_bins = 100;
		prefix_file_name = "2xgaus_";
		yields_n_errs_2Gauss[i] = doFit2xGaus(conditions, MuonId, (path_bins_fit_folder + prefix_file_name).c_str());

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
		yields_n_errs_MassUp[i] = doFit(conditions, MuonId, (path_bins_fit_folder + prefix_file_name).c_str());

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
		yields_n_errs_MassDown[i] = doFit(conditions, MuonId, (path_bins_fit_folder + prefix_file_name).c_str());

		//BinUp
		_mmin = default_min;
		_mmax = default_max;
		fit_bins = 105;
		prefix_file_name = "binfit105_";
		yields_n_errs_BinUp[i] = doFit(conditions, MuonId, (path_bins_fit_folder + prefix_file_name).c_str());

		//BinDown
		_mmin = default_min;
		_mmax = default_max;
		fit_bins = 95;
		prefix_file_name = "binfit95_";
		yields_n_errs_BinDown[i] = doFit(conditions, MuonId, (path_bins_fit_folder + prefix_file_name).c_str());

		//Calculates the result
		double* result = new double[4];
		result[0] = yields_n_errs_Nominal[i][0];
		result[1] = yields_n_errs_Nominal[i][1];
		result[2] = sqrt(pow(yields_n_errs_Nominal[i][2],2) + pow(yields_n_errs_2Gauss[i][2],2) + pow(yields_n_errs_MassUp[i][2],2) + pow(yields_n_errs_MassUp[i][2],2) + pow(yields_n_errs_BinUp[i][2],2) + pow(yields_n_errs_BinDown[i][2],2));
		result[3] = sqrt(pow(yields_n_errs_Nominal[i][3],2) + pow(yields_n_errs_2Gauss[i][3],2) + pow(yields_n_errs_MassUp[i][3],2) + pow(yields_n_errs_MassUp[i][3],2) + pow(yields_n_errs_BinUp[i][3],2) + pow(yields_n_errs_BinDown[i][3],2));
		yields_n_errs[i] = result;
	}

	//Path where is going to save efficiency 
	string directoryToSave = string("results/efficiencies/systematic_1D/") + output_folder_name + string("/");
	create_folder(directoryToSave.c_str());

	//Create file
	string file_path = directoryToSave + quantity + "_" + MuonId + ".root";
	TFile* generatedFile = new TFile(file_path.c_str(),"recreate");
	generatedFile->mkdir("histograms/");
	generatedFile->   cd("histograms/");
	
	TH1D *hist_all           = make_TH1D("all"          , yields_n_errs         , 0, bins, nbins, quantity);
	TH1D *hist_nominal_all   = make_TH1D("all_nominal"  , yields_n_errs_Nominal , 0, bins, nbins, quantity);
	TH1D *hist_2gaus_all     = make_TH1D("all_2xGauss"  , yields_n_errs_2Gauss  , 0, bins, nbins, quantity);
	TH1D *hist_massup_all    = make_TH1D("all_MassUp"   , yields_n_errs_MassUp  , 0, bins, nbins, quantity);
	TH1D *hist_massdown_all  = make_TH1D("all_MassDown" , yields_n_errs_MassDown, 0, bins, nbins, quantity);
	TH1D *hist_binup_all     = make_TH1D("all_BinUp"    , yields_n_errs_BinUp   , 0, bins, nbins, quantity);
	TH1D *hist_bindown_all   = make_TH1D("all_BinDown"  , yields_n_errs_BinDown , 0, bins, nbins, quantity);

	TH1D *hist_pass          = make_TH1D("pass"         , yields_n_errs         , 1, bins, nbins, quantity);
	TH1D *hist_2gaus_pass    = make_TH1D("pass_2xGauss" , yields_n_errs_Nominal , 1, bins, nbins, quantity);
	TH1D *hist_nominal_pass  = make_TH1D("pass_nominal" , yields_n_errs_2Gauss  , 1, bins, nbins, quantity);
	TH1D *hist_massup_pass   = make_TH1D("pass_MassUp"  , yields_n_errs_MassUp  , 1, bins, nbins, quantity);
	TH1D *hist_massdown_pass = make_TH1D("pass_MassDown", yields_n_errs_MassDown, 1, bins, nbins, quantity);
	TH1D *hist_binup_pass    = make_TH1D("pass_BinDown" , yields_n_errs_BinUp   , 1, bins, nbins, quantity);
	TH1D *hist_bindown_pass  = make_TH1D("pass_BinDown" , yields_n_errs_BinDown , 1, bins, nbins, quantity);

	generatedFile->   cd("/");
	get_efficiency(hist_all         , hist_pass         , quantity, MuonId, ""        , true);
	get_efficiency(hist_nominal_all , hist_2gaus_pass   , quantity, MuonId, "Nominal" , true);
	get_efficiency(hist_2gaus_all   , hist_nominal_pass , quantity, MuonId, "2xGauss" , true);
	get_efficiency(hist_massup_all  , hist_massup_pass  , quantity, MuonId, "MassUp"  , true);
	get_efficiency(hist_massdown_all, hist_massdown_pass, quantity, MuonId, "MassDown", true);
	get_efficiency(hist_binup_all   , hist_binup_pass   , quantity, MuonId, "BinUp"   , true);
	get_efficiency(hist_bindown_all , hist_bindown_pass , quantity, MuonId, "BinDown" , true);

	generatedFile->Write();

	cout << "\n------------------------\n";
	cout << "Output: " << file_path;
	cout << "\n------------------------\n";
}