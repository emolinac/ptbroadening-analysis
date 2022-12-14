#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>
#include "TROOT.h"
#include "TString.h"

// Binning configuration
const int N_Q2  = 3;
const int N_Nu  = 3;
const int N_Zh  = 8;
const int N_Pt2 = 90;
const int N_Phi = 12;

const float Q2_limits[N_Q2+1]   = { 1.0 , 1.3 , 1.8 , 4.0 };
const float Nu_limits[N_Nu+1]   = { 2.2 , 3.2 , 3.7 , 4.26};
const float Zh_limits[N_Zh+1]   = { 0.0 , 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.8 , 1.0 };

const float Pt2_min   = 0.;
const float Pt2_max   = 3.;
const float delta_Pt2 = (Pt2_max-Pt2_min)/N_Pt2;

const float Phi_min   = -180.;
const float Phi_max   =  180.;
const float delta_Phi = (Phi_max-Phi_min)/N_Phi;

// Targets
const int N_targets    = 6;
const int N_broadening = 3;

const double A13[N_targets] = { pow(2,1./3.) , pow(2,1./3.) , pow(2,1./3.) , pow(12,1./3.) , pow(55, 1./3.) , pow(208,1./3.) };

TString targets[N_targets]               = { "DC" , "DFe" , "DPb" , "C" , "Fe" , "Pb" };
TString broadening_targets[N_broadening] = { "C" , "Fe" , "Pb" };


// Visual Settings
const int targ_colors[N_targets] = { 2 , 4 , 1 , 2 , 4 , 1 };
const int targ_marker[N_targets] = { 24 , 24 , 24 , 20 , 20 , 20 };

const int zh_colors[N_Zh]        = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8};
const int zh_marker[N_Zh]        = { 20 , 21 , 22 , 23 , 24 , 25 , 26 , 27};

const double shift_x_q2[N_targets]  = { -0.1   , 0 , 0.1   , -0.1   , 0 , 0.1   };
const double shift_x_nu[N_targets]  = { -0.1   , 0 , 0.1   , -0.1   , 0 , 0.1   };
const double shift_x_zh[N_targets]  = { -0.015 , 0 , 0.015 , -0.015 , 0 , 0.015 };

const double shift_x_a13[4] = { -0.075 , -0.025 , 0.025 , 0.075 };

// Analysis
// Integrations of ZH starts at Zh = 0.2
const int Zh_cutoff = 2;

#endif /* CONSTANTS_H */
