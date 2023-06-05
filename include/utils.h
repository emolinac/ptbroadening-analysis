#ifndef UTILS_H
#define UTILS_H

#include "TH1F.h"
#include "TF1.h"
#include "TMath.h"
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
int get_first_empty_bin(TH1F* h)
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

// Scans histo and returns the first uncorrected bin it founds (valid for acc + rc analysis)
int get_first_uncorr_bin(TH1F* h)
{
    for(Int_t bin = 1 ; bin <= h->GetNbinsX() ; bin++)
    {
        if(h->GetBinContent(bin)==1)
        {
            return bin;
        }
    }

    return -1;
}

// Set the parameters of the best fit to function
void set_best_fit_parameters(TF1* function, TH1F* h)
{
    // Determine number of differen fits
    const int n_fits = sizeof(fit_options)/sizeof(std::string);

    // Declare array that stores how close to one iis the goodness of fit
    double discriminator[n_fits];

    // Fit
    for(int i = 0 ; i < n_fits ; i++)
    {
        h->Fit(function, (fit_options[i]).c_str());   
        discriminator[i] = TMath::Abs((function->GetChisquare()/function->GetNDF()) - 1);
    }

    // Determine which fit is the best
    int best_fit_index = TMath::LocMin(n_fits, discriminator);

    // Set the function with the parameters of the best fit
    h->Fit(function, (fit_options[best_fit_index]).c_str());

    return;
}

// Corrects the RC, as a function of Pt2, 
void correct_rc_pt2_behavior(TF1* function, TH1F* h, int last_corr_bin)
{
    for(int Pt2_bin = 1 ; Pt2_bin <= last_corr_bin ; Pt2_bin++)
    {
        double function_value = function->Eval(h->GetBinCenter(Pt2_bin));
        double ratio_content  = h->GetBinContent(Pt2_bin);
        double difference     = 100*TMath::Abs(function_value-ratio_content)/function_value;
        if(difference<difference_cutoff) continue;
        else
        {
            h->SetBinContent(Pt2_bin,function_value);
        }
    }

    return;
}

void remove_big_rc_corr(TH1F* h, int last_corr_bin)
{
    double min_corr = 0.6;
    double max_corr = 1.4;

    for (int Pt2_bin = 1 ; Pt2_bin <= last_corr_bin ; Pt2_bin++)
    {
        if(h->GetBinContent(Pt2_bin)<min_corr||h->GetBinContent(Pt2_bin)>max_corr)
        {
            double sum = 0;
            double N   = 0;

            // Calculate average of neighbouring N points
            const int lim = TMath::Min(Pt2_bin + 2, last_corr_bin);
            for(int i = Pt2_bin - 2 ; i < lim ; i++)
            {
                if(h->GetBinContent(i)>min_corr&&h->GetBinContent(i)<max_corr)
                {
                    sum += h->GetBinContent(i);
                    N++;
                }
            }

            if(N>0) h->SetBinContent(Pt2_bin, sum/N);
        }
    }

    return;
}


// Uses the last RC corrected bin and assigns the rest of the tail that correction
void assign_last_rc_to_tail(TH1F* h, int last_corr_bin, int first_empty_bin)
{
    for(int Pt2_bin = last_corr_bin+1 ; Pt2_bin < first_empty_bin ; Pt2_bin++)
    {
        h->SetBinContent(Pt2_bin, h->GetBinContent(Pt2_bin-1));
    }

    return;
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

void get_neighbouring_histos(TH1F* h_Pt2_neighbours[3][3][3][2], TH1F* h_Pt2_ratio_neighbours[3][3][3], TFile* fin, int targ, int Q2_bin, int Nu_bin, int Zh_bin)
{
    for(int i = Q2_bin-1 ; i <= Q2_bin+1 ; i++)
    {
        for(int j = Nu_bin-1 ; j <= Nu_bin+1 ; j++)
        {
            for(int k = Zh_bin-1 ; k <= Zh_bin+1 ; k++)
            {

                h_Pt2_neighbours[i-(Q2_bin-1)][j-(Nu_bin-1)][k-(Zh_bin-1)][0] = (TH1F*) fin->Get(get_acccorr_cleaninterpolated_Pt2_histo_name(targ,i,j,k).c_str());
                h_Pt2_neighbours[i-(Q2_bin-1)][j-(Nu_bin-1)][k-(Zh_bin-1)][1] = (TH1F*) fin->Get(get_accrccorr_cleaninterpolated_Pt2_histo_name(targ,i,j,k).c_str());
                
                if(h_Pt2_neighbours[i-(Q2_bin-1)][j-(Nu_bin-1)][k-(Zh_bin-1)][0]==NULL||(i==Q2_bin&&j==Nu_bin&&k==Zh_bin))
                {
                    h_Pt2_neighbours[i-(Q2_bin-1)][j-(Nu_bin-1)][k-(Zh_bin-1)][0]    = new TH1F("","",N_Pt2, Pt2_min, Pt2_max);
                    h_Pt2_neighbours[i-(Q2_bin-1)][j-(Nu_bin-1)][k-(Zh_bin-1)][1]    = new TH1F("","",N_Pt2, Pt2_min, Pt2_max);
                    h_Pt2_ratio_neighbours[i-(Q2_bin-1)][j-(Nu_bin-1)][k-(Zh_bin-1)] = new TH1F("","",N_Pt2, Pt2_min, Pt2_max);

                    continue;
                }

                h_Pt2_ratio_neighbours[i-(Q2_bin-1)][j-(Nu_bin-1)][k-(Zh_bin-1)] = new TH1F("","",N_Pt2, Pt2_min, Pt2_max);
                h_Pt2_ratio_neighbours[i-(Q2_bin-1)][j-(Nu_bin-1)][k-(Zh_bin-1)]->Divide(h_Pt2_neighbours[i-(Q2_bin-1)][j-(Nu_bin-1)][k-(Zh_bin-1)][1],
                                                                                         h_Pt2_neighbours[i-(Q2_bin-1)][j-(Nu_bin-1)][k-(Zh_bin-1)][0],1,1);
            }
        }
    }

    return;
}

double get_average_rccorr_impact(TH1F* h_Pt2_ratio_neighbours[3][3][3], int Q2_bin, int Nu_bin, int Zh_bin, int Pt2_bin)
{
    double sum = 0;
    double N = 0;

    for(int i = 0 ; i < 3 ; i++)
    {
        for(int j = 0 ; j < 3 ; j++)
        {
            for(int k = 0 ; k < 3 ; k++)
            {
                double content_n  = h_Pt2_ratio_neighbours[i][j][k]->GetBinContent(Pt2_bin-1);
                double content_n1 = h_Pt2_ratio_neighbours[i][j][k]->GetBinContent(Pt2_bin);
                double content_n2 = h_Pt2_ratio_neighbours[i][j][k]->GetBinContent(Pt2_bin+1);

                if(content_n !=0){sum += content_n;  N++;}
                if(content_n1!=0){sum += content_n1; N++;}
                if(content_n2!=0){sum += content_n2; N++;}
            }
        }
    }

    return sum/N;
}

#endif