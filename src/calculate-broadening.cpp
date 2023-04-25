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
    TFile* fin1 = new TFile((results_dir+file_name_pt2_bg).c_str());
    TFile* fin2 = new TFile((results_dir+file_name_meanpt2).c_str());
    if(fin1==NULL||fin2==NULL){std::cout<<"No input file!"<<std::endl; return 1;}

    TFile* fout = new TFile((results_dir+file_name_broad).c_str(),"RECREATE");
    gROOT->cd();

    // Instrumental histos
    // Index : 0 (liquid), 1 (solid)
    TH1F* h_meanPt2[2];
    TH1F* h_meanPt2_Q2[2];
    TH1F* h_meanPt2_Nu[2];
    TH1F* h_meanPt2_Zh[2];
    TH1F* h_meanPt2_Q2Nu[2];
    
    // Result histos
    TH1F* h_broadening[N_broadening];
    TH1F* h_broadening_Q2[N_broadening];
    TH1F* h_broadening_Nu[N_broadening];
    TH1F* h_broadening_Zh[N_broadening];
    TH1F* h_broadening_Q2Nu[N_broadening][N_Q2][N_Nu];

    // Total broadening (used for A13 dependence)
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        // Define broadening histo
        h_broadening[targ] = new TH1F("","",1,0,1); 

        // Obtain histos
        h_meanPt2[0] = (TH1F*) fin2->Get(get_meanPt2_histo_name(targ).c_str());
        h_meanPt2[1] = (TH1F*) fin2->Get(get_meanPt2_histo_name(targ+3).c_str());

        // Calculate broadening
        h_broadening[targ]->Add(h_meanPt2[1],h_meanPt2[0],1,-1);

        // Write
        fout->cd();
        h_broadening[targ]->Write(get_broadening_histo_name(targ+3).c_str());
        gROOT->cd();

        // Reset
        h_meanPt2[0]->Reset();
        h_meanPt2[1]->Reset();
    }

    // Broadening (Q2)
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        // Define broadening histo
        h_broadening_Q2[targ] = new TH1F("","",N_Q2,Q2_limits);

        // Obtain histos
        h_meanPt2_Q2[0] = (TH1F*) fin2->Get(get_meanPt2_Q2_histo_name(targ).c_str());
        h_meanPt2_Q2[1] = (TH1F*) fin2->Get(get_meanPt2_Q2_histo_name(targ+3).c_str());

        // Calculate broadening
        h_broadening_Q2[targ]->Add(h_meanPt2_Q2[1],h_meanPt2_Q2[0],1,-1);

        // Write
        fout->cd();
        h_broadening_Q2[targ]->Write(get_broadening_Q2_histo_name(targ+3).c_str());
        gROOT->cd();

        // Reset
        h_meanPt2_Q2[0]->Reset(); 
        h_meanPt2_Q2[1]->Reset();
    }

    // Broadening (Nu)
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        // Define broadening histo
        h_broadening_Nu[targ] = new TH1F("","",N_Nu,Nu_limits);

        // Obtain histos
        h_meanPt2_Nu[0] = (TH1F*) fin2->Get(get_meanPt2_Nu_histo_name(targ).c_str());
        h_meanPt2_Nu[1] = (TH1F*) fin2->Get(get_meanPt2_Nu_histo_name(targ+3).c_str());

        // Calculate broadening
        h_broadening_Nu[targ]->Add(h_meanPt2_Nu[1],h_meanPt2_Nu[0],1,-1);

        // Write
        fout->cd();
        h_broadening_Nu[targ]->Write(get_broadening_Nu_histo_name(targ+3).c_str());
        gROOT->cd();

        // Reset
        h_meanPt2_Nu[0]->Reset();
        h_meanPt2_Nu[1]->Reset();
    }

    // Broadening (Zh)
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        // Define broadening histo
        h_broadening_Zh[targ] = new TH1F("","",N_Zh,Zh_limits);

        // Obtain histos
        h_meanPt2_Zh[0] = (TH1F*) fin2->Get(get_meanPt2_Zh_histo_name(targ).c_str());
        h_meanPt2_Zh[1] = (TH1F*) fin2->Get(get_meanPt2_Zh_histo_name(targ+3).c_str());

        // Calculate broadening
        h_broadening_Zh[targ]->Add(h_meanPt2_Zh[1],h_meanPt2_Zh[0],1,-1);

        // Write
        fout->cd();
        h_broadening_Zh[targ]->Write(get_broadening_Zh_histo_name(targ+3+3).c_str());
        gROOT->cd();

        // Reset
        h_meanPt2_Zh[0]->Reset();
        h_meanPt2_Zh[1]->Reset();
    }

    // Broadening (Q2,Nu,Zh)
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
            {
                // Define broadening histo
                h_broadening_Q2Nu[targ][Q2_bin][Nu_bin] = new TH1F("","",N_Zh,Zh_limits);

                // Obtain histos
                h_meanPt2_Q2Nu[0] = (TH1F*) fin2->Get(get_cleaninterpolated_meanPt2_Zh_histo_name(targ  , Q2_bin, Nu_bin).c_str());
                h_meanPt2_Q2Nu[1] = (TH1F*) fin2->Get(get_cleaninterpolated_meanPt2_Zh_histo_name(targ+3, Q2_bin, Nu_bin).c_str());

                // Calculate broadening
                h_broadening_Q2Nu[targ][Q2_bin][Nu_bin]->Add(h_meanPt2_Q2Nu[1],h_meanPt2_Q2Nu[0],1,-1);

                // Write
                fout->cd();
                h_broadening_Q2Nu[targ][Q2_bin][Nu_bin]->Write(get_broadening_Zh_histo_name(targ+3+3, Q2_bin, Nu_bin).c_str());
                gROOT->cd();

                // Reset
                h_meanPt2_Q2Nu[0]->Reset();
                h_meanPt2_Q2Nu[1]->Reset();
            }
        }
    }

    // Close files
    fin1->Close();
    fin2->Close();
    fout->Close();

    // Release memory
    delete fin1; delete fin2; delete fout;

    std::cout<<"Finished broadening measurements."<<std::endl;

    return 0;
}