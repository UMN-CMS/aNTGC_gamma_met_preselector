#!/bin/bash

outDir=$1
outDir=$(readlink -f ${outDir}/)



find ${outDir} -name "*QCD*.root" > ${outDir}/QCD_preSelected.txt
find ${outDir} -name "*GJets*.root" -not -name "*TGJets*"> ${outDir}/GJets_preSelected.txt
# find ${outDir} -name "*TTJets*.root"> ${outDir}/TTJets_preSelected.txt
# find ${outDir} -name "*TTGammaHadronic*.root"> ${outDir}/TTGammaHadronic_preSelected.txt
# find ${outDir} -name "*TTGJets*.root"> ${outDir}/TTGJets_preSelected.txt
# find ${outDir} -name "*TTToHadronic*.root"> ${outDir}/TTToHadronic_preSelected.txt
# find ${outDir} -name "*SinglePhoton2017*.root"> ${outDir}/SinglePhoton2017_preSelected.txt

# find ${outDir} -name "*aNTGCjjgloh3z0pt*.root"> ${outDir}/aNTGCjjgloh3z0sm_preSelected.txt
# find ${outDir} -name "*aNTGCjjgloh3z0p0003*.root"> ${outDir}/aNTGCjjgloh3z0p0003_preSelected.txt
# find ${outDir} -name "*aNTGCjjgloh3z0p0005*.root"> ${outDir}/aNTGCjjgloh3z0p0005_preSelected.txt
# find ${outDir} -name "*aNTGCjjgloh3z0p0008*.root"> ${outDir}/aNTGCjjgloh3z0p0008_preSelected.txt
# find ${outDir} -name "*aNTGCjjgloh3z0p0015*.root"> ${outDir}/aNTGCjjgloh3z0p0015_preSelected.txt
# find ${outDir} -name "*aNTGCjjgloh3z0p0029*.root"> ${outDir}/aNTGCjjgloh3z0p0029_preSelected.txt
# find ${outDir} -name "*aNTGCjjgloh3z0p0038*.root"> ${outDir}/aNTGCjjgloh3z0p0038_preSelected.txt


preSelectedList=${outDir}/preSelectedList.txt
rm -f ${preSelectedList}
echo "QCD,"${outDir}/QCD_preSelected.txt >>${preSelectedList}
echo "GJets,"${outDir}/GJets_preSelected.txt>>${preSelectedList}
# echo "TTGJetsTuneCP513TeVamcatnloFXFXmadspinpythia8,"${outDir}/TTGJets_preSelected.txt>>${preSelectedList}
# echo "TTGammaHadronicTuneCP5PSweights13TeVmadgraphpythia8,"${outDir}/TTGammaHadronic_preSelected.txt>>${preSelectedList}
# echo "TTJetsTuneCP513TeVamcatnloFXFXpythia8,"${outDir}/TTJets_preSelected.txt>>${preSelectedList}
# echo "TTToHadronicTuneCP513TeVpowhegpythia8,"${outDir}/TTToHadronic_preSelected.txt>>${preSelectedList}
# echo "SinglePhoton2017,"${outDir}/SinglePhoton2017_preSelected.txt>>${preSelectedList}
# echo "aNTGCjjgloh3z0sm,"${outDir}/aNTGCjjgloh3z0sm_preSelected.txt>>${preSelectedList}
# echo "aNTGCjjgloh3z0p0003,"${outDir}/aNTGCjjgloh3z0p0003_preSelected.txt>>${preSelectedList}
# echo "aNTGCjjgloh3z0p0005,"${outDir}/aNTGCjjgloh3z0p0005_preSelected.txt>>${preSelectedList}
# echo "aNTGCjjgloh3z0p0008,"${outDir}/aNTGCjjgloh3z0p0008_preSelected.txt>>${preSelectedList}
# echo "aNTGCjjgloh3z0p0015,"${outDir}/aNTGCjjgloh3z0p0015_preSelected.txt>>${preSelectedList}
# echo "aNTGCjjgloh3z0p0029,"${outDir}/aNTGCjjgloh3z0p0029_preSelected.txt>>${preSelectedList}
# echo "aNTGCjjgloh3z0p0038,"${outDir}/aNTGCjjgloh3z0p0038_preSelected.txt>>${preSelectedList}


