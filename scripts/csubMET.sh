#!/bin/bash

### syntax:  bash csub.sh [path_to_ntuple_lists]  [write_directory]

cmsswDir=/hdfs/store/user/mwadud/CMSSW_9_4_13/src/
workDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"/

writeDir=$2
jobsDir=$2
jobList=$1

dataPUfile="/hdfs/store/user/mwadud/aNTGCmet/pileup/Processed2017DataPileup.root"





############################################################################
jobflavor=tomorrow
splitfiles=50
macroTemplate=${workDir}/macroTemplate.C
runScriptTemplate=${workDir}/submit_job.sh
condorCFGtemplate=${workDir}/condor_job.sh
ccfil1epath=$(readlink -e ${workDir}/../macros/eventPreselectorGammaMET.cc)
ccfil2epath=$(readlink -e ${workDir}/../macros/extra_tools.cc)
hfilepath=$(readlink -e ${workDir}/../macros/eventPreselectorGammaMET.h)

ccfilename=$(basename "${ccfil1epath}")

current_date_time=$(date +%Y-%m-%d_%H-%M-%S)
echo $current_date_time;


# source /cvmfs/cms.cern.ch/cmsset_default.sh;
# cd ${cmsswDir}; eval `scramv1 runtime -sh`; cd -;
# cd ${workDir}

echo -e "\n\n"

echo 'Work directory = '${workDir}''
echo 'Job directory = '${jobsDir}''
echo 'Write directory = '${writeDir}''
echo 'Job list = '${jobList}''


function preSelectDtaset(){
	fileListPath=$(echo $1 | tr -d '\040\011\012\015')
	jobName=$(echo $2 | tr -d '\040\011\012\015')
	jobDir=$(echo $3 | tr -d '\040\011\012\015')
	writeOutDir=$(echo $4 | tr -d '\040\011\012\015')
	mcPUdist=$(echo $5 | tr -d '\040\011\012\015')

	echo	-e		"\t\t Creating job for " ${fileListPath}
	echo 	-e		"\t\t Job name "${jobName}
	echo 	-e		"\t\t Job directory "${jobDir}
	echo 	-e		"\t\t Pileup file "${mcPUdist}
	echo 	-e		"\t\t Ouput directory "${writeOutDir}

	if [ ! -f ${fileListPath} ]; then
		echo "Error! File not found! Offending file: " ${fileListPath}
		exit
	fi

	nFiles=$(sed -n '=' ${fileListPath} | wc -l)
	echo	-e	"\t\t# of files = " ${nFiles}


	#### output root file ###
	outFile=${jobName}.root
	echo	-e	"\t\tOutput file = "${outFile}


	### prepare root macro ###
	rootMacro=${jobDir}/${jobName}.C
	listFileName=$(basename "${fileListPath}")
	cp ${macroTemplate} ${rootMacro}
	sed -i 's|#macroname|'${jobName}'|g' ${rootMacro}
	sed -i 's|#fileList|'${listFileName}'|g' ${rootMacro}
	sed -i 's|#outfilepath|'${outFile}'|g' ${rootMacro}
	sed -i 's|#ccfilepath|'${ccfilename}'|g' ${rootMacro}
	sed -i 's|#mcPU|'${mcPUdist}'|g' ${rootMacro}
	sed -i 's|#dataPU|'${dataPUfile}'|g' ${rootMacro}


	### prepare run script ###
	runScript=${jobDir}/${jobName}.sh
	cp ${runScriptTemplate} ${runScript}
	sed -i 's|#cmsswdir|'${cmsswDir}'|g' ${runScript}
	sed -i 's|#macrofile|'${jobName}.C'|g' ${runScript}


	chmod +x ${runScript}


	if [[ -z "${mcPUdist// }" ]]; then
		fileListPath=${fileListPath}", "${mcPUdist}", "${dataPUfile}
	fi

	### prepare condor script ###
	condorCFG=${jobDir}/condor_${jobName}.sh
	cp ${condorCFGtemplate} ${condorCFG}
	sed -i 's|#script|'${runScript}'|g' ${condorCFG}
	sed -i 's|#logDir|'${logDir}'|g' ${condorCFG}
	sed -i 's|#jobname|'${jobName}'|g' ${condorCFG}
	sed -i 's|#ccfile1|'${ccfil1epath}'|g' ${condorCFG}
	sed -i 's|#ccfile2|'${ccfil2epath}'|g' ${condorCFG}
	sed -i 's|#hfile|'${hfilepath}'|g' ${condorCFG}
	sed -i 's|#rootmacro|'${rootMacro}'|g' ${condorCFG}
	sed -i "s|#filelist|${fileListPath}|g" ${condorCFG}
	sed -i 's|#outfile|'${outFile}'|g' ${condorCFG}
	sed -i 's|#jobflavour|'${jobflavor}'|g' ${condorCFG}
	chmod +x ${condorCFG}

	cd ${jobDir}
	condor_submit ${condorCFG}
	cd ${workDir}
}

mkdir -p ${writeDir}
cp ${ccfil1epath} ${writeDir}/${ccfilename}

ccfil1epath=${writeDir}/${ccfilename}

[ ! -f $jobList ] && { echo "$jobList file not found"; exit 99; }

while IFS=, read -r dataset xSec singleJobFileList mcPUfile
do

	if [[ ! $singleJobFileList =~ "SinglePhoton" ]]
	then
		continue;
	fi


	echo -e "\n Preparing job:\n \t Dataset = "${dataset} "\n \t Ntuple List = "${singleJobFileList} "\n \t xSec = "${xSec} "\n \t mc pileup file = "${mcPUfile}

	jobBaseName=$(basename "${singleJobFileList}")
	jobBaseName="${jobBaseName%.*}"
	jobBaseName=$(echo ${jobBaseName} | tr -cd [:alnum:])


	jobDir=${jobsDir}/${jobBaseName}/

	rm -rf ${jobDir}

	if [ -d "${jobDir}" ]; then
		echo -e "\t Error! Job directory already exists "${jobDir}
		exit
	fi


	writeOutDir=${writeDir}/${jobBaseName}/out/
	if [ -d "${writeOutDir}" ]; then
		echo -e "\t Error! Write directory already exists "${writeOutDir}
		exit
	fi


	### log directory ###
	logDir=${jobDir}/log/

	## make log and write directories ###
	mkdir -p ${logDir}
	mkdir -p ${writeOutDir}

	nFiles=$(sed -n '=' ${singleJobFileList} | wc -l)
	echo	-e	"\t # of files in base job = " ${nFiles}

	split -d -a 3 -l ${splitfiles} ${singleJobFileList} ${jobDir}/${jobBaseName}_

	for subJobList in $(find "${jobDir}" -name "${jobBaseName}_*");
	do
		jobName=$(basename ${subJobList})
		jobName="${jobName%.*}"
		echo -e	"\t Submitting "${jobName}
		preSelectDtaset ${subJobList} ${jobName} ${jobDir} ${writeOutDir} ${mcPUfile} ${dataPUfile}
	done

done <${jobList}


echo "Submission complete!"
current_date_time=$(date +%Y-%m-%d_%H-%M-%S)
echo $current_date_time;
