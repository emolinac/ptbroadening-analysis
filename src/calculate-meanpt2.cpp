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
    TFile* fin = new TFile((results_dir+"rcfix_"+file_name_pt2_bg).c_str());
    
    TFile* fout = new TFile((results_dir+file_name_meanpt2).c_str(),"RECREATE");
    gROOT->cd();

    // Instrumental histos
    TH1F* h_Pt2_integral = new TH1F("","",N_Pt2,Pt2_min,Pt2_max);
    TH1F* h_Pt2          = new TH1F("","",N_Pt2,Pt2_min,Pt2_max);
    TH1F* h_Pt2_vars[3][3];

    h_Pt2_integral->Sumw2();
    
    // Results histos
    TH1F* h_meanPt2[3][N_targets];
    TH1F* h_meanPt2_Q2[3][N_targets];
    TH1F* h_meanPt2_Nu[3][N_targets];
    TH1F* h_meanPt2_Zh[3][N_targets];
    TH1F* h_meanPt2_Q2Nu[3][N_targets][3];

    // Uncorrected meanPt2 (used for A13 dependence)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2[0][targ] = new TH1F("","",1,0,1);

        q2nuzh_integration(h_Pt2_integral, h_Pt2, fin, targ);

        // Assign content and error
        h_meanPt2[0][targ]->SetBinContent(1,h_Pt2_integral->GetMean());
        h_meanPt2[0][targ]->SetBinError(1,h_Pt2_integral->GetMeanError());

        // Write
        fout->cd();
        h_meanPt2[0][targ]->Write(get_meanPt2_histo_name(targ).c_str());
        gROOT->cd();

        // Reset
        h_Pt2_integral->Reset();
        h_Pt2->Reset();
    }

    // Acceptance-corrected meanPt2 (used for A13 dependence)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2[1][targ] = new TH1F("","",1,0,1);

        q2nuzh_acc_integration(h_Pt2_integral, h_Pt2, fin, targ);

        // Assign content and error
        h_meanPt2[1][targ]->SetBinContent(1,h_Pt2_integral->GetMean());
        h_meanPt2[1][targ]->SetBinError(1,h_Pt2_integral->GetMeanError());

        // Write
        fout->cd();
        h_meanPt2[1][targ]->Write(get_acc_meanPt2_histo_name(targ).c_str());
        gROOT->cd();

        // Reset
        h_Pt2_integral->Reset();
        h_Pt2->Reset();
    }

    // Acceptance-Rad-corrected meanPt2 (used for A13 dependence)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2[2][targ] = new TH1F("","",1,0,1);

        q2nuzh_accrc_integration(h_Pt2_integral, h_Pt2, fin, targ);

        // Assign content and error
        h_meanPt2[2][targ]->SetBinContent(1,h_Pt2_integral->GetMean());
        h_meanPt2[2][targ]->SetBinError(1,h_Pt2_integral->GetMeanError());

        // Write
        fout->cd();
        h_meanPt2[2][targ]->Write(get_accrc_meanPt2_histo_name(targ).c_str());
        gROOT->cd();

        // Reset
        h_Pt2_integral->Reset();
        h_Pt2->Reset();
    }
    
    // Uncorrected meanPt2(Q2)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2_Q2[0][targ] = new TH1F("","",N_Q2,Q2_limits);
        
        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            nuzh_integration(h_Pt2_integral, h_Pt2, fin, targ, Q2_bin);

            // Assign content and error
            h_meanPt2_Q2[0][targ]->SetBinContent(Q2_bin+1,h_Pt2_integral->GetMean());
            h_meanPt2_Q2[0][targ]->SetBinError(Q2_bin+1,h_Pt2_integral->GetMeanError());

            // Reset
            h_Pt2_integral->Reset();
            h_Pt2->Reset();
        }
        
        // Write
        fout->cd();
        h_meanPt2_Q2[0][targ]->Write(get_meanPt2_Q2_histo_name(targ).c_str());
        gROOT->cd();
    }

    // Acceptance-corrected meanPt2(Q2)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2_Q2[1][targ] = new TH1F("","",N_Q2,Q2_limits);
        
        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            nuzh_acc_integration(h_Pt2_integral, h_Pt2, fin, targ, Q2_bin);

            // Assign content and error
            h_meanPt2_Q2[1][targ]->SetBinContent(Q2_bin+1,h_Pt2_integral->GetMean());
            h_meanPt2_Q2[1][targ]->SetBinError(Q2_bin+1,h_Pt2_integral->GetMeanError());

            // Reset
            h_Pt2_integral->Reset();
            h_Pt2->Reset();
        }
        
        // Write
        fout->cd();
        h_meanPt2_Q2[1][targ]->Write(get_acc_meanPt2_Q2_histo_name(targ).c_str());
        gROOT->cd();
    }

    // Acceptance-Rad-corrected meanPt2(Q2)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2_Q2[2][targ] = new TH1F("","",N_Q2,Q2_limits);
        
        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            nuzh_accrc_integration(h_Pt2_integral, h_Pt2, fin, targ, Q2_bin);

            // Assign content and error
            h_meanPt2_Q2[2][targ]->SetBinContent(Q2_bin+1,h_Pt2_integral->GetMean());
            h_meanPt2_Q2[2][targ]->SetBinError(Q2_bin+1,h_Pt2_integral->GetMeanError());

            // Reset
            h_Pt2_integral->Reset();
            h_Pt2->Reset();
        }
        
        // Write
        fout->cd();
        h_meanPt2_Q2[2][targ]->Write(get_accrc_meanPt2_Q2_histo_name(targ).c_str());
        gROOT->cd();
    }

    // Uncorrected meanPt2(Nu)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2_Nu[0][targ] = new TH1F("","",N_Nu,Nu_limits);
        
        for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
        {
            q2zh_integration(h_Pt2_integral, h_Pt2, fin, targ, Nu_bin);

            // Assign content and error
            h_meanPt2_Nu[0][targ]->SetBinContent(Nu_bin+1,h_Pt2_integral->GetMean());
            h_meanPt2_Nu[0][targ]->SetBinError(Nu_bin+1,h_Pt2_integral->GetMeanError());

            // Reset
            h_Pt2_integral->Reset();
            h_Pt2->Reset();
        }
        
        // Write
        fout->cd();
        h_meanPt2_Nu[0][targ]->Write(get_meanPt2_Nu_histo_name(targ).c_str());
        gROOT->cd();
    }

    // Acceptance-corrected meanPt2(Nu)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2_Nu[1][targ] = new TH1F("","",N_Nu,Nu_limits);
        
        for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
        {
            q2zh_acc_integration(h_Pt2_integral, h_Pt2, fin, targ, Nu_bin);

            // Assign content and error
            h_meanPt2_Nu[1][targ]->SetBinContent(Nu_bin+1,h_Pt2_integral->GetMean());
            h_meanPt2_Nu[1][targ]->SetBinError(Nu_bin+1,h_Pt2_integral->GetMeanError());

            // Reset
            h_Pt2_integral->Reset();
            h_Pt2->Reset();
        }
        
        // Write
        fout->cd();
        h_meanPt2_Nu[1][targ]->Write(get_acc_meanPt2_Nu_histo_name(targ).c_str());
        gROOT->cd();
    }

    // Acceptance-Rad-corrected meanPt2(Nu)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2_Nu[2][targ] = new TH1F("","",N_Nu,Nu_limits);
        
        for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
        {
            q2zh_accrc_integration(h_Pt2_integral, h_Pt2, fin, targ, Nu_bin);

            // Assign content and error
            h_meanPt2_Nu[2][targ]->SetBinContent(Nu_bin+1,h_Pt2_integral->GetMean());
            h_meanPt2_Nu[2][targ]->SetBinError(Nu_bin+1,h_Pt2_integral->GetMeanError());

            // Reset
            h_Pt2_integral->Reset();
            h_Pt2->Reset();
        }
        
        // Write
        fout->cd();
        h_meanPt2_Nu[2][targ]->Write(get_accrc_meanPt2_Nu_histo_name(targ).c_str());
        gROOT->cd();
    }

    // Uncorrected meanPt2(Zh)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2_Zh[0][targ] = new TH1F("","",N_Zh,Zh_limits);
        
        for(int Zh_bin = 0 ; Zh_bin < N_Zh ; Zh_bin++)
        {
            q2nu_integration(h_Pt2_integral, h_Pt2, fin, targ, Zh_bin);

            // Assign content and error
            h_meanPt2_Zh[0][targ]->SetBinContent(Zh_bin+1,h_Pt2_integral->GetMean());
            h_meanPt2_Zh[0][targ]->SetBinError(Zh_bin+1,h_Pt2_integral->GetMeanError());

            // Reset
            h_Pt2_integral->Reset();
            h_Pt2->Reset();
        }
        
        // Write
        fout->cd();
        h_meanPt2_Zh[0][targ]->Write(get_meanPt2_Zh_histo_name(targ).c_str());
        gROOT->cd();
    }

    // Acceptance-corrected meanPt2(Zh)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2_Zh[1][targ] = new TH1F("","",N_Zh,Zh_limits);
        
        for(int Zh_bin = 0 ; Zh_bin < N_Zh ; Zh_bin++)
        {
            q2nu_acc_integration(h_Pt2_integral, h_Pt2, fin, targ, Zh_bin);

            // Assign content and error
            h_meanPt2_Zh[1][targ]->SetBinContent(Zh_bin+1,h_Pt2_integral->GetMean());
            h_meanPt2_Zh[1][targ]->SetBinError(Zh_bin+1,h_Pt2_integral->GetMeanError());

            // Reset
            h_Pt2_integral->Reset();
            h_Pt2->Reset();
        }
        
        // Write
        fout->cd();
        h_meanPt2_Zh[1][targ]->Write(get_acc_meanPt2_Zh_histo_name(targ).c_str());
        gROOT->cd();
    }

    // Acceptance-Rad-corrected meanPt2(Zh)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histo
        h_meanPt2_Zh[2][targ] = new TH1F("","",N_Zh,Zh_limits);
        
        for(int Zh_bin = 0 ; Zh_bin < N_Zh ; Zh_bin++)
        {
            q2nu_accrc_integration(h_Pt2_integral, h_Pt2, fin, targ, Zh_bin);

            // Assign content and error
            h_meanPt2_Zh[2][targ]->SetBinContent(Zh_bin+1,h_Pt2_integral->GetMean());
            h_meanPt2_Zh[2][targ]->SetBinError(Zh_bin+1,h_Pt2_integral->GetMeanError());

            // Reset
            h_Pt2_integral->Reset();
            h_Pt2->Reset();
        }
        
        // Write
        fout->cd();
        h_meanPt2_Zh[2][targ]->Write(get_accrc_meanPt2_Zh_histo_name(targ).c_str());
        gROOT->cd();
    }

    // meanPt2(Q2,Nu,Zh)
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        // Define histos
        for(int i = 0 ; i < 3 ; i++)
        {
            h_meanPt2_Q2Nu[i][targ][0] = new TH1F("","",N_Zh,Zh_limits);
            h_meanPt2_Q2Nu[i][targ][1] = new TH1F("","",N_Zh,Zh_limits);
            h_meanPt2_Q2Nu[i][targ][2] = new TH1F("","",N_Zh,Zh_limits);    
        }

        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
            {
                for(int Zh_bin = 0 ; Zh_bin < N_Zh ; Zh_bin++)
                {
                    // Obtain histos
                    h_Pt2_vars[0][0] = (TH1F*) fin->Get(get_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2_vars[0][1] = (TH1F*) fin->Get(get_clean_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2_vars[0][2] = (TH1F*) fin->Get(get_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    
                    h_Pt2_vars[1][0] = (TH1F*) fin->Get(get_acccorr_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2_vars[1][1] = (TH1F*) fin->Get(get_acccorr_clean_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2_vars[1][2] = (TH1F*) fin->Get(get_acccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());

                    h_Pt2_vars[2][0] = (TH1F*) fin->Get(get_accrccorr_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2_vars[2][1] = (TH1F*) fin->Get(get_accrccorr_clean_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2_vars[2][2] = (TH1F*) fin->Get(get_accrccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());

                    // Assign content and error
                    for(int i = 0 ; i < 3 ; i++)
                    {
                        set_mean_and_meanerror(h_Pt2_vars[i][0], h_meanPt2_Q2Nu[i][targ][0], Zh_bin + 1);
                        set_mean_and_meanerror(h_Pt2_vars[i][1], h_meanPt2_Q2Nu[i][targ][1], Zh_bin + 1);
                        set_mean_and_meanerror(h_Pt2_vars[i][2], h_meanPt2_Q2Nu[i][targ][2], Zh_bin + 1);    
                    }
                }
                
                // Write the histos
                fout->cd();
                h_meanPt2_Q2Nu[0][targ][0]->Write(get_meanPt2_Zh_histo_name(targ,Q2_bin,Nu_bin).c_str());
                h_meanPt2_Q2Nu[0][targ][1]->Write(get_clean_meanPt2_Zh_histo_name(targ,Q2_bin,Nu_bin).c_str());
                h_meanPt2_Q2Nu[0][targ][2]->Write(get_cleaninterpolated_meanPt2_Zh_histo_name(targ,Q2_bin,Nu_bin).c_str());
                
                h_meanPt2_Q2Nu[1][targ][0]->Write(get_acc_meanPt2_Zh_histo_name(targ,Q2_bin,Nu_bin).c_str());
                h_meanPt2_Q2Nu[1][targ][1]->Write(get_acc_clean_meanPt2_Zh_histo_name(targ,Q2_bin,Nu_bin).c_str());
                h_meanPt2_Q2Nu[1][targ][2]->Write(get_acc_cleaninterpolated_meanPt2_Zh_histo_name(targ,Q2_bin,Nu_bin).c_str());
                
                h_meanPt2_Q2Nu[2][targ][0]->Write(get_accrc_meanPt2_Zh_histo_name(targ,Q2_bin,Nu_bin).c_str());
                h_meanPt2_Q2Nu[2][targ][1]->Write(get_accrc_clean_meanPt2_Zh_histo_name(targ,Q2_bin,Nu_bin).c_str());
                h_meanPt2_Q2Nu[2][targ][2]->Write(get_accrc_cleaninterpolated_meanPt2_Zh_histo_name(targ,Q2_bin,Nu_bin).c_str());
                gROOT->cd();

                // Reset histos
                for(int i = 0 ; i < 3 ; i++)
                {
                    h_meanPt2_Q2Nu[i][targ][0]->Reset();
                    h_meanPt2_Q2Nu[i][targ][1]->Reset();
                    h_meanPt2_Q2Nu[i][targ][2]->Reset();    
                }
            }
        }       
    }

    // Close files
    fin->Close();
    fout->Close();

    // Release memory
    delete fin;
    delete fout;

    std::cout<<"Finished averaged squared transverse momentum measurements."<<std::endl;
    
    return 0;
}