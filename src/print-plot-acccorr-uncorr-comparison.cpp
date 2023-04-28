#include <iostream>
#include "TFile.h"

#include "utils.h"

int main(int argc, char* argv[])
{
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
        hmeanpt2[0][targ] = (TH1F*) fmeanpt2->Get(get_meanPt2_Zh_histo_name(targ).c_str());
        hmeanpt2[1][targ] = (TH1F*) fmeanpt2->Get(get_acc_meanPt2_Zh_histo_name(targ).c_str());
    }

    //// broadening
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        hbroad[0][targ] = (TH1F*) fbroad->Get(get_broadening_Zh_histo_name(targ+3).c_str());
        hbroad[1][targ] = (TH1F*) fbroad->Get(get_acc_broadening_Zh_histo_name(targ+3).c_str());
    }

    // Calculate the ratios to know the impact of the desired correction
    TH1F* hmeanpt2_ratio[N_targets];
    TH1F* hbroad_ratio[N_broadening];

    //// meanpt2
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        hmeanpt2_ratio[targ]->Divide(hmeanpt2[1][targ],hmeanpt2[0][targ],1,1);
    }

    //// broadening
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        hbroad_ratio[targ]->Divide(hbroad[1][targ],hbroad[0][targ],1,1);
    }

}