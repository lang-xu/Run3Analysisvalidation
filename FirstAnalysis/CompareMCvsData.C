// This is a macro to compare different variables'distributions of Monte Carlo and data.
// In order to do so, you will need an output ROOT file from MC and another from data.
// To run the code, please check the input file names.
// Author: Lang Xu <lang.xu@cern.ch>

void CompareMCvsData(TString filemc = "AnalysisResults_O2_MC.root", TString filedata = "AnalysisResults_O2_DATA.root", TString task = "xic" ){
//Set Canvas
	TCanvas *c = new TCanvas("c",Form("MC and data Comparison for %s task", task.Data()),2100,1200);
	c->Divide(4,3);
	gStyle->SetOptStat(0);
	gStyle->SetPalette(0);
	gStyle->SetCanvasColor(0);
	gStyle->SetFrameFillColor(0);


//Read original root files
	auto fileMC(TFile::Open(filemc.Data(),"READ"));
	auto fileDATA(TFile::Open(filedata.Data(),"READ"));

//Loop for get histograms and make the comparison.
    TString histnames[]={Form("hf-task-%s/hptcand",task.Data()),Form("hf-task-%s/hptprong0",task.Data()),Form("hf-task-%s/hptprong1",task.Data()),Form("hf-task-%s/hptprong2",task.Data()),Form("hf-task-%s/hmass",task.Data()),Form("hf-task-%s/hCPA",task.Data()),Form("hf-task-%s/hCt",task.Data()),Form("hf-task-%s/hEta",task.Data()),Form("hf-task-%s/hdeclength",task.Data()),Form("hf-task-%s/hd0Prong0",task.Data()),Form("hf-task-%s/hd0Prong1",task.Data()),Form("hf-task-%s/hd0Prong2",task.Data())};
	for (int i = 0; i<12; i++){
		auto oMC = fileMC->Get(histnames[i]);		
		auto oDATA = fileDATA->Get(histnames[i]);
		TH1 *hMC;
		TH1 *hDATA;
		if (oMC -> InheritsFrom("TH2")){
			hMC = static_cast<TH1D*>(((TH2F*)oMC)->ProjectionX(Form("hMC_x[%d]",i))); 
			hDATA = static_cast<TH1D*>(((TH2F*)oDATA)->ProjectionX(Form("hDATA_x[%d]",i)));
		} else{
			hMC = static_cast<TH1F*>(oMC);
			hDATA = static_cast<TH1F*>(oDATA);
		}
		
		hMC -> SetTitle("");
		hDATA -> SetTitle("");

		hMC -> Scale(1/hMC->GetEntries());
		hDATA -> Scale(1/hDATA->GetEntries());
		hMC -> Rebin(2);
		hDATA -> Rebin(2);

		hMC -> SetLineColor(kRed);
		hDATA -> SetLineColor(kBlue);

		TLegend *legend = new TLegend(0.75,0.6,0.95,0.75);
		legend->AddEntry(hMC,"MC");
		legend->AddEntry(hDATA,"data");
		legend->SetFillStyle(0);
		legend->SetLineWidth(0);

		c->cd(i+1);
		hMC -> Draw();
		hDATA -> Draw("same");
		legend -> Draw("same");	
	}

	c->SaveAs(Form("comparison_MCvsDATA_%s.png",task.Data()));
}
