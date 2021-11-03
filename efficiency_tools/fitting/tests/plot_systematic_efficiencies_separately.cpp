#include "src/create_folder.cpp"

void plot_systematic_efficiencies()
{
	const char* folderName = "../efficiency_result/Jpsi_Run_2011/";
	const char* filesName[3][2] = {
		{"nominal_trackerMuon.root",       "2xgaus_trackerMuon.root"},
		{"mass_2p75_3p35trackerMuon.root", "mass_2p85_3p25trackerMuon.root"},
		{"binfit95_trackerMuon.root",      "binfit105_trackerMuon.root"}
	};
	const char* legend[3][2] = {
		{"Gaus+CB, 100 bins, 2.8 #Leftrightarrow 3.3", 	"2x Gaussian"},
		{"Mass 2.75 #Leftrightarrow 3.35",              "Mass 2.85 #Leftrightarrow 3.25"},
		{"Mass 95 bins",                             	"Mass 105 bins"}
	};

	TCanvas* c1 = new TCanvas("systematic_efficiencies", "Systematic Efficiencies", 800, 800);
	c1->Divide(2,3);

	for (int row = 0; row <= 2; row++)
	{
		for (int col = 0; col <= 1; col++)
		{
			int id = row*2+col;
			c1->cd(id+1);
			//c1->SetMargin(0.10, 0.03, 0.11, 0.07);

			/*
			//Test
			TText *t = new TText(.5,.5, to_string(id).c_str());
			t->SetTextAlign(22);
			t->SetTextColor(kRed+2);
			t->SetTextFont(43);
			t->SetTextSize(40);
			t->Draw();
			*/


			string file_path = string(folderName) + string(filesName[row][col]);
			TFile *file0 = TFile::Open(file_path.c_str());
			if (file0 == NULL)
			{
				std::cerr << "Could not find \"" << file_path << "\" file.\n";
				std::cerr << "ABORTING...\n";
				abort();
			}

			const char* hist_path = "trackerMuon_Efficiency";
			TEfficiency* pEff0 = (TEfficiency*)file0->Get(hist_path);
			if (pEff0 == NULL)
			{
				std::cerr << "Could not find the histogram path \"" << hist_path << "\" in \"" << file_path << "\" file.\n";
				std::cerr << "ABORTING...\n";
				abort();
			}

			//Plot
			//pEff0->SetLineWidth(1);
			pEff0->Draw();
	
			//Set range in y axis
			gPad->Update();
			auto graph = pEff0->GetPaintedGraph();
			graph->SetMinimum(0.0);
			graph->SetMaximum(1.05);
			graph->SetMinimum(0.96);
			graph->SetMaximum(1.01);
			gPad->Update();

			pEff0->Draw();
			pEff0->SetTitle((string(legend[row][col]) + ";p_{T} [GeV/c];Efficiency").data());
		}
	}

	c1->SaveAs("systematic_plots.png");
}