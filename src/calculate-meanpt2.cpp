#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "constants.h"
#include "integrals.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    std::cout<<"Starting averaged squared transverse momentum measurements."<<std::endl;

    // Open files
    TFile* fin = new TFile("../output-files/pt2-distributions-bgtreated.root");
    if(fin==NULL){std::cout<<"No input file!"<<std::endl; return 1;}

    TFile* fout = new TFile("../output-files/results-meanpt2.root","RECREATE");
    gROOT->cd();

    // Instrumental histos
    TH1F* h_Pt2_integral =  new TH1F("","",N_Pt2,Pt2_min,Pt2_max);
    TH1F* h_Pt2          =  new TH1F("","",N_Pt2,Pt2_min,Pt2_max);
    TH1F* h_Pt2_vars[3];

    h_Pt2_integral->Sumw2();
    
    // Results histos
    TH1F* h_meanPt2[N_targets];
    TH1F* h_meanPt2_Q2[N_targets];
    TH1F* h_meanPt2_Nu[N_targets];
    TH1F* h_meanPt2_Zh[N_targets];
    TH1F* h_meanPt2_Q2Nu[N_targets][3];

    // meanPt2 (used for A13 dependence)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2[targ] = new TH1F("","",1,0,1);
        q2nuzh_integration(h_Pt2_integral, h_Pt2, fin, targ);

        // Assign content and error
        h_meanPt2[targ]->SetBinContent(1,h_Pt2_integral->GetMean());
        h_meanPt2[targ]->SetBinError(1,h_Pt2_integral->GetMeanError());

        // Write
        fout->cd();
        h_meanPt2[targ]->Write("meanPt2_" + targets[targ]);
        gROOT->cd();

        // Reset
        h_Pt2_integral->Reset();
        h_Pt2->Reset();

    }
    
    // meanPt2(Q2)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2_Q2[targ] = new TH1F("","",N_Q2,Q2_limits);
        
        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            nuzh_integration(h_Pt2_integral, h_Pt2, fin, targ, Q2_bin);

            // Assign content and error
            h_meanPt2_Q2[targ]->SetBinContent(Q2_bin+1,h_Pt2_integral->GetMean());
            h_meanPt2_Q2[targ]->SetBinError(Q2_bin+1,h_Pt2_integral->GetMeanError());

            // Reset
            h_Pt2_integral->Reset();
            h_Pt2->Reset();
        }
        
        // Write
        fout->cd();
        h_meanPt2_Q2[targ]->Write("meanPt2_Q2_" + targets[targ]);
        gROOT->cd();
    }

    // meanPt2(Nu)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2_Nu[targ] = new TH1F("","",N_Nu,Nu_limits);
        
        for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
        {
            q2zh_integration(h_Pt2_integral, h_Pt2, fin, targ, Nu_bin);

            // Assign content and error
            h_meanPt2_Nu[targ]->SetBinContent(Nu_bin+1,h_Pt2_integral->GetMean());
            h_meanPt2_Nu[targ]->SetBinError(Nu_bin+1,h_Pt2_integral->GetMeanError());

            // Reset
            h_Pt2_integral->Reset();
            h_Pt2->Reset();
        }
        
        // Write
        fout->cd();
        h_meanPt2_Nu[targ]->Write("meanPt2_Nu_" + targets[targ]);
        gROOT->cd();
    }

    // meanPt2(Zh)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2_Zh[targ] = new TH1F("","",N_Zh,Zh_limits);
        
        for(int Zh_bin = 0 ; Zh_bin < N_Zh ; Zh_bin++)
        {
            q2nu_integration(h_Pt2_integral, h_Pt2, fin, targ, Zh_bin);

            // Assign content and error
            h_meanPt2_Zh[targ]->SetBinContent(Zh_bin+1,h_Pt2_integral->GetMean());
            h_meanPt2_Zh[targ]->SetBinError(Zh_bin+1,h_Pt2_integral->GetMeanError());

            // Reset
            h_Pt2_integral->Reset();
            h_Pt2->Reset();
        }
        
        // Write
        fout->cd();
        h_meanPt2_Zh[targ]->Write("meanPt2_Zh_" + targets[targ]);
        gROOT->cd();
    }

    // meanPt2(Q2,Nu,Zh)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histos
        h_meanPt2_Q2Nu[targ][0] = new TH1F("","",N_Zh,Zh_limits);
        h_meanPt2_Q2Nu[targ][1] = new TH1F("","",N_Zh,Zh_limits);
        h_meanPt2_Q2Nu[targ][2] = new TH1F("","",N_Zh,Zh_limits);

        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
            {
                for(int Zh_bin = 0 ; Zh_bin < N_Zh ; Zh_bin++)
                {
                    // Obtain histos
                    h_Pt2_vars[0] = (TH1F*) fin->Get(Form("Pt2_" + targets[targ] + "_%i%i%i", Q2_bin, Nu_bin, Zh_bin));
                    h_Pt2_vars[1] = (TH1F*) fin->Get(Form("Pt2_" + targets[targ] + "_%i%i%i_clean", Q2_bin, Nu_bin, Zh_bin));
                    h_Pt2_vars[2] = (TH1F*) fin->Get(Form("Pt2_" + targets[targ] + "_%i%i%i_clean_interpolated", Q2_bin, Nu_bin, Zh_bin));

                    // Assign content and error
                    set_mean_and_meanerror(h_Pt2_vars[0], h_meanPt2_Q2Nu[targ][0], Zh_bin + 1);
                    set_mean_and_meanerror(h_Pt2_vars[1], h_meanPt2_Q2Nu[targ][1], Zh_bin + 1);
                    set_mean_and_meanerror(h_Pt2_vars[2], h_meanPt2_Q2Nu[targ][2], Zh_bin + 1);
                }
                
                // Write the histos
                fout->cd();
                h_meanPt2_Q2Nu[targ][0]->Write(Form("meanPt2_Zh_" + targets[targ] + "_%i%i", Q2_bin, Nu_bin));
                h_meanPt2_Q2Nu[targ][1]->Write(Form("meanPt2_Zh_" + targets[targ] + "_%i%i_clean", Q2_bin, Nu_bin));
                h_meanPt2_Q2Nu[targ][2]->Write(Form("meanPt2_Zh_" + targets[targ] + "_%i%i_clean_interpolated", Q2_bin, Nu_bin));
                gROOT->cd();

                // Reset histos
                h_meanPt2_Q2Nu[targ][0]->Reset();
                h_meanPt2_Q2Nu[targ][1]->Reset();
                h_meanPt2_Q2Nu[targ][2]->Reset();
            }
        }       
    }

    // Close files
    fin->Close();
    fout->Close();

    // Release memory
    delete fin; delete fout;

    std::cout<<"Finished averaged squared transverse momentum measurements."<<std::endl;
    
    return 0;
}