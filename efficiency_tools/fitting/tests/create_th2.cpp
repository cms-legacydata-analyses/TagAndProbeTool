
void create_th2()
{
	double xbins[] = {0., 2., 4., 8., 14.};
	double ybins[] = {0., 2., 4., 12., 16.};
	TH2D* hpass = new TH2D("name", "title", 4, xbins, 4, ybins);
	cout << "created" << endl;
	cout << hpass->GetXaxis()->GetXbins()->At(1) << endl;

	double fill[4][4] = {
		{1., 2., 3., 4.},
		{5., 6., 7., 8.},
		{1., 2., 3., 4.},
		{5., 6., 7., 8.},
	};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			hpass->SetBinContent(i,j,fill[i][j]);
			//hpass->SetBinContent(i+1,j+1,fill[i][j]);
			//cout << i << "," << j << " -> " << hpass->GetBinContent(i,j) << endl;
		}
	}

	hpass->Draw("colz");
	gStyle->SetOptStat(0);
}