#include "TH1F.h"
#include "constants.h"

void phi_integration(TH1F* h_Phi, TH1F* h_Pt2, int Pt2_bin)
{
	if(h_Phi==NULL)
	{
		h_Pt2->SetBinContent(Pt2_bin+1, 0);
		h_Pt2->SetBinError(Pt2_bin+1, 0.);

		return;
	}
	
    double error[1] = {};
	double integral = h_Phi->IntegralAndError(1,N_Phi,*error);

	h_Pt2->SetBinContent(Pt2_bin+1, integral);
	h_Pt2->SetBinError(Pt2_bin+1, error[0]);

    return;
}

void q2nuzh_integration(TH1F* h_Pt2_integral, TH1F* h_Pt2, TFile* fin, int targ)
{
	for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
	{
		for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
		{
			for(int Zh_bin = Zh_cutoff ; Zh_bin < N_Zh ; Zh_bin++)
			{
				h_Pt2 = (TH1F*) fin->Get(Form("Pt2_" + targets[targ] + "_%i%i%i_clean_interpolated", Q2_bin, Nu_bin, Zh_bin));
				h_Pt2_integral->Add(h_Pt2, 1);
			}
		}
	}

	return;
}

void q2nu_integration(TH1F* h_Pt2_integral, TH1F* h_Pt2, TFile* fin, int targ, int Zh_bin)
{
	for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
	{
		for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
		{
			h_Pt2 = (TH1F*) fin->Get(Form("Pt2_" + targets[targ] + "_%i%i%i_clean_interpolated", Q2_bin, Nu_bin, Zh_bin));
			h_Pt2_integral->Add(h_Pt2, 1);
		}
	}

	return;
}

void q2zh_integration(TH1F* h_Pt2_integral, TH1F* h_Pt2, TFile* fin, int targ, int Nu_bin)
{
	for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
	{
		for(int Zh_bin = Zh_cutoff ; Zh_bin < N_Zh ; Zh_bin++)
		{
			h_Pt2 = (TH1F*) fin->Get(Form("Pt2_" + targets[targ] + "_%i%i%i_clean_interpolated", Q2_bin, Nu_bin, Zh_bin));
			h_Pt2_integral->Add(h_Pt2, 1);
		}
	}

	return;
}

void nuzh_integration(TH1F* h_Pt2_integral, TH1F* h_Pt2, TFile* fin, int targ, int Q2_bin)
{
	for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
	{
		for(int Zh_bin = Zh_cutoff ; Zh_bin < N_Zh ; Zh_bin++)
		{
			h_Pt2 = (TH1F*) fin->Get(Form("Pt2_" + targets[targ] + "_%i%i%i_clean_interpolated", Q2_bin, Nu_bin, Zh_bin));
			h_Pt2_integral->Add(h_Pt2, 1);
		}
	}

	return;
}
