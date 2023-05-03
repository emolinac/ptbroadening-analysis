#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "constants.h"
#include "integrals.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    std::cout<<"Starting broadening measurements."<<std::endl;

    // Open files
    if(!check_file_existence(results_dir, file_name_meanpt2)) return 1;
    TFile* fin = new TFile((results_dir+file_name_meanpt2).c_str());
    
    TFile* fout = new TFile((results_dir+file_name_broad).c_str(),"RECREATE");
    gROOT->cd();

    // Instrumental histos
    // Index : 0 (liquid), 1 (solid)
    TH1F* h_meanPt2[3][2];
    TH1F* h_meanPt2_Q2[3][2];
    TH1F* h_meanPt2_Nu[3][2];
    TH1F* h_meanPt2_Zh[3][2];
    TH1F* h_meanPt2_Q2Nu[3][2];
    
    // Result histos
    TH1F* h_broadening[3][N_broadening];
    TH1F* h_broadening_Q2[3][N_broadening];
    TH1F* h_broadening_Nu[3][N_broadening];
    TH1F* h_broadening_Zh[3][N_broadening];
    TH1F* h_broadening_Q2Nu[3][N_broadening][N_Q2][N_Nu];

    // Total broadening (used for A13 dependence)
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        // Define broadening histo
        h_broadening[0][targ] = new TH1F("","",1,0,1); 
        h_broadening[1][targ] = new TH1F("","",1,0,1); 
        h_broadening[2][targ] = new TH1F("","",1,0,1); 

        // Obtain histos
        h_meanPt2[0][0] = (TH1F*) fin->Get(get_meanPt2_histo_name(targ).c_str());
        h_meanPt2[0][1] = (TH1F*) fin->Get(get_meanPt2_histo_name(targ+3).c_str());
        h_meanPt2[1][0] = (TH1F*) fin->Get(get_acc_meanPt2_histo_name(targ).c_str());
        h_meanPt2[1][1] = (TH1F*) fin->Get(get_acc_meanPt2_histo_name(targ+3).c_str());
        h_meanPt2[2][0] = (TH1F*) fin->Get(get_accrc_meanPt2_histo_name(targ).c_str());
        h_meanPt2[2][1] = (TH1F*) fin->Get(get_accrc_meanPt2_histo_name(targ+3).c_str());

        // Calculate broadening
        h_broadening[0][targ]->Add(h_meanPt2[0][1],h_meanPt2[0][0],1,-1);
        h_broadening[1][targ]->Add(h_meanPt2[1][1],h_meanPt2[1][0],1,-1);
        h_broadening[2][targ]->Add(h_meanPt2[2][1],h_meanPt2[2][0],1,-1);

        // Write
        fout->cd();
        h_broadening[0][targ]->Write(get_broadening_histo_name(targ+3).c_str());
        h_broadening[1][targ]->Write(get_acc_broadening_histo_name(targ+3).c_str());
        h_broadening[2][targ]->Write(get_accrc_broadening_histo_name(targ+3).c_str());
        gROOT->cd();

        // Reset
        h_meanPt2[0][0]->Reset();
        h_meanPt2[0][1]->Reset();
        h_meanPt2[1][0]->Reset();
        h_meanPt2[1][1]->Reset();
        h_meanPt2[2][0]->Reset();
        h_meanPt2[2][1]->Reset();
    }

    // Broadening (Q2)
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        // Define broadening histo
        h_broadening_Q2[0][targ] = new TH1F("","",N_Q2,Q2_limits);
        h_broadening_Q2[1][targ] = new TH1F("","",N_Q2,Q2_limits);
        h_broadening_Q2[2][targ] = new TH1F("","",N_Q2,Q2_limits);

        // Obtain histos
        h_meanPt2_Q2[0][0] = (TH1F*) fin->Get(get_meanPt2_Q2_histo_name(targ).c_str());
        h_meanPt2_Q2[0][1] = (TH1F*) fin->Get(get_meanPt2_Q2_histo_name(targ+3).c_str());
        h_meanPt2_Q2[1][0] = (TH1F*) fin->Get(get_acc_meanPt2_Q2_histo_name(targ).c_str());
        h_meanPt2_Q2[1][1] = (TH1F*) fin->Get(get_acc_meanPt2_Q2_histo_name(targ+3).c_str());
        h_meanPt2_Q2[2][0] = (TH1F*) fin->Get(get_accrc_meanPt2_Q2_histo_name(targ).c_str());
        h_meanPt2_Q2[2][1] = (TH1F*) fin->Get(get_accrc_meanPt2_Q2_histo_name(targ+3).c_str());

        // Calculate broadening
        h_broadening_Q2[0][targ]->Add(h_meanPt2_Q2[0][1],h_meanPt2_Q2[0][0],1,-1);
        h_broadening_Q2[1][targ]->Add(h_meanPt2_Q2[1][1],h_meanPt2_Q2[1][0],1,-1);
        h_broadening_Q2[2][targ]->Add(h_meanPt2_Q2[2][1],h_meanPt2_Q2[2][0],1,-1);

        // Write
        fout->cd();
        h_broadening_Q2[0][targ]->Write(get_broadening_Q2_histo_name(targ+3).c_str());
        h_broadening_Q2[1][targ]->Write(get_acc_broadening_Q2_histo_name(targ+3).c_str());
        h_broadening_Q2[2][targ]->Write(get_accrc_broadening_Q2_histo_name(targ+3).c_str());
        gROOT->cd();

        // Reset
        h_meanPt2_Q2[0][0]->Reset(); 
        h_meanPt2_Q2[0][1]->Reset();
        h_meanPt2_Q2[1][0]->Reset(); 
        h_meanPt2_Q2[1][1]->Reset();
        h_meanPt2_Q2[2][0]->Reset(); 
        h_meanPt2_Q2[2][1]->Reset();
    }

    // Broadening (Nu)
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        // Define broadening histo
        h_broadening_Nu[0][targ] = new TH1F("","",N_Nu,Nu_limits);
        h_broadening_Nu[1][targ] = new TH1F("","",N_Nu,Nu_limits);
        h_broadening_Nu[2][targ] = new TH1F("","",N_Nu,Nu_limits);

        // Obtain histos
        h_meanPt2_Nu[0][0] = (TH1F*) fin->Get(get_meanPt2_Nu_histo_name(targ).c_str());
        h_meanPt2_Nu[0][1] = (TH1F*) fin->Get(get_meanPt2_Nu_histo_name(targ+3).c_str());
        h_meanPt2_Nu[1][0] = (TH1F*) fin->Get(get_acc_meanPt2_Nu_histo_name(targ).c_str());
        h_meanPt2_Nu[1][1] = (TH1F*) fin->Get(get_acc_meanPt2_Nu_histo_name(targ+3).c_str());
        h_meanPt2_Nu[2][0] = (TH1F*) fin->Get(get_accrc_meanPt2_Nu_histo_name(targ).c_str());
        h_meanPt2_Nu[2][1] = (TH1F*) fin->Get(get_accrc_meanPt2_Nu_histo_name(targ+3).c_str());

        // Calculate broadening
        h_broadening_Nu[0][targ]->Add(h_meanPt2_Nu[0][1],h_meanPt2_Nu[0][0],1,-1);
        h_broadening_Nu[1][targ]->Add(h_meanPt2_Nu[1][1],h_meanPt2_Nu[1][0],1,-1);
        h_broadening_Nu[2][targ]->Add(h_meanPt2_Nu[2][1],h_meanPt2_Nu[2][0],1,-1);

        // Write
        fout->cd();
        h_broadening_Nu[0][targ]->Write(get_broadening_Nu_histo_name(targ+3).c_str());
        h_broadening_Nu[1][targ]->Write(get_acc_broadening_Nu_histo_name(targ+3).c_str());
        h_broadening_Nu[2][targ]->Write(get_accrc_broadening_Nu_histo_name(targ+3).c_str());
        gROOT->cd();

        // Reset
        h_meanPt2_Nu[0][0]->Reset();
        h_meanPt2_Nu[0][1]->Reset();
        h_meanPt2_Nu[1][0]->Reset();
        h_meanPt2_Nu[1][1]->Reset();
        h_meanPt2_Nu[2][0]->Reset();
        h_meanPt2_Nu[2][1]->Reset();
    }

    // Broadening (Zh)
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        // Define broadening histo
        h_broadening_Zh[0][targ] = new TH1F("","",N_Zh,Zh_limits);
        h_broadening_Zh[1][targ] = new TH1F("","",N_Zh,Zh_limits);
        h_broadening_Zh[2][targ] = new TH1F("","",N_Zh,Zh_limits);

        // Obtain histos
        h_meanPt2_Zh[0][0] = (TH1F*) fin->Get(get_meanPt2_Zh_histo_name(targ).c_str());
        h_meanPt2_Zh[0][1] = (TH1F*) fin->Get(get_meanPt2_Zh_histo_name(targ+3).c_str());
        h_meanPt2_Zh[1][0] = (TH1F*) fin->Get(get_acc_meanPt2_Zh_histo_name(targ).c_str());
        h_meanPt2_Zh[1][1] = (TH1F*) fin->Get(get_acc_meanPt2_Zh_histo_name(targ+3).c_str());
        h_meanPt2_Zh[2][0] = (TH1F*) fin->Get(get_accrc_meanPt2_Zh_histo_name(targ).c_str());
        h_meanPt2_Zh[2][1] = (TH1F*) fin->Get(get_accrc_meanPt2_Zh_histo_name(targ+3).c_str());

        // Calculate broadening
        h_broadening_Zh[0][targ]->Add(h_meanPt2_Zh[0][1],h_meanPt2_Zh[0][0],1,-1);
        h_broadening_Zh[1][targ]->Add(h_meanPt2_Zh[1][1],h_meanPt2_Zh[1][0],1,-1);
        h_broadening_Zh[2][targ]->Add(h_meanPt2_Zh[2][1],h_meanPt2_Zh[2][0],1,-1);

        // Write
        fout->cd();
        h_broadening_Zh[0][targ]->Write(get_broadening_Zh_histo_name(targ+3).c_str());
        h_broadening_Zh[1][targ]->Write(get_acc_broadening_Zh_histo_name(targ+3).c_str());
        h_broadening_Zh[2][targ]->Write(get_accrc_broadening_Zh_histo_name(targ+3).c_str());
        gROOT->cd();

        // Reset
        h_meanPt2_Zh[0][0]->Reset();
        h_meanPt2_Zh[0][1]->Reset();
        h_meanPt2_Zh[1][0]->Reset();
        h_meanPt2_Zh[1][1]->Reset();
        h_meanPt2_Zh[2][0]->Reset();
        h_meanPt2_Zh[2][1]->Reset();

    }

    // Broadening (Q2,Nu,Zh)
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
            {
                // Define broadening histo
                h_broadening_Q2Nu[0][targ][Q2_bin][Nu_bin] = new TH1F("","",N_Zh,Zh_limits);
                h_broadening_Q2Nu[1][targ][Q2_bin][Nu_bin] = new TH1F("","",N_Zh,Zh_limits);
                h_broadening_Q2Nu[2][targ][Q2_bin][Nu_bin] = new TH1F("","",N_Zh,Zh_limits);

                // Obtain histos
                h_meanPt2_Q2Nu[0][0] = (TH1F*) fin->Get(get_meanPt2_Zh_histo_name(targ  , Q2_bin, Nu_bin).c_str());
                h_meanPt2_Q2Nu[0][1] = (TH1F*) fin->Get(get_meanPt2_Zh_histo_name(targ+3, Q2_bin, Nu_bin).c_str());
                h_meanPt2_Q2Nu[1][0] = (TH1F*) fin->Get(get_acc_cleaninterpolated_meanPt2_Zh_histo_name(targ  , Q2_bin, Nu_bin).c_str());
                h_meanPt2_Q2Nu[1][1] = (TH1F*) fin->Get(get_acc_cleaninterpolated_meanPt2_Zh_histo_name(targ+3, Q2_bin, Nu_bin).c_str());
                h_meanPt2_Q2Nu[2][0] = (TH1F*) fin->Get(get_accrc_cleaninterpolated_meanPt2_Zh_histo_name(targ  , Q2_bin, Nu_bin).c_str());
                h_meanPt2_Q2Nu[2][1] = (TH1F*) fin->Get(get_accrc_cleaninterpolated_meanPt2_Zh_histo_name(targ+3, Q2_bin, Nu_bin).c_str());

                // Calculate broadening
                h_broadening_Q2Nu[0][targ][Q2_bin][Nu_bin]->Add(h_meanPt2_Q2Nu[0][1],h_meanPt2_Q2Nu[0][0],1,-1);
                h_broadening_Q2Nu[1][targ][Q2_bin][Nu_bin]->Add(h_meanPt2_Q2Nu[1][1],h_meanPt2_Q2Nu[1][0],1,-1);
                h_broadening_Q2Nu[2][targ][Q2_bin][Nu_bin]->Add(h_meanPt2_Q2Nu[2][1],h_meanPt2_Q2Nu[2][0],1,-1);

                // Write
                fout->cd();
                h_broadening_Q2Nu[0][targ][Q2_bin][Nu_bin]->Write(get_broadening_Zh_histo_name(targ+3, Q2_bin, Nu_bin).c_str());
                h_broadening_Q2Nu[1][targ][Q2_bin][Nu_bin]->Write(get_acc_broadening_Zh_histo_name(targ+3, Q2_bin, Nu_bin).c_str());
                h_broadening_Q2Nu[2][targ][Q2_bin][Nu_bin]->Write(get_accrc_broadening_Zh_histo_name(targ+3, Q2_bin, Nu_bin).c_str());
                gROOT->cd();

                // Reset
                h_meanPt2_Q2Nu[0][0]->Reset();
                h_meanPt2_Q2Nu[0][1]->Reset();
                h_meanPt2_Q2Nu[1][0]->Reset();
                h_meanPt2_Q2Nu[1][1]->Reset();
                h_meanPt2_Q2Nu[2][0]->Reset();
                h_meanPt2_Q2Nu[2][1]->Reset();
            }
        }
    }

    // Close files
    fin->Close();
    fout->Close();

    // Release memory
    delete fin; delete fout;

    std::cout<<"Finished broadening measurements."<<std::endl;

    return 0;
}