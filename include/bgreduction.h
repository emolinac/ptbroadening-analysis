#include "TH1F.h"
#include "TF1.h"
#include "TMath.h"
#include "constants.h"
// Constants of the bg reduction process
const double ChiSQndf_weight = 0.95;
const double ndf_weight      = 0.05;
const double max_ChiSQndf    = 10;  
const double max_gauss_width = 0.3;

const int number_of_fits  = 15;

double Pt2_cutoff_array[number_of_fits];
double ChiSQndf_array[number_of_fits];
double ndf_array[number_of_fits];

// Functions of the bg reduction process

double calculate_weighted_pt2_cutoff(double *ChiSQndf_array, double *ndf_array, double *Pt2_cutoff_array, int array_size)
{
    double total_ChiSQndf = 0;
    double total_ndf      = 0;
    double total_weight   = 0;
    double weight         = 0;
    double sum_weight_Pt2 = 0;

    // Obtention of total ChiSQndf and NDF
    for (int index = 0; index < array_size; index++) 
    {
        if (TMath::IsNaN(Pt2_cutoff_array[index]) || Pt2_cutoff_array[index] == 0) continue;
        total_ChiSQndf += ChiSQndf_array[index];
        total_ndf      += ndf_array[index];
    }
    
    // Obtention of weight*Pt2
    for (int index = 0; index < array_size; index++) 
    {
        if (TMath::IsNaN(Pt2_cutoff_array[index]) || Pt2_cutoff_array[index] == 0) continue;
        weight          = (TMath::Gaus(ChiSQndf_array[index], 1, 0.2) * ChiSQndf_weight + (ndf_array[index] / total_ndf) * ndf_weight) / (ChiSQndf_weight + ndf_weight);
        total_weight   += weight;
        sum_weight_Pt2 += weight * Pt2_cutoff_array[index];
    }

    //std::cout << "weighted_Pt2_cutoff_array=" << sum_weight_Pt2 / total_weight << std::endl;
    return sum_weight_Pt2 / total_weight;
}

double obtain_interpolated_pt2_bin(double x,double prev_bin, double next_bin, double prev_bin_center, double next_bin_center)
{
  double m     = (next_bin - prev_bin)/(next_bin_center - prev_bin_center);
  double y_ref = prev_bin;
  double x_ref = prev_bin_center;

  //returns straigh line interpolation
  return (x - x_ref)*m + y_ref;
}

double obtain_interpolated_pt2_bin_error(double x, double prev_bin_error, double next_bin_error, double prev_bin_center, double next_bin_center)
{
  double m     = (next_bin_error - prev_bin_error)/(next_bin_center - prev_bin_center);
  double y_ref = prev_bin_error;
  double x_ref = prev_bin_center;

  //returns straigh line interpolation
  return (x - x_ref)*m + y_ref;
}

void obtain_bgreducted_pt2(TH1F* h, double cutoff)
{
    if(cutoff==0) return;
    
    for(int Pt2_bin = 1 ; Pt2_bin <= h->GetNbinsX() ; Pt2_bin++)
    {
        if(h->GetBinCenter(Pt2_bin)>cutoff)
        {
            for(int Pt2_sub_bin = Pt2_bin ; Pt2_sub_bin <= h->GetNbinsX() ; Pt2_sub_bin++)
            {
                h->SetBinContent(Pt2_sub_bin, 0.);
                h->SetBinError(Pt2_sub_bin, 0.);
            }
            break;
        }
    }
}

void obtain_interpolated_pt2(TH1F* h)
{
  for(int bin = 1 ; bin <= h->GetNbinsX() ; bin++)
    {
        if(h->GetBinContent(bin)==0)
	    {
	        double prev_bin = h->GetBinContent(bin-1);
	        double next_bin = h->GetBinContent(bin+1);

	        int counter = 0;
	        int exit_counter = 0;

            // Loop to prevent invalid interpolation
	        while(next_bin==0.)
            {
	            counter++;
	            next_bin = h->GetBinContent(bin+1+counter);
	            if(bin+1+counter == N_Pt2)
                {
	                exit_counter++;
	                break;
	            }
	        }

            if(exit_counter!=0){break;}
            double prev_bin_error     = h->GetBinError(bin-1);
	        double next_bin_error     = h->GetBinError(bin+1+counter);
	        double prev_bin_center    = h->GetBinCenter(bin-1);
	        double next_bin_center    = h->GetBinCenter(bin+1+counter);
	        double missing_bin_center = h->GetBinCenter(bin);

	        double interpolated_Pt2       = obtain_interpolated_pt2_bin(missing_bin_center,prev_bin,next_bin,prev_bin_center,next_bin_center);
	        double interpolated_Pt2_error = obtain_interpolated_pt2_bin_error(missing_bin_center,prev_bin,next_bin,prev_bin_center,next_bin_center);

   	        h->SetBinContent(bin, interpolated_Pt2);
	        h->SetBinError(bin, interpolated_Pt2_error);
	    }
    }
}

double obtain_pt2_cutoff(TH1F* h, TFile* fout, TString target, int Q2_bin, int Nu_bin, int Zh_bin)
{
    int fits_completed = 0;
    double Pt2_fit_min = 0;
    double Pt2_fit_max = 3.;

    // Fits loop
    for (int i = 0; i < number_of_fits; i++) 
    {
        Pt2_fit_min = i * delta_Pt2;
        TF1 *fit_func = new TF1("fit_func", "[0]*TMath::Exp(-x/[1])", Pt2_fit_min, Pt2_fit_max);
        fit_func->SetParameter(0, h->GetBinContent(1));
        fit_func->SetParameter(1, 0.2);
        h->Fit(fit_func, "SRQ");

        // Region validity function
        TF1 *validity_func = new TF1("validity_func", "[0]*TMath::Exp(-x/[1])", Pt2_fit_min, 1000);
        validity_func->SetParameter(0, fit_func->GetParameter(0));
        validity_func->SetParameter(1, fit_func->GetParameter(1));

        // Fit quality check
        if((fit_func->GetChisquare() / fit_func->GetNDF() < max_ChiSQndf) && (fit_func->GetParameter(1) < max_gauss_width)) 
        {
            fits_completed++;
            // Check whether the validity function gives a cutoff value bigger or smaller than the maximum experimental value and assign the cutoff
            Pt2_cutoff_array[i] = (validity_func->GetX(1) >= Pt2_fit_max) ? Pt2_fit_max : fit_func->GetX(1);
            ChiSQndf_array[i]   = fit_func->GetChisquare() / fit_func->GetNDF();
            ndf_array[i]        = fit_func->GetNDF();

            // Write fits
            fout->cd();
            h->Write(target+Form("Pt2_fit_%i%i%i_%i", Q2_bin, Nu_bin, Zh_bin, i));
            gROOT->cd();
        }
        else 
        {
            Pt2_cutoff_array[i] = 0;
            ChiSQndf_array[i]   = 0;
            ndf_array[i]        = 0;
        }

        delete fit_func;
        delete validity_func;
    }

    if(fits_completed==0)
    {
        return Pt2_fit_max;
    }
    else if(fits_completed==1)
    {
        return TMath::MaxElement(number_of_fits, Pt2_cutoff_array);
    }
    else if(fits_completed>1)
    {
        return calculate_weighted_pt2_cutoff(ChiSQndf_array, ndf_array, Pt2_cutoff_array, number_of_fits);
    }

    return -1;
}

