

void change_bin(int bin_iD, string condition, string MuonId, string quantity, string hist_file = "Histograms.root")
{
	TFile *_file0 = TFile::Open(hist_file.c_str());
	
	TH1D *hist_all  = (TH1D*)_file0->Get("ALL");
	TH1D *hist_pass = (TH1D*)_file0->Get("PASS");
	
	double * values = new double[4];
	
	values = doFit(condition, quantity);
	
	hist_all->SetBinContent(bin_iD, values[0]);
	hist_all->SetBinError(bin_iD, values[2]);
	
	hist_pass->SetBinContent(bin_iD, values[1]);
	hist_pass->SetBinError(bin_iD, values[3]);
	
	TFile* EfficiencyFile = TFile::Open(hist_file.c_str(),"RECREATE");
	hist_all->SetDirectory(gDirectory);
	hist_pass->SetDirectory(gDirectory);
	EfficiencyFile->Write();
	
	get_efficiency(hist_all, hist_pass, quantity, MuonId);
}
