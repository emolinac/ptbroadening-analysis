#!/bin/bash

if [[ ! -d ./output-files || ! -d ./output-plots || ! -d ./output-tables ]]
then
    echo "Output directories do not exist. Making output directories ..."
    mkdir ./output-files
    mkdir ./output-plots
    mkdir ./output-tables
fi

if [[ ! -d ./bin ]]
then
    echo "bin directory does not exist. Making bin directory ..."
    mkdir ./bin
fi

if [[ ! -d ./input-file ]]
then
    echo "Input directory does not exist. Making input directory ..."
    mkdir ./input-file
    echo "Put a file named phi-distributions.root inside this folder; then, execute this script again!"
    exit 1
fi

# Compile the software
make clean
make

cd ./bin

## Integration of PhiPQ distributions
#./integrate-phi-q2nuzhpt2
#
## Background reduction in Pt2 distributions
#./bgreduction-pt2-q2nuzh

# RC Fix process
./rcfactors-fix-fit

# Calculate meanPt2
./calculate-meanpt2

# Calculate broadening
./calculate-broadening

# Print impact of correction
./print-plot-acccorr-accrccorr-comparison

# Plots
#./print-plot-meanpt2-q2nuzh
#./print-plot-meanpt2-q2-nu-zh-a13
#./print-plot-broadening-q2nuzh
#./print-plot-broadening-q2nuzha13
#./print-plot-broadening-q2-nu-zh-a13
