TH1F* make_hist(string name, double** values, int qnt, int bin_n, Double_t* binning, bool draw = false)
{
	//AddBinContent
	//HISTOGRAM NEEDS TO HAVE VARIABLE BINS
   
	TH1F* hist = new TH1F(name.c_str(), name.c_str(), bin_n, binning);

	for (int i = 0; i < bin_n; i++)
	{
		hist->SetBinContent(i, values[i][qnt]);
		hist->SetBinError(i, values[i][qnt+2]);
	}

	if (draw)
	{
		TCanvas* xperiment = new TCanvas;
		xperiment->cd();
		hist->Draw();
	}
	
	return hist;
}

