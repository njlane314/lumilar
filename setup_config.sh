#!/usr/bin/env bash

source /cvmfs/larsoft.opensciencegrid.org/products/setup

setup cmake  v3_20_0
setup boost  v1_75_0      -q e20:prof
setup root   v6_22_08d    -q e20:p392:prof
setup geant4 v4_11_0_p01c -q e20:prof