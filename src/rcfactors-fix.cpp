#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TMath.h"
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
    TH1F* h_Pt2_neighbours[3][3][3][2];
    TH1F* h_Pt2_ratio_neighbours[3][3][3];

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

                    // Write the histos
                    fout->cd();
                    h_Pt2_ratio->Write(("original_"+get_Pt2_ratio_histo_name(targ,Q2_bin,Nu_bin,Zh_bin)).c_str());
                    gROOT->cd();

                    get_neighbouring_histos(h_Pt2_neighbours, h_Pt2_ratio_neighbours, fin, targ, Q2_bin, Nu_bin, Zh_bin);
                    
                    std::cout<<"Neighbouring histos for bin "<<Q2_bin<<Nu_bin<<Zh_bin<<" obtained!"<<std::endl;

                    const int first_empty_bin = get_first_empty_bin(h_Pt2_ratio);

                    for(int Pt2_bin = 1 ; Pt2_bin < first_empty_bin-1 ; Pt2_bin++)
                    {
                        // Obtain local content and the content of the next bin
                        double content_n1 = h_Pt2_ratio->GetBinContent(Pt2_bin);
                        double content_n2 = h_Pt2_ratio->GetBinContent(Pt2_bin+1);

                        // Calculate the absolute porcentual difference between them
                        double difference = 100*TMath::Abs(content_n1 - content_n2)/content_n1;

                        // If the difference is small then skip this. If not then process the bin
                        if(difference < difference_cutoff) continue;
                        else
                        {
                            double avge_impact = get_average_rccorr_impact(h_Pt2_ratio_neighbours, Q2_bin, Nu_bin, Zh_bin, Pt2_bin);
                            h_Pt2_ratio->SetBinContent(Pt2_bin, avge_impact);
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