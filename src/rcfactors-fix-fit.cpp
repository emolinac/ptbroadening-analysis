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
                    const int    last_corr_bin = get_first_uncorr_bin(h_Pt2_ratio) - 1;
                    const double Pt2_cutoff    = delta_Pt2*last_corr_bin;

                    TF1* fit_func = new TF1("fit_func","[0]+[1]*TMath::Cos(x)",0,Pt2_cutoff);
                    //TF1* fit_func = new TF1("fit_func","[0]+TMath::Exp(x*[1])",0,Pt2_cutoff);

                    h_Pt2_ratio->Fit("fit_func","RSEQ");

                    if(fit_func->GetChisquare()/fit_func->GetNDF()<2)
                    {
                        for(int Pt2_bin = 1 ; Pt2_bin <= last_corr_bin ; Pt2_bin++)
                        {
                            double fit_func_value = fit_func->Eval(h_Pt2_ratio->GetBinCenter(Pt2_bin));
                            double ratio_content  = h_Pt2_ratio->GetBinContent(Pt2_bin);

                            double difference = 100*TMath::Abs(fit_func_value-ratio_content)/fit_func_value;

                            if(difference<difference_cutoff) continue;
                            else
                            {
                                h_Pt2_ratio->SetBinContent(Pt2_bin,fit_func_value);
                            }
                        }
                    }
                    else
                    {
                        h_Pt2_ratio->Fit("fit_func","RSEQBL");
                        
                        for(int Pt2_bin = 1 ; Pt2_bin <= last_corr_bin ; Pt2_bin++)
                        {
                            double fit_func_value = fit_func->Eval(h_Pt2_ratio->GetBinCenter(Pt2_bin));
                            double ratio_content  = h_Pt2_ratio->GetBinContent(Pt2_bin);

                            double difference = 100*TMath::Abs(fit_func_value-ratio_content)/fit_func_value;

                            if(difference<difference_cutoff) continue;
                            else
                            {
                                h_Pt2_ratio->SetBinContent(Pt2_bin,fit_func_value);
                            }
                        }
                    }
                 
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
