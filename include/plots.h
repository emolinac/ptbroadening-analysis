#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TLatex.h"
#include "TPad.h"
#include "constants.h"

// Visual settings
double lp_leftmargin  = 0.12;
double rp_rightmargin = 0.1;

double up_margin      = 0.1;
double bottom_margin  = 0.1;

double lp_length = 1./(2-lp_leftmargin+(1-lp_leftmargin)/(1-rp_rightmargin));
double cp_length = (1-lp_leftmargin)*lp_length;
double rp_length = (1-lp_leftmargin)*lp_length/(1-rp_rightmargin);

double top_height    = 1./(2-up_margin+(1-up_margin)/(1-bottom_margin));
double med_height    = (1-up_margin)*top_height;
double bottom_height = (1-up_margin)*top_height/(1-bottom_margin);

double pad_X1[3][3] = {{0 , lp_length , lp_length+cp_length},
              		   {0 , lp_length , lp_length+cp_length},
			           {0 , lp_length , lp_length+cp_length}};

double pad_X2[3][3] = {{lp_length , lp_length+cp_length , 1},
			           {lp_length , lp_length+cp_length , 1},
			           {lp_length , lp_length+cp_length , 1}};

double pad_Y1[3][3] = {{bottom_height+med_height , bottom_height+med_height , bottom_height+med_height},
			           {bottom_height            , bottom_height            , bottom_height           },
			           {0                        , 0                        , 0                       }};

double pad_Y2[3][3] = {{1                        , 1                        , 1                       },
			           {bottom_height+med_height , bottom_height+med_height , bottom_height+med_height},
			           {bottom_height            , bottom_height            , bottom_height           }};

double b_margins[3][3] = {{0             , 0             , 0            },
			              {0             , 0             , 0            },
			              {bottom_margin , bottom_margin , bottom_margin}};

double t_margins[3][3] = {{up_margin , up_margin , up_margin},
			              {0         , 0         , 0        },
			              {0         , 0         , 0        }};

double l_margins[3][3] = {{lp_leftmargin, 0, 0 },
			              {lp_leftmargin, 0, 0 },
			              {lp_leftmargin, 0, 0 }};

double r_margins[3][3] = {{0, 0, rp_rightmargin },
			              {0, 0, rp_rightmargin },
			              {0, 0, rp_rightmargin }};

// Functions
void set_xerr_null(TGraphErrors* g, int Npoints = N_Zh)
{
    // Fail-safe
    if(g==NULL){std::cout<<"No TGraphErrors passed!"<<std::endl; return;}

    // Obtain errors in Y
    double* errors_Y = g->GetEY();

    // Set errors of X to zero
    for(int point = 0 ; point < Npoints ; point++)
    {
        g->SetPointError(point,0,errors_Y[point]);  
    }
}

void shift_x(TGraphErrors* g, double shift, int Npoints = N_Zh)
{
    // Fail-safe
    if(g==NULL){std::cout<<"No TGraphErrors passed!"<<std::endl; return;}

    // Obtain contents
    double* content_Y = g->GetY();
    double* content_X = g->GetX();

    // Shift the x coordinate of the TGraph
    for(int point = 0; point < Npoints ; point++)
    {
        g->SetPoint(point, content_X[point] + shift , content_Y[point]);
    }
}

void set_q2_limits_pads(TPad* p1, TPad* p2, TPad* p3, TLatex* t)
{
    // Fail-safe
    if(p1==NULL || p2==NULL || p3==NULL){std::cout<<"No TPads passed!"<<std::endl; return;}

    p1->cd(); t->DrawLatexNDC(1-p1->GetRightMargin()/2.,0.5,Form("%.1f<Q^{2}(GeV^{2})<%.1f",Q2_limits[0],Q2_limits[1]));
    p2->cd(); t->DrawLatexNDC(1-p1->GetRightMargin()/2.,0.5,Form("%.1f<Q^{2}(GeV^{2})<%.1f",Q2_limits[1],Q2_limits[2]));
    p3->cd(); t->DrawLatexNDC(1-p1->GetRightMargin()/2.,0.5,Form("%.1f<Q^{2}(GeV^{2})<%.1f",Q2_limits[2],Q2_limits[3]));
}

void set_nu_limits_pads(TPad* p1, TPad* p2, TPad* p3, TLatex* t)
{
    // Fail-safe
    if(p1==NULL || p2==NULL || p3==NULL){std::cout<<"No TPads passed!"<<std::endl; return;}

    p1->cd(); t->DrawLatexNDC(0.5,1-p1->GetTopMargin()/2.,Form("%.1f<#nu(GeV)<%.1f",Nu_limits[0],Nu_limits[1]));
    p2->cd(); t->DrawLatexNDC(0.5,1-p1->GetTopMargin()/2.,Form("%.1f<#nu(GeV)<%.1f",Nu_limits[1],Nu_limits[2]));
    p3->cd(); t->DrawLatexNDC(0.5,1-p1->GetTopMargin()/2.,Form("%.1f<#nu(GeV)<%.1f",Nu_limits[2],Nu_limits[3]));
}

void th1f_to_tgrapherrors(TH1F* h, TGraphErrors* g)
{
    const int Nbins = h->GetNbinsX();
    double X[Nbins];
    double Y[Nbins];
    double Xerr[Nbins];
    double Yerr[Nbins];

    // Obtain coordinates and errors
    for(int bin = 1 ; bin <= Nbins ; bin++)
    {
        X[bin-1]    = h->GetBinCenter(bin);
        Y[bin-1]    = h->GetBinContent(bin);
        Xerr[bin-1] = (h->GetBinWidth(bin)/2.);
        Yerr[bin-1] = h->GetBinError(bin);
    }

    // Set coordinates and errors to graph
    for(int point = 0 ; point < Nbins ; point++)
    {
        g->SetPoint(point, X[point], Y[point]);
        g->SetPointError(point, Xerr[point], Yerr[point]);
    }

    return;
}

void set_pad_attributes(TPad* p, int Q2_bin, int Nu_bin)
{
    p->SetBottomMargin(b_margins[Q2_bin][Nu_bin]);
    p->SetTopMargin(t_margins[Q2_bin][Nu_bin]);
    p->SetLeftMargin(l_margins[Q2_bin][Nu_bin]);
    p->SetRightMargin(r_margins[Q2_bin][Nu_bin]);
    p->SetGridx(1);
    p->SetGridy(1);
}

void set_multigraph_properties(TMultiGraph* mg)
{
    // X axis
    mg->GetYaxis()->SetRangeUser(0.04,0.37);
    mg->GetXaxis()->SetRangeUser(0.11,1.);
    mg->GetXaxis()->SetTitleOffset(1.1);
    mg->GetXaxis()->SetTitle("z_{h}");
    mg->GetXaxis()->CenterTitle();

    // Y axis
    mg->GetYaxis()->SetTitle("<P^{2}_{T}> (GeV^{2})");
    mg->GetYaxis()->CenterTitle();
    mg->GetYaxis()->SetTitleOffset(1.4);
}

void set_latex_properties(TLatex* t_q2, TLatex* t_nu)
{
    // Related to Q2
    t_q2->SetTextAlign(22);
    t_q2->SetTextFont(62);
    t_q2->SetTextSize(0.05);
    t_q2->SetTextAngle(-90);

    // Related to Nu
    t_nu->SetTextAlign(22);
    t_nu->SetTextFont(62);
    t_nu->SetTextSize(0.05);  
}