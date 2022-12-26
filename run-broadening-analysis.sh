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

cd ./bin

# Integration of PhiPQ distributions
./integrate-phi-q2nuzhpt2

# Background reduction in Pt2 distributions
./bgreduction-pt2-q2nuzh

# Calculate meanPt2
./calculate-meanpt2

# Calculate broadening
./calculate-broadening

# <Pt2> Plots

# <Pt2> Tables
