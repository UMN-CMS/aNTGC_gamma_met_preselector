#!/bin/bash
ahadd=/scratch/mwadud/anTGCpreselector/scripts/ahadd.py
cmsswDir=/hdfs/store/user/mwadud/CMSSW_9_4_13/src/

cd ${cmsswDir}; eval `scramv1 runtime -sh`; cd -;

inDir=$1
inDir=$(readlink -f ${inDir}/)

outDir=$2
outDir=$(readlink -f ${outDir}/)

mkdir -p ${outDir}

python ${ahadd} -f ${outDir}/aNTGC0p00080p0p0p2001000flatPt.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*aNTGC0p00080p0p0p2001000flatPt*.root"`
python ${ahadd} -f ${outDir}/aNTGC0p00080p0p0p.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*aNTGC0p00080p0p0p*.root" -not -name "*flatPt*.root"`
python ${ahadd} -f ${outDir}/aNTGCjjgloh3z0p0008ptmin.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*aNTGCjjgloh3z0p0008ptmin*.root"`


echo "Mergin complete!"