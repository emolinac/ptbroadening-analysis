#include <iostream>
#include "TStyle.h"
#include "TFile.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TPad.h"
#include "constants.h"
#include "plots.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    // Visual misc
    TStyle* style = new TStyle("my","my");
    style->SetGridColor(17); style->SetPadColor(0); style->SetCanvasColor(0);
    style->SetPadBorderMode(0);
    style->SetTickLength(0.002,"XY");
    style->SetTitleFont(62,"XY");
    style->SetTitleSize(0.04,"XY");
    gROOT->SetStyle("my");

    // Open file
    if(!check_file_existence(results_dir, file_name_meanpt2)) return 1;
    TFile* fin = new TFile((results_dir+file_name_meanpt2).c_str());

    // Create a canvas
    TCanvas* c = new TCanvas("c","",800,600);

    // Declare histos
    TH1F* h[N_targets]; TH1F* h_Q2[N_targets]; TH1F* h_Nu[N_targets]; TH1F* h_Zh[N_targets];
    
    // Declare graphs
    TGraphErrors* g[N_targets]; TGraphErrors* g_Q2[N_targets]; TGraphErrors* g_Nu[N_targets]; TGraphErrors* g_Zh[N_targets];
    
    // Obtain histos and graphs
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        h[targ]    = (TH1F*) fin->Get(get_meanPt2_histo_name(targ).c_str());
        h_Q2[targ] = (TH1F*) fin->Get(get_meanPt2_Q2_histo_name(targ).c_str());
        h_Nu[targ] = (TH1F*) fin->Get(get_meanPt2_Nu_histo_name(targ).c_str());
        h_Zh[targ] = (TH1F*) fin->Get(get_meanPt2_Zh_histo_name(targ).c_str());

        g[targ]    = new TGraphErrors(1);
        g_Q2[targ] = new TGraphErrors(N_Q2);
        g_Nu[targ] = new TGraphErrors(N_Nu);
        g_Zh[targ] = new TGraphErrors(N_Zh);
        
        th1f_to_tgrapherrors(h[targ]   , g[targ]   );
        th1f_to_tgrapherrors(h_Q2[targ], g_Q2[targ]);
        th1f_to_tgrapherrors(h_Nu[targ], g_Nu[targ]);
        th1f_to_tgrapherrors(h_Zh[targ], g_Zh[targ]);

        // Customize Q2 graph
        g_Q2[targ]->SetMarkerStyle(targ_marker[targ]);
        g_Q2[targ]->SetMarkerColor(targ_colors[targ]);
        g_Q2[targ]->SetLineColor(targ_colors[targ]);
        set_xerr_null(g_Q2[targ], N_Q2);
        shift_x(g_Q2[targ], shift_x_q2[targ], N_Q2);

        // Customize Nu graph
        g_Nu[targ]->SetMarkerStyle(targ_marker[targ]);
        g_Nu[targ]->SetMarkerColor(targ_colors[targ]);
        g_Nu[targ]->SetLineColor(targ_colors[targ]);
        set_xerr_null(g_Nu[targ], N_Nu);
        shift_x(g_Nu[targ], shift_x_nu[targ], N_Nu);

        // Customize Zh graph
        g_Zh[targ]->SetMarkerStyle(targ_marker[targ]);
        g_Zh[targ]->SetMarkerColor(targ_colors[targ]);
        g_Zh[targ]->SetLineColor(targ_colors[targ]);
        set_xerr_null(g_Zh[targ], N_Zh);
        shift_x(g_Zh[targ], shift_x_zh[targ], N_Zh);

        // Customize A13 graph
        g[targ]->SetMarkerStyle(targ_marker[targ]);
        g[targ]->SetMarkerColor(targ_colors[targ]);
        g[targ]->SetLineColor(targ_colors[targ]);
        set_xerr_null(g[targ], 1);
        set_x_a13(g[targ], targ);
    }
    
    // Set a TMultiGraph
    TMultiGraph* mg    = new TMultiGraph();
    TMultiGraph* mg_Q2 = new TMultiGraph();
    TMultiGraph* mg_Nu = new TMultiGraph();
    TMultiGraph* mg_Zh = new TMultiGraph();

    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        mg->Add(g[targ], "APEZ0");
        mg_Q2->Add(g_Q2[targ], "APEZ0");
        mg_Nu->Add(g_Nu[targ], "APEZ0");
        mg_Zh->Add(g_Zh[targ], "APEZ0");
    }
    
    // Set Pads
    TPad* p    = new TPad("p"   ,"p"   ,0,0,1,1);
    TPad* p_Q2 = new TPad("p_Q2","p_Q2",0,0,1,1);
    TPad* p_Nu = new TPad("p_Nu","p_Nu",0,0,1,1);
    TPad* p_Zh = new TPad("p_Zh","p_Zh",0,0,1,1);

    set_pad_attributes(p   );
    set_pad_attributes(p_Q2);
    set_pad_attributes(p_Nu);
    set_pad_attributes(p_Zh);
    
    // Draw the TCanvas
    c->Draw();
    c->cd();

    // Draw Q2 Plots
    p_Q2->Draw();
    p_Q2->cd();

    mg_Q2->Draw("A");
    set_meanpt2_q2_multigraph_properties(mg_Q2);
    c->Print("../output-plots/meanpt2-q2.pdf");
    c->Print("../output-plots/meanpt2-q2.png");
    c->cd();

    // Draw Nu Plots
    p_Nu->Draw();
    p_Nu->cd();

    mg_Nu->Draw("A");
    set_meanpt2_nu_multigraph_properties(mg_Nu);
    c->Print("../output-plots/meanpt2-nu.pdf");
    c->Print("../output-plots/meanpt2-nu.png");
    c->cd();

    // Draw Zh Plots
    p_Zh->Draw();
    p_Zh->cd();

    mg_Zh->Draw("A");
    set_meanpt2_zh_multigraph_properties(mg_Zh);
    c->Print("../output-plots/meanpt2-zh.pdf");
    c->Print("../output-plots/meanpt2-zh.png");
    c->cd();

    // Draw A13 Plots
    p->Draw();
    p->cd();

    mg->Draw("A");
    set_meanpt2_a13_multigraph_properties(mg);
    c->Print("../output-plots/meanpt2-a13.pdf");
    c->Print("../output-plots/meanpt2-a13.png");
    c->cd();

    // Close file
    fin->Close();

    // Free memory
    delete fin;

    std::cout<<"Finished printing plots"<<std::endl;

    return 0;
}