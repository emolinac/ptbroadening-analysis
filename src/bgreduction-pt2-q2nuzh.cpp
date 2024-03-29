#include <iostream>
#include "TFile.h"
#include "constants.h"
#include "bgreduction.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    std::cout<<"Starting background reduction process."<<std::endl;
    
    // Open files
    if(!check_file_existence(results_dir, file_name_pt2)) return 1;
    TFile* fin   = new TFile((results_dir+file_name_pt2).c_str());

    TFile* fout1 = new TFile((results_dir+file_name_pt2_bg).c_str(),"RECREATE");
    gROOT->cd();
    TFile* fout2 = new TFile((results_dir+file_name_pt2_fits).c_str(),"RECREATE");
    gROOT->cd();

    // Initialize histos
    // h_...[0] : Untreated histo
    // h_...[1] : Cleaned histo
    // h_...[2] : Cleaned and interpolated histo
    TH1F* h_Pt2[3];
    TH1F* h_meanPt2[3];
    for(int i = 0 ; i < 3 ; i++)
    {
        h_Pt2[i]     = new TH1F("","",N_Pt2,Pt2_min,Pt2_max);
        h_meanPt2[i] = new TH1F("","",N_Zh,Zh_limits);
    }

    // Start processing
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
            {
                for(int Zh_bin = 0 ; Zh_bin < N_Zh ; Zh_bin++)
                {
                    h_Pt2[0] = (TH1F*) fin->Get(get_acccorr_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[1] = (TH1F*) fin->Get(get_acccorr_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[2] = (TH1F*) fin->Get(get_acccorr_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());

                    if(Zh_bin>=4)
                    {
                        // Obtain the cutoff
                        double Pt2_cutoff = obtain_pt2_cutoff(h_Pt2[0], fout2, targets[targ], Q2_bin, Nu_bin, Zh_bin);

                        // Process BG reduction
                        obtain_bgreducted_pt2(h_Pt2[1], Pt2_cutoff);
                        
                        // Process BG+Interpolation plot
                        obtain_bgreducted_pt2(h_Pt2[2], Pt2_cutoff);
                        obtain_interpolated_pt2(h_Pt2[2]);
                    }

                    fout1->cd();
                    h_Pt2[0]->Write(get_acccorr_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[1]->Write(get_acccorr_clean_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[2]->Write(get_acccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    gROOT->cd();
                }// End of Zh loop  
            }// End of Nu loop
        }// End of Q2 loop
    }// End of targets loop

    fin->Close();
    fout1->Close();
    fout2->Close();
    delete fin;
    delete fout1;
    delete fout2;

    std::cout<<"Finished background reduction process."<<std::endl;
    return 1;
}