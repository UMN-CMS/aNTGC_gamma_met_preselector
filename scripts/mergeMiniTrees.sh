#!/bin/bash
ahadd=/scratch/mwadud/anTGCpreselector/scripts/ahadd.py
cmsswDir=/hdfs/store/user/mwadud/CMSSW_9_4_13/src/

cd ${cmsswDir}; eval `scramv1 runtime -sh`; cd -;

inDir=$1
inDir=$(readlink -f ${inDir}/)

outDir=$2
outDir=$(readlink -f ${outDir}/)

mkdir -p ${outDir}


python ${ahadd} -f ${outDir}/QCD.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*QCD*.root"`
python ${ahadd} -f ${outDir}/GJets.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*GJets*.root" -not -name "*TT*"`
python ${ahadd} -f ${outDir}/TTJets.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*TTJets*.root"`
python ${ahadd} -f ${outDir}/TTGammaHadronic.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*TTGammaHadronic*.root"`
python ${ahadd} -f ${outDir}/TTGJets.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*TTGJets*.root"`
python ${ahadd} -f ${outDir}/TTToHadronic.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*TTToHadronic*.root"`
python ${ahadd} -f ${outDir}/SinglePhoton2017.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*SinglePhoton2017*.root"`
python ${ahadd} -f ${outDir}/aNTGCjjgloh3z0.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*aNTGCjjgloh3z0ptmin200ptmax*.root"`
python ${ahadd} -f ${outDir}/aNTGCjjgloh3z0p0003.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*aNTGCjjgloh3z0p0003ptmin*.root"`
python ${ahadd} -f ${outDir}/aNTGCjjgloh3z0p0005.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*aNTGCjjgloh3z0p0005ptmin*.root"`
python ${ahadd} -f ${outDir}/aNTGCjjgloh3z0p0008.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*aNTGCjjgloh3z0p0008ptmin*.root"`
python ${ahadd} -f ${outDir}/aNTGCjjgloh3z0p0015.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*aNTGCjjgloh3z0p0015ptmin*.root"`
python ${ahadd} -f ${outDir}/aNTGCjjgloh3z0p0029.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*aNTGCjjgloh3z0p0029ptmin*.root"`
python ${ahadd} -f ${outDir}/aNTGCjjgloh3z0p0038.root `find ${inDir} -maxdepth 1 -mindepth 1 -name "*aNTGCjjgloh3z0p0038ptmin*.root"`

echo "Mergin complete!"