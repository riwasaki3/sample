void eff_ang_20220214(){
  Int_t ndim = 31;
  std::vector< std::vector<TFile*> > file      (ndim, vector<TFile*>(ndim));;
  std::vector< std::vector<TTree*> > nt        (ndim, vector<TTree*>(ndim));;
  for (Int_t i = 1; i <= 28; i++){
    std::cout << "i: "  << i << std::endl;
    file[i][i+1] = new TFile(Form("l-%03d-%03d.root",i,i+1),"read");
    file[i][i+2] = new TFile(Form("l-%03d-%03d.root",i,i+2),"read");
    nt[i][i+1] = (TTree*)file[i][i+1]->Get("nt");
    nt[i][i+2] = (TTree*)file[i][i+2]->Get("nt");
  }

  TFile *fout = new TFile("eff_ang_20220214.root","recreate");
  
  TGraphErrors * gr = new TGraphErrors();
  gr->SetMarkerColor(1);
  //gr->SetMarkerStyle(20);
  //gr->SetMarkerSize(0.5);
  gStyle->SetEndErrorSize(5);
  
  Int_t m = 0;
  std::vector< TH1* > hang(29);
  std::vector< TH1* > hang_coin(29);
  std::vector< TH1* > heff(29);
  std::vector< TH1* > hdr(29);
  std::vector< TH1* > hdr_coin(29);
  std::vector< TH1* > hda(29);
  std::vector< TH1* > hda_coin(29);
  std::vector< TH2* > hxy(29);
  std::vector< TH2* > hxy_coin(29);
  std::vector< TH2* > haxay(29);
  std::vector< TH2* > haxay_coin(29);
  std::vector< TH2* > hdxdy(29);
  std::vector< TH2* > hdxdy_coin(29);
  std::vector< TH2* > hdaxday(29);
  std::vector< TH2* > hdaxday_coin(29);
  for (Int_t i = 1; i <= 28; i++){
    std::cout << "i: "  << i << std::endl;
    Int_t id0[3];
    Int_t nent1 = nt[i][i+1]->GetEntries();
    Int_t nent2 = nt[i][i+2]->GetEntries();
    nt[i][i+1]->SetBranchAddress("id0", &(id0[1]));
    nt[i][i+2]->SetBranchAddress("id0", &(id0[2]));
    std::vector < std::vector <Int_t> > coin_ient1(nent2, std::vector<Int_t>(0));
    
    Int_t ient2 = 0;
    for (Int_t ient1 = 0; ient1 < nent1; ient1++) {
      nt[i][i+1]->GetEntry(ient1);
      Int_t jent2 = 0;
      while(ient2+jent2 < nent2){
	nt[i][i+2]->GetEntry(ient2+jent2);
//		std::cout << "ient1, ient2, jent2, id0[1], id[2]: "
//			  << ient1 << ", "
//			  << ient2 << ", "
//			  << jent2 << ", "
//			  << id0[1] << ", "
//			  << id0[2] << std::endl;
	if(id0[1] < id0[2]) {
	  break;
	}else if(id0[1] == id0[2]){
	  //	  std::cout << "coin" << std::endl;
	  coin_ient1[ient2+jent2].push_back(ient1);
	  jent2++;
	}else{
	  ient2 = ient2 + jent2 + 1;
	  jent2 = 0;
	}
      }
    }
    
    hang[i] = new TH1D(Form("hang_%d",i), Form("hang_%d",i), 40,0,4);
    hang_coin[i] = new TH1D(Form("hang_coin_%d",i), Form("hang_coin_%d",i), 40,0,4);
    heff[i] = new TH1D(Form("heff_%d",i), Form("heff_%d",i), 40,0,4);
    
    hdr[i]          = new TH1D(Form("hdr_%d",i), Form("hdr_%d",i), 200,0,80);
    hda[i]          = new TH1D(Form("hda_%d",i), Form("hda_%d",i), 200,0,0.4);
    hxy[i]          = new TH2D(Form("hxy_%d",i), Form("hxy_%d",i), 200,0,80000,200,0,80000);
    haxay[i]        = new TH2D(Form("haxay_%d",i), Form("haxay_%d",i), 200,-2,2,200,-2,2);
    hdxdy[i]        = new TH2D(Form("hdxdy_%d",i), Form("hdxdy_%d",i), 200,-80,80,200,-80,80);
    hdaxday[i]      = new TH2D(Form("hdaxday_%d",i), Form("hdaxday_%d",i), 200,-0.4,0.4,200,-0.4,0.4);
    hxy_coin[i]     = new TH2D(Form("hxy_coin_%d",i), Form("hxy_coin_%d",i), 200,0,80000,200,0,80000);
    hda_coin[i]     = new TH1D(Form("hda_coin_%d",i), Form("hda_coin_%d",i), 100,0,0.4);
    hdr_coin[i]     = new TH1D(Form("hdr_coin_%d",i), Form("hdr_coin_%d",i), 100,0,80);
    haxay_coin[i]   = new TH2D(Form("haxay_coin_%d",i), Form("haxay_coin_%d",i), 200,-2,2,200,-2,2);
    hdxdy_coin[i]   = new TH2D(Form("hdxdy_coin_%d",i), Form("hdxdy_coin_%d",i), 200,-80,80,200,-80,80);
    hdaxday_coin[i] = new TH2D(Form("hdaxday_coin_%d",i), Form("hdaxday_coin_%d",i), 100,-0.4,0.4,100,-0.4,0.4);
    
    Float_t x0[3],y0[3],ax0[3],ay0[3],ax1[3],ay1[3],dx[3],dy[3];
    for (Int_t j = 1; j<=2; j++){
      nt[i][i+j]->SetBranchAddress("x0", &(x0[j]));
      nt[i][i+j]->SetBranchAddress("y0", &(y0[j]));
      nt[i][i+j]->SetBranchAddress("dx", &(dx[j]));
      nt[i][i+j]->SetBranchAddress("dy", &(dy[j]));
      nt[i][i+j]->SetBranchAddress("ax0", &(ax0[j]));
      nt[i][i+j]->SetBranchAddress("ay0", &(ay0[j]));
      nt[i][i+j]->SetBranchAddress("ax1", &(ax1[j]));
      nt[i][i+j]->SetBranchAddress("ay1", &(ay1[j]));
    }
    for (Int_t ient2 = 0; ient2 < nent2; ient2++) {
      nt[i][i+2]->GetEntry(ient2);
      Double_t dr[3],da[3],a0[3];
      dr[2]= sqrt(pow(dx[2],2)+pow(dy[2],2));
      da[2] = sqrt(pow(ax0[2]-ax1[2],2)+pow(ay0[2]-ay1[2],2));
      a0[2] = sqrt(pow(ax0[2],2)+pow(ay0[2],2));
      hda[i]->Fill(da[2]);
      hdr[i]->Fill(dr[2]);
      hxy[i]->Fill(x0[2],y0[2]);
      haxay[i]->Fill(ax0[2],ay0[2]);
      hdxdy[i]->Fill(dx[2],dy[2]);
      hdaxday[i]->Fill(ax0[2]-ax1[2],ay0[2]-ay1[2]);
      if ((da[2]<0.04) && (dr[2] < 15) &&
	  (x0[2] > 20000) && (x0[2] < 50000) &&
	  (y0[2] > 10000) && (y0[2] < 35000)){
	hang[i] ->Fill(a0[2]);
	for (Int_t j =0; j < coin_ient1[ient2].size(); j++) {
	  nt[i][i+1]->GetEntry(coin_ient1[ient2][j]);
	  dr[1] = sqrt(pow(dx[1],2)+pow(dy[1],2));
	  da[1] = sqrt(pow(ax0[1]-ax1[1],2)+pow(ay0[1]-ay1[1],2));
	  hxy_coin[i]->Fill(x0[1],y0[1]);
	  haxay_coin[i]->Fill(ax0[1],ay0[1]);
	  hda_coin[i]->Fill(da[1]);
	  hdr_coin[i]->Fill(dr[1]);
	  hdxdy_coin[i]->Fill(dx[1],dy[1]);
	  hdaxday_coin[i]->Fill(ax0[1]-ax1[1],ay0[1]-ay1[1]);
	  if ((da[1]<0.04) && (dr[1] < 15)) {
	    hang_coin[i] ->Fill(a0[2]);
	  }
	}
      }
    }
    for (Int_t k=1; k<= hang[i]->GetNbinsX(); k++){
      if(hang[i]->GetBinContent(k)!=0){
	heff[i]->SetBinContent(k,100*hang_coin[i]->GetBinContent(k)/hang[i]->GetBinContent(k));
	heff[i]->SetBinError(k,100*sqrt((hang[i]->GetBinContent(k)-hang_coin[i]->GetBinContent(k))*hang_coin[i]->GetBinContent(k))/hang[i]->GetBinContent(k)/sqrt(hang[i]->GetBinContent(k)));
      }else{
	heff[i]->SetBinContent(k,0);
      }
    }
    Double_t eff = 100*hang_coin[i]->Integral()/hang[i]->Integral();
    Double_t e_eff = 100*sqrt((hang[i]->Integral()-hang_coin[i]->Integral())*hang_coin[i]->Integral())/sqrt(hang[i]->Integral())/hang[i]->Integral();
    gr->SetPoint(m, i+1, eff);
    gr->SetPointError(m, 0., e_eff);
    m++;
  }
  
  std::string pdfname = "eff_ang_20220214.pdf";
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  c1->SetLogx(0);
  c1->SetLogy(0);
  c1->SetMargin(0.1,0.1,0.1,0.1);
  c1->Print(Form("%s[",pdfname.c_str()));
  
  TH1 *frame = c1->DrawFrame(0,80.,30,100.);
  frame->SetTitle(";PL number;Efficiency (%)");
  frame->GetYaxis()->CenterTitle();
  frame->GetXaxis()->CenterTitle();
  frame->GetXaxis()->SetLabelOffset(0.001);
  frame->Draw();
  gr->Draw("p");
  TLegend* legend = new TLegend(0.3,0.1,0.9,0.3);
  legend->SetFillStyle(0);
  legend->SetLineStyle(0);
  legend->SetLineWidth(0);
  legend->SetTextSize(0.03);
  legend->SetTextFont(132);
  legend->SetBorderSize(0);
  legend->SetMargin(0.3);
  legend->SetHeader("Efficiency of tracks",""); /* option "C" allows to center the header*/
  legend->AddEntry(gr,  "No. of coin. / No. of tracks in the 2nd linklet","pe");
  legend->Draw();
  
  c1->Print(Form("%s",pdfname.c_str()));
  c1->Clear();  
  c1->Divide(2,2);
  for (Int_t i = 1; i <= 28; i++){
    c1->cd(1); hang[i]->Draw();
    c1->cd(2); hang_coin[i]->Draw();
    c1->cd(3); heff[i]->Draw("e");
     c1->Print(Form("%s",pdfname.c_str()));
  }
  c1->Print(Form("%s]",pdfname.c_str()));
  
  fout->Write();
  fout->Close();
  return;
}
