#!/bin/bash

ahadd=/scratch/mwadud/anTGCpreselector/scripts/ahadd.py
cmsswDir=/hdfs/store/user/mwadud/CMSSW_9_4_13/src/

cd ${cmsswDir}; eval `scramv1 runtime -sh`; cd -;

function mergeFilesInDir(){
	_dir=$1
	_outDir=$2

	echo "Merging root files in " ${_dir}

	_outFile=${_outDir}/$(basename -- "$_dir")".root"

	python ${ahadd} -f ${_outFile} ${_dir}/*.root
}


searchPath=$1
searchPath=$(readlink -f ${searchPath}/)

outDir=$2
outDir=$(readlink -f ${outDir}/)


echo "start" $outDir
mkdir -p ${outDir}
# for directory in $(find "${searchPath}" -maxdepth 1 -mindepth 1 -type d -not -path '*/\.*' -and -not -path '*/xSecs');
for directory in $(find "${searchPath}" -maxdepth 1 -mindepth 1 -name "*aNTGCjjgloh3z0pt*"  -type d);
do
	mergeFilesInDir ${directory} ${outDir}
done


# python ${ahadd} ${_outDir}/SinglePhoton2017.root ${_outDir}/SinglePhotonRun2017*17Nov2017v1MINIAOD.root

# rm -f ${_outDir}/SinglePhotonRun2017*17Nov2017v1MINIAOD.root



# find ${outDir} -name "*QCD*.root" > ${outDir}/QCD_preSelected.txt
# find ${outDir} -name "*GJets*.root" -not -name "*TGJets*"> ${outDir}/GJets_preSelected.txt
# find ${outDir} -name "*TTJets*.root"> ${outDir}/TTJets_preSelected.txt
# find ${outDir} -name "*TTGammaHadronic*.root"> ${outDir}/TTGammaHadronic_preSelected.txt
# find ${outDir} -name "*TTGJets*.root"> ${outDir}/TTGJets_preSelected.txt
# find ${outDir} -name "*TTToHadronic*.root"> ${outDir}/TTToHadronic_preSelected.txt
# find ${outDir} -name "*SinglePhoton2017*.root"> ${outDir}/SinglePhoton2017_preSelected.txt

find ${outDir} -name "*aNTGCjjgloh3z0pt*.root" > ${outDir}/aNTGCjjgloh3z0sm_preSelected.txt
# find ${outDir} -name "*aNTGCjjgloh3z0p0003*.root"> ${outDir}/aNTGCjjgloh3z0p0003_preSelected.txt
# find ${outDir} -name "*aNTGCjjgloh3z0p0005*.root"> ${outDir}/aNTGCjjgloh3z0p0005_preSelected.txt
# find ${outDir} -name "*aNTGCjjgloh3z0p0008*.root"> ${outDir}/aNTGCjjgloh3z0p0008_preSelected.txt
# find ${outDir} -name "*aNTGCjjgloh3z0p0015*.root"> ${outDir}/aNTGCjjgloh3z0p0015_preSelected.txt
# find ${outDir} -name "*aNTGCjjgloh3z0p0029*.root"> ${outDir}/aNTGCjjgloh3z0p0029_preSelected.txt
# find ${outDir} -name "*aNTGCjjgloh3z0p0038*.root"> ${outDir}/aNTGCjjgloh3z0p0038_preSelected.txt

# find ${outDir} -name "*aNTGC0p00080p0p0p*.root" -not -name "*flat*"> ${outDir}/aNTGC0p00080p0p0pBinnedPt_preSelected.txt
# find ${outDir} -name "*aNTGC0p00080p0p0p*flat*.root"> ${outDir}/aNTGC0p00080p0p0pFlatPt_preSelected.txt


preSelectedList=${outDir}/preSelectedList.txt
# rm -f ${preSelectedList}
# echo "QCD,"${outDir}/QCD_preSelected.txt >>${preSelectedList}
# echo "GJets,"${outDir}/GJets_preSelected.txt>>${preSelectedList}
# echo "TTGJetsTuneCP513TeVamcatnloFXFXmadspinpythia8,"${outDir}/TTGJets_preSelected.txt>>${preSelectedList}
# echo "TTGammaHadronicTuneCP5PSweights13TeVmadgraphpythia8,"${outDir}/TTGammaHadronic_preSelected.txt>>${preSelectedList}
# echo "TTJetsTuneCP513TeVamcatnloFXFXpythia8,"${outDir}/TTJets_preSelected.txt>>${preSelectedList}
# echo "TTToHadronicTuneCP513TeVpowhegpythia8,"${outDir}/TTToHadronic_preSelected.txt>>${preSelectedList}
# echo "SinglePhoton2017,"${outDir}/SinglePhoton2017_preSelected.txt>>${preSelectedList}
echo "aNTGCjjgloh3z0sm,"${outDir}/aNTGCjjgloh3z0sm_preSelected.txt>>${preSelectedList}
# echo "aNTGCjjgloh3z0p0003,"${outDir}/aNTGCjjgloh3z0p0003_preSelected.txt>>${preSelectedList}
# echo "aNTGCjjgloh3z0p0005,"${outDir}/aNTGCjjgloh3z0p0005_preSelected.txt>>${preSelectedList}
# echo "aNTGCjjgloh3z0p0008,"${outDir}/aNTGCjjgloh3z0p0008_preSelected.txt>>${preSelectedList}
# echo "aNTGCjjgloh3z0p0015,"${outDir}/aNTGCjjgloh3z0p0015_preSelected.txt>>${preSelectedList}
# echo "aNTGCjjgloh3z0p0029,"${outDir}/aNTGCjjgloh3z0p0029_preSelected.txt>>${preSelectedList}
# echo "aNTGCjjgloh3z0p0038,"${outDir}/aNTGCjjgloh3z0p0038_preSelected.txt>>${preSelectedList}

# echo "aNTGC0p00080p0p0pBinnedPt,"${outDir}/aNTGC0p00080p0p0pBinnedPt_preSelected.txt>>${preSelectedList}
# echo "aNTGC0p00080p0p0pFlatPt,"${outDir}/aNTGC0p00080p0p0pFlatPt_preSelected.txt>>${preSelectedList}


