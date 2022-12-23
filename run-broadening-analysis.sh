#!/bin/bash

cd ./bin

# Integration of PhiPQ distributions
#./integrate-phi-q2nuzhpt2

# Background reduction in Pt2 distributions
./bgreduction-pt2-q2nuzh

# Calculate meanPt2
./calculate-meanpt2

# Calculate broadening
./calculate-broadening

# <Pt2> Plots

# <Pt2> Tables
