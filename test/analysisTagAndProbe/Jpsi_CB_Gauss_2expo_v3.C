using namespace RooFit ;

double Jpsi_CB_Gauss_2expo_v3()
{

//Opening a root file and getting a TH1D wich contains the data
TFile* file = new TFile("resonance.root","read"); 
TH1D *massJpsiHist = (TH1D *)file->Get("h_FailingTagProbes_Mass");

RooRealVar mass("mass","mass",2.9,3.3);
RooDataHist rooNoCutMass("rooNoCutMass","rooNoCutMass",mass,Import(*massJpsiHist));


/*
//---------------------------------------------
//P A R A M E T E R S    C O M B I N A T I O N
//----------------------------------------------
//open a file
ofstream textfile;
textfile.open("chi2_23.txt");

for(double k = 3.090 ; k < 3.100 ; k = k + 0.001 ) //loop mean1
{
  for(double l = 0.01 ; l < 0.09 ; l = l + 0.01 )  //loop sigma1
  {
    for(double m = 0.1 ; m < 3.0 ; m = m + 0.1) //loop alpha1
    {
      for(double n = 0.1 ; n < 2.5 ; n = n + 0.1) //loop power
      {
	for(double o = 0.5 ; o < 5 ; o = o + 0.5)//lambda
		{
     	   for(double p = 50 ; p < 2000 ; p = p + 50 )  //frac_sig
  		   {
      		 for(double q = 50 ; q < 1100 ; q = q + 50 )  //loop frac_bkg
  		     {*/


//--------------------------------------------------
//Roofit Variables
//--------------------------------------------------
RooRealVar mean1( "mean", "mean1", 3.094, 2.95, 3.151);
RooRealVar sigma1( "CrystalBallSigma", "sigma1", 0.0200, 0.0110, 0.100);
RooRealVar sigma2( "GausSigma", "sigma2", 0.021, 0.010, 0.100);
RooRealVar alpha( "CrystalBallAlpha", "tail shift", 2.10, 0.40, 2.30);
RooRealVar npow( "CrystalBallnpow", "power order", 0.40, 0.10, 11.50);
RooRealVar lambda1( "expo lambda", "slope", -1.7, -1.7, 5.5);
RooRealVar lambda2( "expo lambda2", "slope", 2.0, 0.1, 6.8);

//---------------------------------------------------------------
//F U N C T I O N S
//---------------------------------------------------------------
//Crystall Ball Fit (S I G N A L)
RooCBShape cball1("cball1","cball1",mass,mean1,sigma1,alpha,npow);

//Gaussian (S I G N A L)
RooGaussian gauss("gauss","gauss",mass,mean1,sigma2);

//ROOFIT EXPONENTIAL (B A C K G R O U N D)
RooExponential expo1("expo1", "exponential PDF", mass, lambda1);

//ROOFIT EXPONENTIAL (B A C K G R O U N D)
RooExponential expo2("expo2", "exponential PDF", mass, lambda2);

//------------------------------------------------
//A D D  S I G N A L  +  S I G N A L
//------------------------------------------------
RooRealVar signalCB_frac("signalCB_frac","fraction of crystal ball", 900, 600 , 3500);
RooRealVar signalGauss_frac("signalGauss_frac","fraction of Gaussian", 650 , 200 , 670);
RooAddPdf signal("signal","Signal CB + Gauss", RooArgList(cball1,gauss), RooArgList(signalCB_frac,signalGauss_frac));

//--------------------------------------------------
//A D D   B A C K G R O U N D + B A C K G R O U N D
//--------------------------------------------------
RooRealVar bkgExpo1_frac("bkgExpo1_frac", "fraction of expo1", 500, 100, 8000);
RooRealVar bkgExpo2_frac("bkgExpo2_frac", "fraction of expo2", 300, 200, 11000); 
RooAddPdf bkg( "bkg", "Background expo1 + expo2", RooArgList(expo1,expo2), RooArgList(bkgExpo1_frac,bkgExpo2_frac));

//------------------------------------------------
// A d d  s i g n a l   a n d   b a c k g r o u n d
// ------------------------------------------------
// Sum the composite signal and background 
RooRealVar signal_frac("signal_frac","fraction of signal", 1620, 930, 2630);
RooRealVar bkg_frac("bkg_frac","fraction of background", 290, 250, 290);
RooAddPdf  sigbkg("sigbkg","CBall1 + Expo",RooArgList(signal,bkg),RooArgList(signal_frac,bkg_frac));

//F I T T I N G
sigbkg.fitTo(rooNoCutMass,Range(2.9,3.3), Extended());
//sigbkg.fitTo(rooNoCutMass);

//Creating a frame
RooPlot* frame = mass.frame(Title("J/psi Signal + Background"));
frame.SetXTitle("Invariant Mass [GeV]");

//Plots
rooNoCutMass.plotOn(frame,Name("Data"));
//cball1.plotOn(frame,LineStyle(kDashed),LineColor(kGreen));
//expo1.plotOn(frame,Name("expo1"),LineStyle(kDashed),LineColor(kBlack));
//expo2.plotOn(frame,Name("expo2"),LineStyle(kDashed),LineColor(kGreen));
sigbkg.plotOn(frame,Name("SPLUSBK"),LineColor(kBlue));
sigbkg.plotOn(frame,Components("exp*"),Name("exp"),LineStyle(kDashed),LineColor(kRed));

//--------------------------------------
//C H I 2   A N D   P A R A M E T E R S
//--------------------------------------
RooArgSet * pars = sigbkg.getParameters(rooNoCutMass);
int nfloatpars = pars->selectByAttrib("Constant",kFALSE)->getSize(); 
double mychsq = frame->chiSquare("SPLUSBK","Data", nfloatpars); 
double myndof = massJpsiHist->GetNbinsX() - nfloatpars;
//chi2 of the last fit tha was "plotOned"
//double alt_chi2 = frame->chiSquare();
 
cout << "--------------------------------------" << endl;
cout << "nfloatpars: "<< nfloatpars << endl;
cout << "mychsq: " << mychsq << endl;
cout << "myndof: " << myndof << endl;
//cout << "frame->chiSquare()= " << alt_chi2 << endl;
cout << "--------------------------------------" << endl;
//sig_bkg.Print("t");

//Parameters Legend
sigbkg.paramOn(frame,Layout(0.65,0.99,0.99),Format("NE"),Label(Form("#chi^{2}/ndf = %2.0f/%2.0f", myndof*mychsq, myndof)) );
frame->getAttText()->SetTextSize(0.02);//change the font size
//frame->getAttText()->SetBorderSize(1);

//----------------------------------------------------------
//modifying frame bin
//----------------------------------------------------------
//rooNoCutMass.plotOn(frame,Binning(5)) ;
//cball1.plotOn(frame,Binning(5)) ;


/*
			if (mychsq < 30.)
			{
//			cout << "========================================"<< endl;
//			cout << "Mean: "<< k << " ,Sigma: "<< l << " ,Alpha: "<< m << " ,Power: "<< n <<" ,chi^2 = " << frame->chiSquare() << endl;
			//textfile << "Mean: " << k << " ,mychsq: " << mychsq << endl;
//			cout << "========================================"<< endl;

   			textfile << "Mean: "<< k << " ,Sigma: "<< l << " ,Alpha: "<< m << " ,Power: "<< n << " ,Lambda: "<< o << " ,sig_frac: " << p << " ,bkg_frac: "<< q <<" ,chi^2 = " << chi2 << endl;
   
			}//end loop chi2<150

	   }//bkg_frac
	 }//sig_frac
	}//lambda
      }//end loop power1
    }//end loop alpha1
  }//end loop sigma1
}//end loop mean

//close file
textfile.close();*/

//Making a Canvas
TCanvas* Jpsi_teste_canvas = new TCanvas("Jpsi_teste_canvas","Jpsi_teste",1200,600);
frame->Draw();
//L E G E N D
//---------------------------------------------
TLegend *leg1 = new TLegend(0.1427379,0.6742557,0.2996661,0.8756567);
leg1->SetFillColor(kWhite);
leg1->AddEntry(frame->findObject("Data"),"Data","P");
leg1->AddEntry(frame->findObject("SPLUSBK"),"Sig + Bkg","L");
leg1->AddEntry(frame->findObject("exp"),"Background","LP");
//leg1->AddEntry(frame->findObject("expo1"),"Expo1","LP");
//leg1->AddEntry(frame->findObject("expo2"),"Expo2","LP");
leg1->Draw();

//TFile f_analysis("Jpsi_CB_Gauss_2expo.root","recreate"); //Creates root file
//TTree t_analysis("t_analysis","Jpsi_analysis_Tree"); //Creates a Tree
//t_analysis.Branch("vectorInvariantMassJpsi",&vectorInvariantMassJpsi); //Creates a branch
//t_analysis.Fill();
//frame->Write(); //Write() if a file is open, this function writes a root objectics on it.
//Jpsi_teste_canvas->Write(); //Write() if a file is open, this function writes a root objectics on it.
//t_analysis.Write();  //Write in the root file
//

//----------------------------------------
//S A V E   C A N V A S   A S   P N G
//----------------------------------------
//Jpsi_teste_canvas->SaveAs("/eos/user/r/ragomesd/analysis_2018/Jpsi_CB_Gauss_2expo_v2.png");

cout << "---------------------------------" << endl;
cout << "E N D   P R O G R A M" << endl;
cout << "---------------------------------" << endl;
}//end program
