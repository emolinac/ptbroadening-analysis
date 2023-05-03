#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TROOT.h"
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
    TH1F* h_Pt2[1][3];
    for(int i = 0 ; i < 1 ; i++)
    {
        h_Pt2[0][i] = new TH1F("","",N_Pt2,Pt2_min,Pt2_max);
        h_Pt2[1][i] = new TH1F("","",N_Pt2,Pt2_min,Pt2_max);
        h_Pt2[2][i] = new TH1F("","",N_Pt2,Pt2_min,Pt2_max);
    }

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
                    h_Pt2[0][0] = (TH1F*) fin->Get(get_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[0][1] = (TH1F*) fin->Get(get_acccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[0][2] = (TH1F*) fin->Get(get_accrccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[1][0] = (TH1F*) fin->Get(get_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[1][1] = (TH1F*) fin->Get(get_acccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[1][2] = (TH1F*) fin->Get(get_accrccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    
                    h_Pt2_ratio->Divide(h_Pt2[0][2],h_Pt2[0][1],1,1);


                    // Write the histos
                    fout->cd();
                    h_Pt2[0][0]->Write(get_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[0][1]->Write(get_acccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2[0][2]->Write(get_accrccorr_cleaninterpolated_Pt2_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
                    h_Pt2_ratio->Write(get_Pt2_ratio_histo_name(targ,Q2_bin,Nu_bin,Zh_bin).c_str());
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