//Program to extract data (variables and vector format) of the root file and create histograms. This root file is made of others root files, therefore, has multiple entries which we access with a diferent method than a normal root file.

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atof */
#include <math.h>       /* sin */
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include <vector>
#ifndef ROOT_TLatex
#define ROOT_TLatex
#ifndef ROOTiosfwd
#include "Riosfwd.h"
#endif
#ifndef ROOT_TText
#include "TText.h"
#endif
#ifndef ROOT_TAttLine
#include "TAttLine.h"
#endif

using namespace std;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int analysis()
{
	//call a file for a histogram style (Optional)
	gROOT->LoadMacro("styleTDR.C"); 
	//setTDRStyle();

	//counters for file f1 (pythia)
	int count_Total_Events_pythia = 0;
	int counter_Muon_pythia = 0; 

	
	int count_Jpsi_pythia = 0; 


	//counters for file f2 (data)
	int count_Total_Events_data = 0;
	int counter_Muon_data = 0;
	
	int count_Jpsi_data = 0; 
	
		
	//Lorentz Vector
	TLorentzVector mu_1;
	TLorentzVector mu_2;	
	
	//Variaveis	and Vectors
	int Total_Events = 0;
	int Muons = 0;

	int nDimuon = 0;

	double InvariantMass = 0.;
	int PassingProbeTrackingMuon = 0;

	int nDimuon2 = 0;

	double ProbeMuon_Pt = 0.;
	
	double M = 0.;
	double Pt = 0.;
	double Eta = 0.;
	double Rapidity = 0.;
	
	//***********************************************************	
	//Creating Histogramas
	//***********************************************************	
	TH1D *h_PassingTagProbes_Mass = new TH1D("h_PassingTagProbes_Mass","h_PassingTagProbes_Mass",100,2.8,3.4);
	h_PassingTagProbes_Mass->SetTitle("Passing Tag and Probe Invariant Mass ; #mu#mu [GeV] ; Events ");
	h_PassingTagProbes_Mass->SetName("h_PassingTagProbes_Mass");

	TH1D *h_FailingTagProbes_Mass = new TH1D("h_FailingTagProbes_Mass","h_FailingTagProbes_Mass",100,2.8,3.4);
	h_FailingTagProbes_Mass->SetTitle("Faling Tag and Probe Invariant Mass ; #mu#mu [GeV] ; Events ");
	h_FailingTagProbes_Mass->SetName("h_FailingTagProbes_Mass");

//End Histograms
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------	
	//Reading the root file and the tree
	//TFile *f1 = new TFile("histo_pythia_trigger.root");
	//TTree *t1 = (TTree*)f1->Get("demo/AnalysisTree");

	//for file2
	//TFile *f2 = new TFile("all.root");
	TFile *f2 = new TFile("all.root");
	TTree *t1 = (TTree*)f2->Get("demo/AnalysisTree");
	TTree *t2 = (TTree*)f2->Get("demo/PlotControl");

	//addressing the memory to vector and variables for file f2
	//For Variables
	TBranch *b_ProbeMuon_Pt = t1->GetBranch("ProbeMuon_Pt");
	b_ProbeMuon_Pt->SetAddress(&ProbeMuon_Pt);
	TBranch *b_InvariantMass = t1->GetBranch("InvariantMass");
	b_InvariantMass->SetAddress(&InvariantMass);
	TBranch *b_PassingProbeTrackingMuon = t1->GetBranch("PassingProbeTrackingMuon");
	b_PassingProbeTrackingMuon->SetAddress(&PassingProbeTrackingMuon);
	
	//**********************************************************		
	//Reading Number of tree entries for file f2
	Long64_t nentries1 = t1->GetEntries();
	cout<< "Numero de Entradas: "<< nentries1 <<std::endl;

	Long64_t nentries2 = t2->GetEntries();
	cout<< "Numero de Entradas: "<< nentries2 <<std::endl;

	//**********************************************************
	//Creating a output root file	
	TFile f_analysis("resonance.root","recreate"); //Creates root file
	TTree t_analysis("t_analysis","analise_Tree"); //Creates a Tree
	t_analysis.Branch("InvariantMass",&InvariantMass); //Creates a branch

	// loop tree entries t1
	Long64_t nbytes = 0, nb = 0, i=0;
	for (Long64_t kentry1=0; kentry1<nentries1;kentry1++) 
	{
		Long64_t ientry1 = t1->LoadTree(kentry1);
   
		if (ientry1 < 0) break;

		//For counters (Intergers)
		b_ProbeMuon_Pt->GetEntry(ientry1);
		b_InvariantMass->GetEntry(ientry1);
		b_PassingProbeTrackingMuon->GetEntry(ientry1);
		
		if( PassingProbeTrackingMuon == 1 )
		{
			h_PassingTagProbes_Mass->Fill(InvariantMass);
			//cout << "ProbeMuon_Pt: "<< ProbeMuon_Pt << endl;	
			t_analysis.Fill();
		}
		else
		{
			h_FailingTagProbes_Mass->Fill(InvariantMass);
		}
	
	}//End loop tree t1

/*	for (Long64_t kentry2 =0; kentry2 < nentries2;kentry2++) // loop tree entries for file f2
	{
		Long64_t ientry2 = t2->LoadTree(kentry2);
		
		if (ientry2 < 0) break;
							
		for(Long64_t i=0; i < VectorProbeMuon_Pt->size(); i++) //loop nDimuon
		{ 
			cout << "======================================================== " << endl;
		} //end loop nDimuon

	}//End loop tree t2
*/

	
	h_PassingTagProbes_Mass->Write(); //Writing the histograms in the output tree
	h_FailingTagProbes_Mass->Write(); //Writing the histograms in the output tree
	t_analysis.Write();  //Write in the root file

   //=========================================================================		
	//Creating Canvas
	//TCanvas* c1 = new TCanvas("c1","Canvas 1 - Invriant Mass",1200,600);
	//h_dimuons_Mass->SetLineColor(kBlack);
	//TLegend* leg_dimuons_Mass = new TLegend(0.75,0.81,0.97,0.97);
	//leg_dimuons_Mass->SetFillColor(kWhite);
	//leg_dimuons_Mass->SetFillStyle(1001);
	//leg_dimuons_Mass->AddEntry(h_dimuons_Mass,"#mu#mu","L");
	//h_dimuons_Mass->Draw("ep1");
	//leg_dimuons_Mass->Draw();
	

}//end program

