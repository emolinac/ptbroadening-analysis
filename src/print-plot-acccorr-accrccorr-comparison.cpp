#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "utils.h"
#include "plots.h"

// Convention for arrays: [X][Y]
// [X] : Refers to the type of results, i.e., uncorrected or corrected.
// [Y] : Refers to the target.

int main(int argc, char* argv[])
{
    // Visual misc
    TStyle* style = new TStyle("my","my");
    style->SetGridColor(17); style->SetPadColor(0); style->SetCanvasColor(0);
    style->SetPadBorderMode(0);
    style->SetTickLength(0.002,"XY");
    style->SetTitleFont(62,"XY");
    style->SetTitleSize(0.04,"XY");
    gROOT->SetStyle("my");

    // Create a canvas
    TCanvas* c = new TCanvas("c","",800,600);

    // Open the file
    if(!check_file_existence(results_dir, file_name_meanpt2)) return 1;
    if(!check_file_existence(results_dir, file_name_broad)) return 1;

    TFile* fmeanpt2 = new TFile((results_dir+file_name_meanpt2).c_str());
    TFile* fbroad   = new TFile((results_dir+file_name_broad).c_str());

    // Get the necessary histos
    TH1F* hmeanpt2[2][N_targets];
    TH1F* hbroad[2][N_broadening];

    //// meanpt2
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        hmeanpt2[0][targ] = (TH1F*) fmeanpt2->Get(get_acc_meanPt2_Zh_histo_name(targ).c_str());
        hmeanpt2[1][targ] = (TH1F*) fmeanpt2->Get(get_accrc_meanPt2_Zh_histo_name(targ).c_str());
    }

    //// broadening
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        hbroad[0][targ] = (TH1F*) fbroad->Get(get_acc_broadening_Zh_histo_name(targ+3).c_str());
        hbroad[1][targ] = (TH1F*) fbroad->Get(get_accrc_broadening_Zh_histo_name(targ+3).c_str());
    }

    // Calculate the ratios to know the impact of the desired correction
    TH1F* hmeanpt2_ratio[N_targets];
    TH1F* hbroad_ratio[N_broadening];

    //// meanpt2
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        hmeanpt2_ratio[targ] = new TH1F("","",N_Zh,Zh_limits);
        hmeanpt2_ratio[targ]->Divide(hmeanpt2[0][targ],hmeanpt2[1][targ],1,1);
    }

    //// broadening
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        hbroad_ratio[targ] = new TH1F("","",N_Zh,Zh_limits);
        hbroad_ratio[targ]->Divide(hbroad[0][targ],hbroad[1][targ],1,1);
    }

    // Plot the ratios
    TGraphErrors* gmeanpt2_ratio[N_targets];
    TGraphErrors* gbroad_ratio[N_broadening];
    
    TMultiGraph* mgmeanpt2 = new TMultiGraph();
    TMultiGraph* mgbroad = new TMultiGraph();

    //// Transform the histos to graphs and add them to multigraphs
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Initiate graph and its visuals
        gmeanpt2_ratio[targ] = new TGraphErrors(N_Zh);
        gmeanpt2_ratio[targ]->SetMarkerColor(targ_colors[targ]);
        gmeanpt2_ratio[targ]->SetLineColor(targ_colors[targ]);
        gmeanpt2_ratio[targ]->SetMarkerStyle(targ_marker[targ]);

        th1f_to_tgrapherrors(hmeanpt2_ratio[targ], gmeanpt2_ratio[targ]);
        set_xerr_null(gmeanpt2_ratio[targ]);

        mgmeanpt2->Add(gmeanpt2_ratio[targ],"APXL");
    }

    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        gbroad_ratio[targ] = new TGraphErrors(N_Zh);
        gbroad_ratio[targ]->SetMarkerColor(targ_colors[targ+3]);
        gbroad_ratio[targ]->SetLineColor(targ_colors[targ+3]);
        gbroad_ratio[targ]->SetMarkerStyle(targ_marker[targ+3]);
        
        th1f_to_tgrapherrors(hbroad_ratio[targ], gbroad_ratio[targ]);
        set_xerr_null(gbroad_ratio[targ]);

        mgbroad->Add(gbroad_ratio[targ],"APL");
    }

    // Set Pads
    TPad* p_meanpt2 = new TPad("p_meanpt2","p_meanpt2",0,0,1,1);
    TPad* p_broad   = new TPad("p_broad"  ,"p_broad"  ,0,0,1,1);

    set_pad_attributes(p_meanpt2);
    set_pad_attributes(p_broad);

    // Draw the TCanvas
    c->Draw();
    c->cd();

    // Draw meanpt2 Plots
    p_meanpt2->Draw();
    p_meanpt2->cd();

    mgmeanpt2->Draw("A");
    set_meanpt2_acc_accrc_multigraph_properties(mgmeanpt2);
    c->Print("../output-plots/accrc-impact-meanpt2.pdf");
    c->Print("../output-plots/accrc-impact-meanpt2.png");
    c->cd();

    // Draw braodening Plots
    p_broad->Draw();
    p_broad->cd();

    mgbroad->Draw("A");
    set_broad_acc_accrc_multigraph_properties(mgbroad);
    c->Print("../output-plots/accrc-impact-broadening.pdf");
    c->Print("../output-plots/accrc-impact-broadening.png");
    c->cd();


    // Close files
    fmeanpt2->Close();
    fbroad->Close();

    return 1;
}
