#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include "constants.h"

int main(int argc, char *argv[])
{
    // Open file
    TFile* fin = new TFile("../output-files/results-broadening.root");
    if(fin==NULL){std::cout<<"No input file!"<<std::endl; return 1;}

    // Maybe put a directory check part!
}