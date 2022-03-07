
TH2D* create_TH2D(const char* name, const char* title, string xquantity, string yquantity, int nbinsx, int nbinsy,
	double* xbins, double* ybins)
{
	TH2D* hist2d = new TH2D(name, title, nbinsx, xbins, nbinsy, ybins);

	//Set x axis title for plot
	if      (xquantity == "Pt" ) hist2d->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	else if (xquantity == "Eta") hist2d->GetXaxis()->SetTitle("|#eta|");
	else if (xquantity == "Phi") hist2d->GetXaxis()->SetTitle("rad");

	//Set y axis title for plot
	if      (yquantity == "Pt" ) hist2d->GetYaxis()->SetTitle("p_{T} [GeV/c]");
	else if (yquantity == "Eta") hist2d->GetYaxis()->SetTitle("|#eta|");
	else if (yquantity == "Phi") hist2d->GetYaxis()->SetTitle("rad");
	
	return hist2d;
}