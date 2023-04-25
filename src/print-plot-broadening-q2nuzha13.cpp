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
    style->SetGridColor(17);
    style->SetPadBorderMode(0);
    style->SetTickLength(0.002,"XY");
    style->SetTitleFont(62,"XY");
    style->SetTitleSize(0.04,"XY");
    gROOT->SetStyle("my");

    // Open file
    TFile* fin = new TFile((results_dir+file_name_broad).c_str());
    if(fin==NULL){std::cout<<"No input file!"<<std::endl; return 1;}

    // Create a canvas
    TCanvas* c = new TCanvas("c","",1200,1200);

    // Declare arrays to contain histos and graphs
    const int Zh_bins = 4;
    const int Zh_bin_start = 3;

    TH1F*         h[N_Q2][N_Nu][N_broadening];
    TGraphErrors* g[N_Q2][N_Nu][N_broadening];

    TGraphErrors* ga13[N_Q2][N_Nu][Zh_bins];

    // Obtain histos and graphs
    for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
    {
        for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
        {
            for(int targ = 0 ; targ < N_broadening ; targ++)
            {
                h[Q2_bin][Nu_bin][targ] = (TH1F*) fin->Get(get_broadening_Zh_histo_name(targ, Q2_bin, Nu_bin).c_str());

                g[Q2_bin][Nu_bin][targ] = new TGraphErrors(N_Zh);

                th1f_to_tgrapherrors(h[Q2_bin][Nu_bin][targ], g[Q2_bin][Nu_bin][targ]);
            }

            for(int Zh_bin = Zh_bin_start ; Zh_bin < (Zh_bin_start+Zh_bins) ; Zh_bin++)
            {
                int Zh_index = Zh_bin-Zh_bin_start;

                ga13[Q2_bin][Nu_bin][Zh_index] = new TGraphErrors(Zh_bins);
                set_zh_to_a13(g[Q2_bin][Nu_bin][0], g[Q2_bin][Nu_bin][1], g[Q2_bin][Nu_bin][2], ga13[Q2_bin][Nu_bin][Zh_index], Zh_bin);

                // Customize graphs
                ga13[Q2_bin][Nu_bin][Zh_index]->SetMarkerStyle(zh_marker[Zh_index]);
                ga13[Q2_bin][Nu_bin][Zh_index]->SetMarkerColor(zh_colors[Zh_index]);
                ga13[Q2_bin][Nu_bin][Zh_index]->SetLineColor(zh_colors[Zh_index]);

                shift_x(ga13[Q2_bin][Nu_bin][Zh_index], shift_x_a13[Zh_index], Zh_bins);
            }       
        }
    }

    // Set a 3x3 TMultiGraph array
    TMultiGraph* mg[N_Q2][N_Nu];

    for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
    {
        for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
        {
            mg[Q2_bin][Nu_bin] = new TMultiGraph();
            for(int Zh_index = 0 ; Zh_index < Zh_bins ; Zh_index++)
            {
                mg[Q2_bin][Nu_bin]->Add(ga13[Q2_bin][Nu_bin][Zh_index], "APEZ0");
            }
        }
    }

    // Loop through the 3x3 TPad array
    TPad* p[N_Q2][N_Nu];

    for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
    {
        for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
        {
            p[Q2_bin][Nu_bin] = new TPad(Form("p%i%i",Q2_bin,Nu_bin),Form("p%i%i",Q2_bin,Nu_bin),pad_X1[Q2_bin][Nu_bin],pad_Y1[Q2_bin][Nu_bin],pad_X2[Q2_bin][Nu_bin],pad_Y2[Q2_bin][Nu_bin]);
            set_pad_attributes(p[Q2_bin][Nu_bin], Q2_bin, Nu_bin);
        }
    }

    // Draw the TCanvas
    c->Draw();
    c->cd();

    // Draw the pads and the multigraphs in it
    for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
    {
        for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
        {
            p[Q2_bin][Nu_bin]->Draw();
            p[Q2_bin][Nu_bin]->cd();

            mg[Q2_bin][Nu_bin]->Draw("A");

            set_broadening_q2nuzha13_multigraph_properties(mg[Q2_bin][Nu_bin]);

            // Return to the canvas
            c->cd();
        }
    }

    // Draw Q2 and Nu kinematical limits
    TLatex* t_q2 = new TLatex();
    TLatex* t_nu = new TLatex();
    
    set_latex_properties(t_q2, t_nu);

    set_q2_limits_pads(p[0][2],p[1][2],p[2][2],t_q2);
    set_nu_limits_pads(p[0][0],p[0][1],p[0][2],t_nu);

    // Print
    c->Print("../output-plots/broadening-q2nuzha13.pdf");
    c->Print("../output-plots/broadening-q2nuzha13.png");

    // Close file
    fin->Close();

    // Free memory
    delete fin;

    std::cout<<"Finished printing plots"<<std::endl;

    return 0;
}