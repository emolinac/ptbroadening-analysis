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
    if(!check_file_existence(results_dir, file_name_pt2_bg)) return 1;
    TFile* fin = new TFile((results_dir+file_name_pt2_bg).c_str());
    
    TFile* fout = new TFile((results_dir+file_name_meanpt2).c_str(),"RECREATE");
    gROOT->cd();

    // Instrumental histos
    TH1F* h_Pt2_integral = new TH1F("","",N_Pt2,Pt2_min,Pt2_max);
    TH1F* h_Pt2          = new TH1F("","",N_Pt2,Pt2_min,Pt2_max);
    TH1F* h_Pt2_vars;

    h_Pt2_integral->Sumw2();
    
    // Results histos
    TH1F* h_meanPt2[N_targets];
    TH1F* h_meanPt2_Q2[N_targets];
    TH1F* h_meanPt2_Nu[N_targets];
    TH1F* h_meanPt2_Zh[N_targets];
    TH1F* h_meanPt2_Q2Nu[N_targets];

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
        h_meanPt2[targ]->Write(get_meanPt2_histo_name(targ).c_str());
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
        h_meanPt2_Q2[targ]->Write(get_meanPt2_Q2_histo_name(targ).c_str());
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
        h_meanPt2_Nu[targ]->Write(get_meanPt2_Nu_histo_name(targ).c_str());
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
        h_meanPt2_Zh[targ]->Write(get_meanPt2_Zh_histo_name(targ).c_str());
        gROOT->cd();
    }

    // meanPt2(Q2,Nu,Zh)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histos
        h_meanPt2_Q2Nu[targ] = new TH1F("","",N_Zh,Zh_limits);
        
        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
            {
                for(int Zh_bin = 0 ; Zh_bin < N_Zh ; Zh_bin++)
                {
                    // Obtain histos
                    h_Pt2_vars = (TH1F*) fin->Get(get_acccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());

                    // Assign content and error
                    set_mean_and_meanerror(h_Pt2_vars, h_meanPt2_Q2Nu[targ], Zh_bin + 1);
                }
                
                // Write the histos
                fout->cd();
                h_meanPt2_Q2Nu[targ]->Write(get_cleaninterpolated_meanPt2_Zh_histo_name(targ,Q2_bin,Nu_bin).c_str());
                gROOT->cd();

                // Reset histos
                h_meanPt2_Q2Nu[targ]->Reset();
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