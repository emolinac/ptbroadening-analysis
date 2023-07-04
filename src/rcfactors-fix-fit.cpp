#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TMath.h"
#include "TF1.h"
#include "utils.h"
#include "constants.h"

int main(int argc, char* argv[])
{
    // Open bg treated file
    if(!check_file_existence(results_dir, file_name_pt2_bg)) return 1;
    TFile* fin  = new TFile((results_dir+file_name_pt2_bg).c_str());

    TFile* fout = new TFile((results_dir+"rcfix_"+file_name_pt2_bg).c_str(),"RECREATE");
    gROOT->cd();

    // Declare instrumental histos
    TH1F* h_Pt2[2][3];
    TH1F* h_Pt2_bg[2][3];
    
    TH1F* h_Pt2_ratio = new TH1F("","",N_Pt2,Pt2_min,Pt2_max);

    // Start processing
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
            {
                for(int Zh_bin = 0 ; Zh_bin < N_Zh ; Zh_bin++)
                {
                    // Obtain the histos twice and modify the second
                    h_Pt2_bg[0][0] = (TH1F*) fin->Get(get_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2_bg[0][1] = (TH1F*) fin->Get(get_acccorr_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2_bg[0][2] = (TH1F*) fin->Get(get_accrccorr_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2_bg[1][0] = (TH1F*) fin->Get(get_clean_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2_bg[1][1] = (TH1F*) fin->Get(get_acccorr_clean_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2_bg[1][2] = (TH1F*) fin->Get(get_accrccorr_clean_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    
                    h_Pt2[0][0] = (TH1F*) fin->Get(get_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[0][1] = (TH1F*) fin->Get(get_acccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[0][2] = (TH1F*) fin->Get(get_accrccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[1][0] = (TH1F*) fin->Get(get_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[1][1] = (TH1F*) fin->Get(get_acccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[1][2] = (TH1F*) fin->Get(get_accrccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    
                    h_Pt2_ratio->Divide(h_Pt2[0][2],h_Pt2[0][1],1,1);

                    // Write the original ratio
                    fout->cd();
                    h_Pt2_ratio->Write(("original_"+get_Pt2_ratio_histo_name(targ,Q2_bin,Nu_bin,Zh_bin)).c_str());
                    gROOT->cd();

                    // Declare the fit function and fit!
                    int last_rccorr_bin = get_first_uncorr_bin(h_Pt2_ratio) - 1;
                    int first_empty_bin = get_first_empty_bin(h_Pt2_ratio);

                    // If bin is RC uncorrected, skip it
                    if(last_rccorr_bin==0||Zh_bin<2)
                    {
                        // Write the histos
                        fout->cd();
                        h_Pt2_ratio->Write(get_Pt2_ratio_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                        h_Pt2[0][0]->Write(get_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                        h_Pt2[0][1]->Write(get_acccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                        h_Pt2[1][1]->Write(get_accrccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                        h_Pt2_bg[0][0]->Write((get_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str()));
                        h_Pt2_bg[0][1]->Write((get_acccorr_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str()));
                        h_Pt2_bg[0][2]->Write((get_accrccorr_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str()));
                        h_Pt2_bg[1][0]->Write((get_clean_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str()));
                        h_Pt2_bg[1][1]->Write((get_acccorr_clean_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str()));
                        h_Pt2_bg[1][2]->Write((get_accrccorr_clean_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str()));
                        gROOT->cd();

                        h_Pt2_ratio->Reset();
                        continue;
                    }

                    // If bin completely corrected then last_rccorr_bin==-2
                    int    bin_cutoff = (last_rccorr_bin==-2) ? first_empty_bin : last_rccorr_bin;
                    double Pt2_cutoff = delta_Pt2*bin_cutoff;

                    // Check if there is an oddly big correction and correct it
                    if(Zh_bin>1) remove_big_rc_corr(h_Pt2_ratio, bin_cutoff);

                    //// Calculate base correction and use it as ref value to fix odd corrections
                    //double base_corr = get_base_corr(h_Pt2_ratio, bin_cutoff);
                    //if(Zh_bin>1) remove_big_rc_corr(h_Pt2_ratio, bin_cutoff, base_corr);

                    fout->cd();
                    h_Pt2_ratio->Write(("nobigcorr_"+get_Pt2_ratio_histo_name(targ,Q2_bin,Nu_bin,Zh_bin)).c_str());
                    gROOT->cd();

                    // Define fitting function
                    TF1* fit_func = new TF1("fit_func","[0]+[1]*TMath::Cos(x)",delta_Pt2*2,Pt2_cutoff);

                    // Set the parameters of the best fit to the fitting function
                    set_best_fit_parameters(fit_func, h_Pt2_ratio);

                    // Correct the RC curve
                    correct_rc_pt2_behavior(fit_func, h_Pt2_ratio, bin_cutoff);

                    // Assign last rc corrected bin to the rest of Pt2 tail
                    if(Zh_bin>4&&bin_cutoff<first_empty_bin) assign_last_rc_to_tail(h_Pt2_ratio, last_rccorr_bin, first_empty_bin);

                    // Assigns a stabilized RC correction to the Pt2 distribution
                    h_Pt2[1][1]->Multiply(h_Pt2_ratio);

                    // Write the histos
                    fout->cd();
                    h_Pt2_ratio->Write(get_Pt2_ratio_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[0][0]->Write(get_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[0][1]->Write(get_acccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[1][1]->Write(get_accrccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2_bg[0][0]->Write((get_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str()));
                    h_Pt2_bg[0][1]->Write((get_acccorr_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str()));
                    h_Pt2_bg[0][2]->Write((get_accrccorr_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str()));
                    h_Pt2_bg[1][0]->Write((get_clean_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str()));
                    h_Pt2_bg[1][1]->Write((get_acccorr_clean_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str()));
                    h_Pt2_bg[1][2]->Write((get_accrccorr_clean_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str()));
                    gROOT->cd();

                    h_Pt2_ratio->Reset();
                }
            }
        }
    }

    //Close files
    fin->Close();
    fout->Close();

    return 1;
}
