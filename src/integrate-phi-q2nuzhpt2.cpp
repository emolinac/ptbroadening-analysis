#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TH1F.h"
#include "integrals.h"
#include "constants.h"

int main(int argc , char *argv[])
{
    // Open files
    TFile* fin  = new TFile("../input-file/phi-distributions.root");
    if(fin==NULL){std::cout<<"No input file!"<<std::endl; return 1;}

    TFile* fout = new TFile("../output-files/pt2-distributions.root","RECREATE");
    gROOT->cd();

    // Integrate through all bins and targets
    TH1F* h_Phi;
    TH1F* h_Pt2 = new TH1F("","",N_Pt2,Pt2_min,Pt2_max);

    std::cout<<"Starting integrations."<<std::endl;
    fout->cd();
    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
            {
                for(int Zh_bin = 0 ; Zh_bin < N_Zh ; Zh_bin++)
                {
                    for(int Pt2_bin = 0 ; Pt2_bin < N_Pt2 ; Pt2_bin++)
                    {
                        h_Phi = (TH1F*) fin->Get(Form("acc_data_"+targets[targ]+"_%i%i%i%i",Q2_bin,Nu_bin,Zh_bin,Pt2_bin));
                        phi_integration(h_Phi, h_Pt2, Pt2_bin);
                    }// End Pt2 loop
                    h_Pt2->Write(Form("corr_data_Pt2_"+targets[targ]+"_%i%i%i",Q2_bin,Nu_bin,Zh_bin));
                    h_Pt2->Reset();
                }// End Zh loop
            }// End Nu loop
        }// End Q2 loop   
        std::cout<<targets[targ]<<" integration completed!"<<std::endl;
    }// End Targets loop
    
    fin->Close(); delete fin;
    fout->Close(); delete fout;
    delete h_Phi;
    delete h_Pt2;

    return 1;
}
