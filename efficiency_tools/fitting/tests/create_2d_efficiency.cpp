#include "../src/get_efficiency_2D.cpp"
#include "../src/create_folder.cpp"

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

void load_TEfficiency()
{
	TFile *file0 = TFile::Open("../results/efficiencies/systematic_2D/Jpsi_Run_2011/Eta_Pt_trackerMuon.root");

	TEfficiency* pEff = (TEfficiency*)file0->Get("histograms/trackerMuon_Eta_Pt_Efficiency");
	pEff->Draw();
	//pEff->Draw("colz");
}

void real_data_using_get_efficiency_2d()
{
	TFile *file0 = TFile::Open("../results/efficiencies/systematic_2D/Jpsi_Run_2011/Pt_Eta_trackerMuon.root");

	TH2D *hall  = (TH2D*)file0->Get("histograms/all_systematic");
	TH2D *hpass = (TH2D*)file0->Get("histograms/pass_systematic");

	TEfficiency* pEff = get_efficiency_2D(hall, hpass, "Pt", "Eta", "trackerMuon");


	for (Int_t i=1; i<=hall->GetNbinsX(); i++)
	{
		for (Int_t j=1; j<=hall->GetNbinsY(); j++)
		{
			double val_all  = hall ->GetBinContent(i,j);
			double val_pass = hpass->GetBinContent(i,j);
			double unc_all  = hall ->GetBinError(i,j);
			double unc_pass = hpass->GetBinError(i,j);

			double value     = val_pass/val_all;
			double uncertain = fabs(value)*sqrt(pow(unc_pass/val_pass, 2) + pow(unc_all/val_all, 2));

			printf(" bin %d,%d | teff %.3f - %.3f + %.3f  th from teff %.3f +- %.3f (sanity check sqrt(N) is: %.3f)\n",
				i, j,
				pEff ->GetEfficiency(hall->GetBin(i,j)),
				pEff ->GetEfficiencyErrorLow(hall->GetBin(i,j)),
				pEff ->GetEfficiencyErrorUp(hall->GetBin(i,j)),
				value, uncertain,
				TMath::Sqrt(hall ->GetBinContent(i,j))
				);
		}
	}
}


//Create Efficiency plot with TH2D
TH2D* get_TH2D_efficiency(TH2D* hall, TH2D* hpass)
{
	TH2D* heff  = (TH2D*)hall ->Clone();
	heff->GetYaxis()->SetTitle("Efficiency");

	for (int i=1; i<=hall->GetNbinsX(); i++)
	{
		for (int j=1; j<=hall->GetNbinsY(); j++)
		{
			double val_all  = hall ->GetBinContent(i,j);
			double val_pass = hpass->GetBinContent(i,j);
			double unc_all  = hall ->GetBinError(i,j);
			double unc_pass = hpass->GetBinError(i,j);

			double value     = val_pass/val_all;
			double uncertain = fabs(value)*sqrt(pow(unc_pass/val_pass, 2) + pow(unc_all/val_all, 2));

			heff->SetBinContent(i,j, value);
			heff->SetBinError(i,j, uncertain);
		}
	}

	return heff;
}

//Loads efficiencies and commands it to get efficiency
void real_data_using_TH2D_efficiency()
{
	//string MuonId   = "trackerMuon";
	string MuonId   = "standaloneMuon";
	//string MuonId   = "globalMuon";

	TFile *file0 = TFile::Open(string("../results/efficiencies/systematic_2D/Jpsi_Run_2011/Pt_Eta_"+MuonId+".root").c_str());

	TH2D *hall  = (TH2D*)file0->Get("histograms/all_systematic");
	TH2D *hpass = (TH2D*)file0->Get("histograms/pass_systematic");

	TH2D* heff = get_TH2D_efficiency(hall, hpass);

	TCanvas* c1 = new TCanvas();
	c1->cd();
	heff->Draw("colztexte");
	//heff->SetMaximum(1.);
	//heff->SetMinimum(0.88);
	heff->SetMaximum(1.06);
	heff->SetMinimum(0.4);

	gStyle->SetOptStat(0);
	c1->SetLogx();

	const char* output_folder_name = "results/tests/";
	create_folder(output_folder_name);
	
	c1->SaveAs(string(output_folder_name + MuonId + "_TH2D.png").c_str());
}

//Main function
void create_2d_efficiency()
{
	//real_data_using_get_efficiency_2d();
	real_data_using_TH2D_efficiency();
}