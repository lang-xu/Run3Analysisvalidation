// This is a macro to compare different variables'distributions of Monte Carlo and data for Xic task.
// In order to do so, you will need an output ROOT file from MC and another from data.
// To run the code, please check the input file names.
// Author: Lang Xu <lang.xu@cern.ch>

void CompareMCvsData(TString filemc = "AnalysisResults_O2_MC.root", TString filedata = "AnalysisResults_O2_DATA.root" ){
//Set Canvas
	TCanvas *c = new TCanvas("c","MC and data Comparison for Xic task",2600,1100);
	c->Divide(4,3);
	gStyle->SetOptStat(0);
	gStyle->SetPalette(0);
	gStyle->SetCanvasColor(0);
	gStyle->SetFrameFillColor(0);


//Read original root files
	auto fileMC(TFile::Open(filemc.Data(),"READ"));
	auto fileDATA(TFile::Open(filedata.Data(),"READ"));

//Loop for get TH1 and make the comparison.
	TString hist1Dnames[]={"hf-task-xic/hptcand","hf-task-xic/hptprong0","hf-task-xic/hptprong1","hf-task-xic/hptprong2"};
	TH1F *hist1D_MC[4];
	TH1F *hist1D_DATA[4];
	for (int i = 0; i<4; i++){
		hist1D_MC[i] = static_cast<TH1F*>(fileMC->Get(hist1Dnames[i]));
		hist1D_DATA[i] = static_cast<TH1F*>(fileDATA->Get(hist1Dnames[i]));
	
		hist1D_MC[i] -> SetLineColor(kRed);
		hist1D_DATA[i] -> SetLineColor(kBlue);
			
		hist1D_MC[i] -> Scale(1/hist1D_MC[i]->GetEntries());
		hist1D_DATA[i] -> Scale(1/hist1D_DATA[i]->GetEntries());
		hist1D_MC[i] -> Rebin(2); 
		hist1D_DATA[i] -> Rebin(2);
		
		c->cd(i+1);
		hist1D_MC[i] -> Draw();
		hist1D_DATA[i] -> Draw("same");
		
		if(i == 0){
			TLegend *l = new TLegend(0.7,0.72,0.9,0.9);
			l->AddEntry(hist1D_MC[i],"MC");
			l->AddEntry(hist1D_DATA[i],"data");
			l->SetFillStyle(0);
			l->SetLineWidth(0);
			l->Draw("same");
		}	
	}


//Loop for get TH2 and make the comparison.
	TString hist2Dnames[]={"hf-task-xic/hmass","hf-task-xic/hCPA","hf-task-xic/hCt","hf-task-xic/hEta","hf-task-xic/hdeclength"};
	TH2F *hist2D_MC[5];
	TH2F *hist2D_DATA[5];
	TH1D *hist2D_MC_x[5];
	TH1D *hist2D_DATA_x[5];
	for (int i = 0; i<5; i++){
		hist2D_MC[i] = static_cast<TH2F*>(fileMC->Get(hist2Dnames[i]));
		hist2D_DATA[i] = static_cast<TH2F*>(fileDATA->Get(hist2Dnames[i]));
		
		hist2D_MC[i] -> SetLineColor(kRed);
		hist2D_DATA[i] -> SetLineColor(kBlue);

		if (i ==0 ){
			auto hist2D_MC_y = hist2D_MC[i]->ProjectionY(Form("hist2D_MC_y[%d]",i));
			auto hist2D_DATA_y = hist2D_DATA[i]->ProjectionY(Form("hist2D_DATA_y[%d]",i));
			hist2D_MC_y -> Scale(1/hist2D_MC[i]->GetEntries());
			hist2D_DATA_y -> Scale(1/hist2D_DATA[i]->GetEntries());
			c->cd(i+5);
			hist2D_MC_y -> Draw(); 
			hist2D_DATA_y -> Draw("same");
		}

		hist2D_MC_x[i] = hist2D_MC[i]->ProjectionX(Form("hist2D_MC_x[%d]",i));
		hist2D_DATA_x[i] = hist2D_DATA[i]->ProjectionX(Form("hist2D_DATA_x[%d]",i));
		
		hist2D_MC_x[i] -> Scale(1/hist2D_MC_x[i]->GetEntries());
		hist2D_DATA_x[i] -> Scale(1/hist2D_DATA_x[i]->GetEntries());
		hist2D_MC_x[i] -> Rebin(2); 
		hist2D_DATA_x[i] -> Rebin(2);
		
		c->cd(i+6);
		hist2D_MC_x[i] -> Draw();
		hist2D_DATA_x[i] -> Draw("same");
	}


	c->SaveAs("comparison_MCvsDATA_Xic.png");
}
