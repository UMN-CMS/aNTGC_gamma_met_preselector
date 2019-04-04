#!/bin/bash

ahadd=/scratch/mwadud/anTGCpreselector/scripts/ahadd.py
cmsswDir=/hdfs/store/user/mwadud/CMSSW_9_4_13/src/

cd ${cmsswDir}; eval `scramv1 runtime -sh`; cd -;

function mergeFilesInDir(){
	_dir=$1
	_outDir=$2

	echo "Merging root files in " ${_dir}

	_outFile=${_outDir}/$(basename -- "$_dir")".root"

	python ${ahadd} ${_outFile} ${_dir}/*.root
}


searchPath=$1
outDir=$2

echo "start"
# for directory in $(find "${searchPath}" -maxdepth 1 -mindepth 1 -type d -not -path '*/\.*' -and -not -path '*/xSecs');
for directory in $(find "${searchPath}" -maxdepth 1 -mindepth 1 -type d);
do
	echo $${directory}
	mergeFilesInDir ${directory} ${outDir}
done
