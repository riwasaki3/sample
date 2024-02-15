#include "TColor.h"
#include "TStyle.h"


void setCanvas()
{
 const Int_t NRGBs = 5;
 const Int_t NCont = 255;
 Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
 Double_t Red[NRGBs] = { 0.00, 0.00, 0.87, 1.00, 0.51 };
 Double_t Green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
 Double_t Blue[NRGBs] = { 0.51, 1.00, 0.12, 0.00, 0.00 };
 TColor::CreateGradientColorTable(NRGBs, stops, Red, Green, Blue, NCont);
 gStyle->SetNumberContours(NCont);
 gStyle->SetOptStat(0);
 //gStyle->SetPadGridX(true);
 //gStyle->SetPadGridY(true);
 gStyle->SetPadLeftMargin(0.15);
 gStyle->SetPadRightMargin(0.15);
 gStyle->SetPadBottomMargin(0.12);
 gStyle->SetTextFont(132);
 gStyle->SetTitleFont(132,"XYZ");
 gStyle->SetLabelFont(132,"XYZ");
}



int lecture()
{
    setCanvas();

  // if you want to save histogram --> 1
  int save_switch = 0;

  // Set histogram range
  int Neutron_range[2] = {1,200};
  int Proton_range[2]  = {1,130};

  // Set histogram
  TH2D*hist = new TH2D("hist","Nuclear chart;Neutron Number;Proton Number;(E_{B}/A)/MeV",
               Neutron_range[1] - Neutron_range[0],Neutron_range[0] - 0.5,Neutron_range[1] - 0.5,
               Proton_range[1]  - Proton_range[0] ,Proton_range[0]  - 0.5,Proton_range[1]  -  0.5);

  // Define some values
  double Mn = 939.6;
  double Mp = 938.3;

  // Prepare variable
  double Energy[7];
  double Answer;
  int    a,delta;

  gStyle -> SetOptStat(0);
  double min = 100000000;
  double max = 0;

  // Input Neutron & Proton 1 ~ 80
  for(int n = Neutron_range[0] ; n < Neutron_range[1] ; n++)
    {
      for(int z = Proton_range[0] ; z < Proton_range[1] ; z++)
    {
      // Define delta(Paring Term)
      if(n % 2 == 0 && z % 2 == 0)// even - even
        {
          delta = -1;
        }
      if(n % 2 == 1 && z % 2 == 1)// odd  - odd
        {
          delta = 1;
        }
      else                        // even - odd
        {
          delta = 0;
        }

      // define A
      a      = z + n;
      // Initirize
      Answer = 0;

      // Input Each Term
      Energy[0] =      z * Mp;
      Energy[1] =      n * Mn;
      Energy[2] = -15.67 * (double)a;
      Energy[3] =  17.23 * pow(a, 2.0 / 3.0);
      Energy[4] =  0.714 * pow(z, 2)   / pow((double)a, 1.0 / 3.0);
      Energy[5] =  93.15 * pow(n-z, 2) / (4 * (double)a);
      Energy[6] =   11.2 * delta       / pow((double)a, 1.0 / 2.0);

      // Calcurate Binding Energy
      for(int i = 2 ; i < 7 ; i++)
        {
          Answer += Energy[i];
        }

      // Check Minimum & Maximum value
      if(min > Answer / (double)a)
        {
          min = Answer / (double)a;
        }
      if(max < Answer / (double)a)
        {
          max = Answer / (double)a;
        }

      if(Answer < 0){
         hist -> Fill(n, z, -Answer / (double)a);
        }
      // Input Binding Energy
     
    }
    }

  // Output Minimum & Maximum value
  cout << "Minimum Value = " << min << endl << "Maximum Value = " << max << endl;
  hist -> GetZaxis() -> SetTitleOffset(0.8);


  // Output Histogram of Binding Energy
  hist -> Draw("colz");

  // Save Histogram
  if(save_switch == 1)hist -> SaveAs("Nuclear_Chart.root");

  ////////////////////////////////////////////////////////////
  ///// root Nuclear_Chart.root  --> hist->Draw("colz") //////
  ////////////////////////////////////////////////////////////
  
  return 0;
    
}

