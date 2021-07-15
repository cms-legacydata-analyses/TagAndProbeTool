#include "src/compare_efficiency.cpp"

//Change if you need
#include "src/dofits/DoFit_Upsilon.cpp"

#include "src/create_folder.cpp"
#include "src/get_efficiency.cpp"
#include "src/change_bin.cpp"
#include "src/make_hist.cpp"

//Which Muon Id do you want to study?
string MuonId   = "trackerMuon";
//string MuonId   = "standaloneMuon";
//string MuonId   = "globalMuon";

//Which quantity do you want to use?
//string quantity = "Pt";     double bins[] = {0., 2.0, 3.4, 4.0, 4.4, 4.7, 5.0, 5.6, 5.8, 6.0, 6.2, 6.4, 6.6, 6.8, 7.3, 9.5, 13.0, 17.0, 40.};
//string quantity = "Eta";    double bins[] = {-2.4, -1.8, -1.4, -1.2, -1.0, -0.8, -0.5, -0.2, 0, 0.2, 0.5, 0.8, 1.0, 1.2, 1.4, 1.8, 2.4};
//string quantity = "Phi";    double bins[] = {-3.0, -1.8, -1.6, -1.2, -1.0, -0.7, -0.4, -0.2, 0, 0.2, 0.4, 0.7, 1.0, 1.2, 1.6, 1.8, 3.0};
string quantity = "Pt";     double bins[] = {0.0, 2.0, 3.4, 4.0, 5.0, 6.0, 8.0, 10.0, 40.};
//string quantity = "Eta";    double bins[] = {0.0, 0.4, 0.6, 0.95, 1.2, 1.4, 1.6, 1.8, 2.1};

void efficiency()
{
	//Path where is going to save results png for every bin 
	const char* path_bins_fit_folder = "results/bins_fit/";
	create_folder(path_bins_fit_folder, true);

	// Loop for every bin and fit it
	int bin_n = sizeof(bins)/sizeof(*bins) - 1;
	double** yields_n_errs = new double*[bin_n];
	for (int i = 0; i < bin_n; i++)
	{
		//Creates conditions
		string conditions = string(    "ProbeMuon_" + quantity + ">" + to_string(bins[i]  ));
		conditions +=       string(" && ProbeMuon_" + quantity + "<" + to_string(bins[i+1]));

		//Stores [yield_all, yield_pass, err_all, err_pass]
		yields_n_errs[i] = doFit(conditions, MuonId, path_bins_fit_folder);
	}

	//Path where is going to save efficiency 
	string directoryToSave = string("results/efficiencies/") + output_folder_name + string("/");
	create_folder(directoryToSave.c_str());

	//Create file
	string file_path = directoryToSave + quantity + "_" + MuonId + ".root";
	TFile* generatedFile = new TFile(file_path.c_str(),"recreate");
	generatedFile->mkdir("histograms/");
	generatedFile->   cd("histograms/");
	
	TH1D *yield_all  = make_hist("ALL" , yields_n_errs, 0, bin_n, bins);
	TH1D *yield_pass = make_hist("PASS", yields_n_errs, 1, bin_n, bins);
	
	generatedFile->   cd("/");
	get_efficiency(yield_all, yield_pass, quantity, MuonId, "", true);
	 
	//In case you want to change the fit on a specific, comment the loop and "result saving" code and uncomment the following function
	//change_bin(/*bin number you want to redo*/, /*condition (you can copy the title from the generated fit .png)*/, MuonId, quantity, init_conditions);
	//bins start on 1

	cout << "\n[Settings]\n";
	cout << output_folder_name << " "<< MuonId << " " << quantity << "\n";
	cout << "Fitting:     " << fit_functions << "\n";
	cout << "Fit between: " << _mmin << " and " << _mmax << " GeV\n";
	cout << "Bins:        " << fit_bins << "\n";

	generatedFile->Write();

	cout << "\n------------------------\n";
	cout << "Output: " << file_path;
	cout << "\n------------------------\n";
}