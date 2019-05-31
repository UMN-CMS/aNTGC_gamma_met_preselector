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
# for directory in $(find "${searchPath}" -maxdepth 1 -mindepth 1 -type d);
for directory in $(find "${searchPath}" -maxdepth 1 -mindepth 1 -type d -path "*3001200*");
do
	mergeFilesInDir ${directory} ${outDir}
done


find ${outDir} -name "*aNTGC0p00050p00p00p03001200.root"> ${outDir}/aNTGC0p00050p00p00p03001200_preSelected.txt
find ${outDir} -name "*aNTGC0p0010p00p00p03001200.root"> ${outDir}/aNTGC0p0010p00p00p03001200_preSelected.txt
find ${outDir} -name "*aNTGC0p00p00p00p03001200.root"> ${outDir}/aNTGC0p00p00p00p03001200_preSelected.txt
find ${outDir} -name "*aNTGC0p0020p00p00p03001200.root"> ${outDir}/aNTGC0p0020p00p00p03001200_preSelected.txt



preSelectedList=${outDir}/preSelectedList.txt
echo "aNTGC0p0020p00p00p03001200,"${outDir}/aNTGC0p0020p00p00p03001200_preSelected.txt >>${preSelectedList}
echo "aNTGC0p0010p00p00p03001200,"${outDir}/aNTGC0p0010p00p00p03001200_preSelected.txt >>${preSelectedList}
echo "aNTGC0p00050p00p00p03001200,"${outDir}/aNTGC0p00050p00p00p03001200_preSelected.txt >>${preSelectedList}
echo "aNTGC0p00p00p00p03001200,"${outDir}/aNTGC0p00p00p00p03001200_preSelected.txt >>${preSelectedList}
