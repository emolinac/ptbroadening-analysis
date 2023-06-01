#include <iostream>
#include "TStyle.h"
#include "TFile.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
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
    style->SetLegendFillColor(0);
    gROOT->SetStyle("my");

    // Open file
    if(!check_file_existence(results_dir, file_name_broad)) return 1;
    TFile* fin = new TFile((results_dir+file_name_broad).c_str());

    // Create a canvas
    TCanvas* c = new TCanvas("c","",1300,500);

    // Declare arrays to contain histos and graphs
    TH1F*         h[N_Q2][N_Nu][N_broadening];
    TGraphErrors* g[N_Q2][N_Nu][N_broadening];

    // Obtain histos and graphs
    for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
    {
        for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
        {
            for(int targ = 0 ; targ < N_broadening ; targ++)
            {
                h[Q2_bin][Nu_bin][targ] = (TH1F*) fin->Get(get_broadening_Zh_histo_name(targ+3, Q2_bin, Nu_bin).c_str());

                g[Q2_bin][Nu_bin][targ] = new TGraphErrors(N_Zh);

                th1f_to_tgrapherrors_zh(h[Q2_bin][Nu_bin][targ], g[Q2_bin][Nu_bin][targ]);

                // Customize graphs
                g[Q2_bin][Nu_bin][targ]->SetMarkerStyle(targ_marker_ndim[Q2_bin][targ]);
                g[Q2_bin][Nu_bin][targ]->SetMarkerColor(targ_colors[targ]);
                g[Q2_bin][Nu_bin][targ]->SetLineColor(targ_colors[targ]);
                set_xerr_null(g[Q2_bin][Nu_bin][targ]);

                // Slightly shift according to target
                shift_x(g[Q2_bin][Nu_bin][targ], shift_x_zh_big[targ]);

                //Slightly shift according to Nu bin
                shift_x(g[Q2_bin][Nu_bin][targ], shift_x_zh_compact[Q2_bin]);
            }
        }
    }

    // Set a 3x3 TMultiGraph array
    TMultiGraph* mg[N_Nu];

    for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
    {
        mg[Nu_bin] = new TMultiGraph();
        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            for(int targ = 0 ; targ < N_broadening ; targ++)
            {
                mg[Nu_bin]->Add(g[Q2_bin][Nu_bin][targ], "APEZ0");
            }
        }
    }

    // Loop through the 3x3 TPad array
    TPad* p[N_Nu];

    for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
    {
        p[Nu_bin] = new TPad(Form("p%i",Nu_bin),Form("p%i",Nu_bin),pad_X1_compact[Nu_bin],pad_Y1_compact[Nu_bin],pad_X2_compact[Nu_bin],pad_Y2_compact[Nu_bin]);
        set_pad_attributes(p[Nu_bin], Nu_bin);
    }
    
    // Draw the TCanvas
    c->Draw();
    c->cd();

    // Draw the pads and the multigraphs in it
    for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
    {
        p[Nu_bin]->Draw();
        p[Nu_bin]->cd();

        mg[Nu_bin]->Draw("A");

        set_broadening_q2nuzh_multigraph_properties(mg[Nu_bin]);

        // Return to the canvas
        c->cd();
    }
    
    // Draw Q2 and Nu kinematical limits
    TLatex* t_nu = new TLatex();
    
    set_latex_properties(t_nu);

    set_nu_limits_pads(p[0],p[1],p[2],t_nu);

    // Draw TLegend wth target info
    TLegend* target_legend_1 = new TLegend(p[0]->GetLeftMargin()     ,1-(p[0]->GetTopMargin()+0.14),p[0]->GetLeftMargin()+0.28,1-p[0]->GetTopMargin(),"","NDC");
    TLegend* target_legend_2 = new TLegend(p[0]->GetLeftMargin()+0.28,1-(p[0]->GetTopMargin()+0.14),p[0]->GetLeftMargin()+0.56,1-p[0]->GetTopMargin(),"","NDC");  
    TLegend* target_legend_3 = new TLegend(p[0]->GetLeftMargin()+0.56,1-(p[0]->GetTopMargin()+0.14),p[0]->GetLeftMargin()+0.84,1-p[0]->GetTopMargin(),"","NDC");
    target_legend_1->SetFillStyle(0);
    target_legend_1->SetBorderSize(0);
    target_legend_2->SetFillStyle(0);
    target_legend_2->SetBorderSize(0);
    target_legend_3->SetFillStyle(0);
    target_legend_3->SetBorderSize(0);
    
    target_legend_1->AddEntry(g[0][0][0],"C 1.0<Q^{2}(GeV^{2})<1.3", "p");
    target_legend_1->AddEntry(g[1][0][0],"C 1.3<Q^{2}(GeV^{2})<1.8", "p");
    target_legend_1->AddEntry(g[2][0][0],"C 1.8<Q^{2}(GeV^{2})<4.0", "p");  
    target_legend_2->AddEntry(g[0][0][1],"Fe 1.0<Q^{2}(GeV^{2})<1.3","p");
    target_legend_2->AddEntry(g[1][0][1],"Fe 1.3<Q^{2}(GeV^{2})<1.8","p");
    target_legend_2->AddEntry(g[2][0][1],"Fe 1.8<Q^{2}(GeV^{2})<4.0","p");
    target_legend_3->AddEntry(g[0][0][2],"Pb 1.0<Q^{2}(GeV^{2})<1.3","p");
    target_legend_3->AddEntry(g[1][0][2],"Pb 1.3<Q^{2}(GeV^{2})<1.8","p");
    target_legend_3->AddEntry(g[2][0][2],"Pb 1.8<Q^{2}(GeV^{2})<4.0","p");
    p[0]->cd();
    target_legend_1->Draw("SAME");
    target_legend_2->Draw("SAME");
    target_legend_3->Draw("SAME");

    // Print
    c->Print("../output-plots/broadening-q2nuzh-compact.pdf");
    c->Print("../output-plots/broadening-q2nuzh-compact.png");

    // Close file
    fin->Close();

    // Free memory
    delete fin;

    std::cout<<"Finished printing plots"<<std::endl;

    return 0;
}