#!/bin/bash

workDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"/

function getXsec(){
	_CompLogFile=$1
	_SampleName=$2

	_compFileName=$(basename -- "$_CompLogFile")
	_exDir=${workDir}/${_compFileName%".log.tar.gz"}
	_jobNum=${_compFileName//[!0-9]/}
	_logFile=${_exDir}/cmsRun-stdout-${_jobNum}.log

	mkdir -p ${_exDir}
	tar -xf ${_CompLogFile} -C ${_exDir}

	miniaodFile=$(grep "Closed file" ${_logFile})
	miniaodFile=${miniaodFile##*/}

	xsec=$(grep "After filter: final cross section =" ${_logFile})
	xsec=$(echo $xsec | sed "s/After filter: final cross section =//g")
	xsec=$(echo $xsec | sed "s/+-/,/g")
	xsec=$(echo $xsec | sed "s/ //g")
	xsec=$(echo $xsec | sed "s/nb/,nb/g")
	xsec=$(echo $xsec | sed "s/pb/,pb/g")
	xsec=$(echo $xsec | sed "s/fb/,fb/g")

	rm -rf ${_exDir}

	echo ${_SampleName},${miniaodFile},${xsec}
}



searchPath=$1
writeFile=$2

rm -f ${writeFile}

# for jobDir in $(find "${searchPath}" -maxdepth 1 -mindepth 1 -type d -not -path '*/\.*');
	# for logFile in $(find ${jobDir} -name "*.gz" -path "*crab_xSections_*" -not -path '*/failed/*' -type f);
# for jobDir in $(find "${searchPath}" -maxdepth 2 -mindepth 2 -type d -path "*GENSIM*"); do
for jobDir in $(find "${searchPath}" -maxdepth 2 -mindepth 2 -type d -path "*GENSIM*" -and -path "*_300_1200/*"); do
	for logFile in $(find ${jobDir} -name "*.gz" -type f -not -path '*/failed/*'); do
		_sampleName=$(basename -- "$jobDir")
		_sampleName=$(echo ${_sampleName} | tr -cd [:alnum:])
		getXsec ${logFile} ${_sampleName} | tee -a ${writeFile}
	done
done