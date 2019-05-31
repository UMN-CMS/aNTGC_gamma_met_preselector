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
for directory in $(find "${searchPath}" -maxdepth 1 -mindepth 1 -type d);
do
	mergeFilesInDir ${directory} ${outDir}
done

find ${outDir} -name "*aNTGC0p00080p0p0p*.root" -not -name "*flat*"> ${outDir}/aNTGC0p00080p0p0pBinnedPt_preSelected.txt
find ${outDir} -name "*aNTGC0p00080p0p0p*flat*.root"> ${outDir}/aNTGC0p00080p0p0pFlatPt_preSelected.txt
find ${outDir} -name "*aNTGCjjgloh3z0p0008*.root"> ${outDir}/aNTGCjjgloh3z0p0008EFT_preSelected.txt

preSelectedList=${outDir}/preSelectedList.txt
rm -f ${preSelectedList}
echo "aNTGC0p00080p0p0pBinnedPt,"${outDir}/aNTGC0p00080p0p0pBinnedPt_preSelected.txt>>${preSelectedList}
echo "aNTGC0p00080p0p0pFlatPt,"${outDir}/aNTGC0p00080p0p0pFlatPt_preSelected.txt>>${preSelectedList}
echo "aNTGCjjgloh3z0p0008EFT,"${outDir}/aNTGCjjgloh3z0p0008EFT_preSelected.txt>>${preSelectedList}
