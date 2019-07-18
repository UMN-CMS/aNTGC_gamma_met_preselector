#!/bin/bash
source #cmssetsh
cd #cmsswdir; eval `scramv1 runtime -sh`; cd -;
echo "Begin script..."
root -b -q #macrofile
echo "End script!"
