TH1D* make_TH1D(string name, double** values, int index, double* bins, int nbins, string quantity = "", bool draw = false)
{
	//AddBinContent
	//HISTOGRAM NEEDS TO HAVE VARIABLE BINS
   
	TH1D* hist = new TH1D(name.c_str(), name.c_str(), nbins, bins);

	hist->GetYaxis()->SetTitle("Events");
	if      (quantity == "Pt" ) hist->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	else if (quantity == "Eta") hist->GetXaxis()->SetTitle("#eta");
	else if (quantity == "Phi") hist->GetXaxis()->SetTitle("rad");

	for (int i = 0; i < nbins; i++)
	{
		hist->SetBinContent(i+1, values[i][index]);
		hist->SetBinError(i+1, values[i][index+2]);
		//cout << i << " -> (" << hist->GetBinLowEdge(i+1) << "," << hist->GetBinLowEdge(i+1)+hist->GetBinWidth(i+1) << ") == " << hist->GetBinContent(i+1) << "\n";
	}

	if (draw)
	{
		TCanvas* xperiment = new TCanvas;
		xperiment->cd();
		hist->Draw();
	}
	
	return hist;
}