#ifndef UTILS_H
#define UTILS_H

#include "TH1F.h"
#include "TSystem.h"
#include "constants.h"

// Check file existence
int check_file_existence(std::string dir, std::string file_name)
{
    if(gSystem->AccessPathName((dir+file_name).c_str()))
    {
        std::cout<<file_name<<" not found!"<<std::endl;
        std::cout<<"Check in "<<dir<<" for "<<file_name<<std::endl;
        return 0;
    }

    return 1;
}

// Returns 1 if the histogram is empty.
int empty_histo(TH1F* h)
{
    int empty  = 0;
    int result = 0;
    for(Int_t i = 1 ; i <= h->GetNbinsX() ; i++)
    {
        if(h->GetBinContent(i)==0) empty++;
    }
    result = (empty==h->GetNbinsX()) ? 1 : 0;
    return result;
}

// Scans histo and returns the first empty bin it founds. If there are no empty bins a -1 value is returned.
int first_empty_bin(TH1F* h)
{
    for(Int_t bin = 1 ; bin <= h->GetNbinsX() ; bin++)
    {
        if(h->GetBinContent(bin)==0)
        {
            return bin;
        }
    }

    return -1;
}

// Use for meanPt2 measurements.
void set_mean_and_meanerror(TH1F* h_Pt2, TH1F* h_meanPt2, int bin)
{
    if(h_Pt2==NULL)
    {
        h_meanPt2->SetBinContent(bin, 0);
        h_meanPt2->SetBinError(bin, 0);
    }

    h_meanPt2->SetBinContent(bin, h_Pt2->GetMean());
    h_meanPt2->SetBinError(bin, h_Pt2->GetMeanError());

    return;
}

std::string get_accf_histo_name(int target_index, int Q2_bin, int Nu_bin, int Zh_bin, int Pt2_bin)
{
    return histo_accf+targets[target_index]+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+std::to_string(Pt2_bin);
}

std::string get_data_histo_name(int target_index, int Q2_bin, int Nu_bin, int Zh_bin, int Pt2_bin)
{
    return histo_data+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+std::to_string(Pt2_bin);
}

std::string get_acccorr_Phi_histo_name(int target_index, int Q2_bin, int Nu_bin, int Zh_bin, int Pt2_bin)
{
    return histo_acc+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+std::to_string(Pt2_bin);
}

std::string get_accrccorr_Phi_histo_name(int target_index, int Q2_bin, int Nu_bin, int Zh_bin, int Pt2_bin)
{
    return histo_accrc+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+std::to_string(Pt2_bin);
}

std::string get_Pt2_ratio_histo_name(int target_index, int Q2_bin, int Nu_bin, int Zh_bin)
{
    return "Pt2_acc_accrc_ratio_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin);
}

std::string get_Pt2_histo_name(int target_index, int Q2_bin, int Nu_bin, int Zh_bin)
{
    return histo_data+"Pt2_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin);
}

std::string get_clean_Pt2_histo_name(int target_index, int Q2_bin, int Nu_bin, int Zh_bin)
{
    return histo_data+"Pt2_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+"_clean";
}

std::string get_cleaninterpolated_Pt2_histo_name(int target_index, int Q2_bin, int Nu_bin, int Zh_bin)
{
    return histo_data+"Pt2_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+"_clean_interpolated";
}

std::string get_acccorr_Pt2_histo_name(int target_index, int Q2_bin, int Nu_bin, int Zh_bin)
{
    return histo_acc+"Pt2_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin);
}

std::string get_acccorr_clean_Pt2_histo_name(int target_index, int Q2_bin, int Nu_bin, int Zh_bin)
{
    return histo_acc+"Pt2_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+"_clean";
}

std::string get_acccorr_cleaninterpolated_Pt2_histo_name(int target_index, int Q2_bin, int Nu_bin, int Zh_bin)
{
    return histo_acc+"Pt2_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+"_clean_interpolated";
}

std::string get_accrccorr_Pt2_histo_name(int target_index, int Q2_bin, int Nu_bin, int Zh_bin)
{
    return histo_accrc+"Pt2_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin);
}

std::string get_accrccorr_clean_Pt2_histo_name(int target_index, int Q2_bin, int Nu_bin, int Zh_bin)
{
    return histo_accrc+"Pt2_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+"_clean";
}

std::string get_accrccorr_cleaninterpolated_Pt2_histo_name(int target_index, int Q2_bin, int Nu_bin, int Zh_bin)
{
    return histo_accrc+"Pt2_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+"_clean_interpolated";
}

// UNCORR

std::string get_meanPt2_histo_name(int target_index)
{
    return "meanPt2_"+targets[target_index];
}

std::string get_meanPt2_Q2_histo_name(int target_index)
{
    return "meanPt2_Q2_"+targets[target_index];
}

std::string get_meanPt2_Nu_histo_name(int target_index)
{
    return "meanPt2_Nu_"+targets[target_index];
}

std::string get_meanPt2_Zh_histo_name(int target_index)
{
    return "meanPt2_Zh_"+targets[target_index];
}

std::string get_meanPt2_Zh_histo_name(int target_index, int Q2_bin, int Nu_bin)
{
    return "meanPt2_Zh_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin);
}

std::string get_clean_meanPt2_Zh_histo_name(int target_index, int Q2_bin, int Nu_bin)
{
    return "meanPt2_Zh_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+"_clean";
}

std::string get_cleaninterpolated_meanPt2_Zh_histo_name(int target_index, int Q2_bin, int Nu_bin)
{
    return "meanPt2_Zh_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+"_clean_interpolated";
}

std::string get_broadening_histo_name(int target_index)
{
    return "broadening_"+targets[target_index];
}

std::string get_broadening_Q2_histo_name(int target_index)
{
    return "broadening_Q2_"+targets[target_index];
}

std::string get_broadening_Nu_histo_name(int target_index)
{
    return "broadening_Nu_"+targets[target_index];
}

std::string get_broadening_Zh_histo_name(int target_index)
{
    return "broadening_Zh_"+targets[target_index];
}

std::string get_broadening_Zh_histo_name(int target_index, int Q2_bin, int Nu_bin)
{
    return "broadening_Zh_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin);
}

// ACC CORR

std::string get_acc_meanPt2_histo_name(int target_index)
{
    return "acc_meanPt2_"+targets[target_index];
}

std::string get_acc_meanPt2_Q2_histo_name(int target_index)
{
    return "acc_meanPt2_Q2_"+targets[target_index];
}

std::string get_acc_meanPt2_Nu_histo_name(int target_index)
{
    return "acc_meanPt2_Nu_"+targets[target_index];
}

std::string get_acc_meanPt2_Zh_histo_name(int target_index)
{
    return "acc_meanPt2_Zh_"+targets[target_index];
}

std::string get_acc_meanPt2_Zh_histo_name(int target_index, int Q2_bin, int Nu_bin)
{
    return "acc_meanPt2_Zh_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin);
}

std::string get_acc_clean_meanPt2_Zh_histo_name(int target_index, int Q2_bin, int Nu_bin)
{
    return "acc_meanPt2_Zh_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+"_clean";
}

std::string get_acc_cleaninterpolated_meanPt2_Zh_histo_name(int target_index, int Q2_bin, int Nu_bin)
{
    return "acc_meanPt2_Zh_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+"_clean_interpolated";
}

std::string get_acc_broadening_histo_name(int target_index)
{
    return "acc_broadening_"+targets[target_index];
}

std::string get_acc_broadening_Q2_histo_name(int target_index)
{
    return "acc_broadening_Q2_"+targets[target_index];
}

std::string get_acc_broadening_Nu_histo_name(int target_index)
{
    return "acc_broadening_Nu_"+targets[target_index];
}

std::string get_acc_broadening_Zh_histo_name(int target_index)
{
    return "acc_broadening_Zh_"+targets[target_index];
}

std::string get_acc_broadening_Zh_histo_name(int target_index, int Q2_bin, int Nu_bin)
{
    return "acc_broadening_Zh_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin);
}

// ACC + RC CORR

std::string get_accrc_meanPt2_histo_name(int target_index)
{
    return "accrc_meanPt2_"+targets[target_index];
}

std::string get_accrc_meanPt2_Q2_histo_name(int target_index)
{
    return "accrc_meanPt2_Q2_"+targets[target_index];
}

std::string get_accrc_meanPt2_Nu_histo_name(int target_index)
{
    return "accrc_meanPt2_Nu_"+targets[target_index];
}

std::string get_accrc_meanPt2_Zh_histo_name(int target_index)
{
    return "accrc_meanPt2_Zh_"+targets[target_index];
}

std::string get_accrc_meanPt2_Zh_histo_name(int target_index, int Q2_bin, int Nu_bin)
{
    return "accrc_meanPt2_Zh_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin);
}

std::string get_accrc_clean_meanPt2_Zh_histo_name(int target_index, int Q2_bin, int Nu_bin)
{
    return "accrc_meanPt2_Zh_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+"_clean";
}

std::string get_accrc_cleaninterpolated_meanPt2_Zh_histo_name(int target_index, int Q2_bin, int Nu_bin)
{
    return "accrc_meanPt2_Zh_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+"_clean_interpolated";
}

std::string get_accrc_broadening_histo_name(int target_index)
{
    return "accrc_broadening_"+targets[target_index];
}

std::string get_accrc_broadening_Q2_histo_name(int target_index)
{
    return "accrc_broadening_Q2_"+targets[target_index];
}

std::string get_accrc_broadening_Nu_histo_name(int target_index)
{
    return "accrc_broadening_Nu_"+targets[target_index];
}

std::string get_accrc_broadening_Zh_histo_name(int target_index)
{
    return "accrc_broadening_Zh_"+targets[target_index];
}

std::string get_accrc_broadening_Zh_histo_name(int target_index, int Q2_bin, int Nu_bin)
{
    return "accrc_broadening_Zh_"+targets[target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin);
}

#endif