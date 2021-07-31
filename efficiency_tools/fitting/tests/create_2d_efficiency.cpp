#include "../src/get_efficiency_2D.cpp"

TH2D* create_th2_all()
{
	double xbins[] = {0., 2., 4., 8., 14.};
	double ybins[] = {0., 2., 4., 12., 16.};
	TH2D* hpass = new TH2D("name", "title", 4, xbins, 4, ybins);
	cout << "created" << endl;
	//cout << hpass->GetXaxis()->GetXbins()->At(1) << endl;

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
			hpass->SetBinContent(i+1,j+1,fill[i][j]);
			//hpass->SetBinContent(i+1,j+1,fill[i][j]);
			//cout << i << "," << j << " -> " << hpass->GetBinContent(i+1,j+1) << endl;
		}
	}

	//hpass->Draw("colz");
	//gStyle->SetOptStat(0);

	return hpass;
}

TH2D* create_th2_pass()
{
	double xbins[] = {0., 2., 4., 8., 14.};
	double ybins[] = {0., 2., 4., 12., 16.};
	TH2D* hpass = new TH2D("name", "title", 4, xbins, 4, ybins);
	cout << "created" << endl;
	//cout << hpass->GetXaxis()->GetXbins()->At(1) << endl;

	double fill[4][4] = {
		{0.5, 1., 2., 3.},
		{2., 3., 6., 7.},
		{0.2, 1., 2., 3.},
		{4., 3., 6., 5.},
	};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			hpass->SetBinContent(i+1,j+1,fill[i][j]);
			//hpass->SetBinContent(i+1,j+1,fill[i][j]);
			//cout << i << "," << j << " -> " << hpass->GetBinContent(i+1,j+1) << endl;
		}
	}

	//hpass->Draw("colz");
	//gStyle->SetOptStat(0);

	return hpass;
}

void invented_data()
{
	TH2D* hpass = create_th2_pass();
	TH2D* hall  = create_th2_all();

	TEfficiency* pEff = new TEfficiency();
	pEff->SetPassedHistogram(*hpass, "f");
	pEff->SetTotalHistogram (*hall, "f");

	pEff->SetStatisticOption(TEfficiency::kBBayesian);
	pEff->SetConfidenceLevel(0.68);

	pEff->Draw("colz");
}

void real_data()
{
	TFile *file0 = TFile::Open("../results/efficiencies/systematic_2D/Jpsi_Run_2011/Eta_Pt_trackerMuon.root");

	TH2D *hpass = (TH2D*)file0->Get("histograms/all_systematic");
	TH2D *hall  = (TH2D*)file0->Get("histograms/pass_systematic");

	TEfficiency* pEff = new TEfficiency();
	pEff->SetPassedHistogram(*hpass, "f");
	pEff->SetTotalHistogram (*hall,"f");

	pEff->Draw("colz");
}

void real_data_using_get_efficiency_2d()
{
	TFile *file0 = TFile::Open("../results/efficiencies/systematic_2D/Jpsi_Run_2011/Eta_Pt_trackerMuon.root");

	TH2D *hpass = (TH2D*)file0->Get("histograms/all_systematic");
	TH2D *hall  = (TH2D*)file0->Get("histograms/pass_systematic");

	TEfficiency* pEff = get_efficiency_2D(hall, hpass, "Eta", "Pt", "trackerMuon", "", true);
	pEff->Draw("colz");
}

void load_TEfficiency()
{
	TFile *file0 = TFile::Open("../results/efficiencies/systematic_2D/Jpsi_Run_2011/Eta_Pt_trackerMuon.root");

	TEfficiency* pEff = (TEfficiency*)file0->Get("histograms/trackerMuon_Eta_Pt_Efficiency");
	pEff->Draw();
	//pEff->Draw("colz");

}

void create_2d_efficiency()
{
	real_data_using_get_efficiency_2d();
}