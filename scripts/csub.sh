#!/bin/bash

cmsswDir=/hdfs/store/user/mwadud/CMSSW_9_4_13/src/
workDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"/
writeDir=/scratch/mwadud/preSelected/jobs/
jobsDir=/scratch/mwadud/preSelected/jobs/

jobList=$1





############################################################################
jobflavor=tomorrow
splitfiles=50
macroTemplate=${workDir}/macroTemplate.C
runScriptTemplate=${workDir}/submit_job.sh
condorCFGtemplate=${workDir}/condor_job.sh
ccfilepath=${workDir}/../macros/eventPreselector.cc
hfilepath=${workDir}/../macros/eventPreselector.h

current_date_time=$(date +%Y-%m-%d_%H-%M-%S)
echo $current_date_time;


source /cvmfs/cms.cern.ch/cmsset_default.sh;
cd ${cmsswDir}; eval `scramv1 runtime -sh`; cd -;
cd ${workDir}

echo -e "\n\n"

echo 'Work directory = '${workDir}''
echo 'Job directory = '${jobsDir}''
echo 'Write directory = '${writeDir}''
echo 'Job list = '${jobList}''


function preSelectDtaset(){
	fileListPath=$1
	jobName=$2
	jobDir=$3
	writeOutDir=$4

	echo	-e		"\t\t Making jobs for " ${fileListPath}
	echo 	-e		"\t\t Job name "${jobName}
	echo 	-e		"\t\t Job directory "${jobDir}
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
	sed -i 's|#ccfilepath|'eventPreselector.cc'|g' ${rootMacro}


	### prepare run script ###
	runScript=${jobDir}/${jobName}.sh
	cp ${runScriptTemplate} ${runScript}
	sed -i 's|#cmsswdir|'${cmsswDir}'|g' ${runScript}
	sed -i 's|#macrofile|'${jobName}.C'|g' ${runScript}


	chmod +x ${runScript}


	### prepare condor script ###
	condorCFG=${jobDir}/condor_${jobName}.sh
	cp ${condorCFGtemplate} ${condorCFG}
	sed -i 's|#script|'${runScript}'|g' ${condorCFG}
	sed -i 's|#logDir|'${logDir}'|g' ${condorCFG}
	sed -i 's|#jobname|'${jobName}'|g' ${condorCFG}
	sed -i 's|#ccfile1|'${ccfilepath}'|g' ${condorCFG}
	sed -i 's|#hfile|'${hfilepath}'|g' ${condorCFG}
	sed -i 's|#ccfile2|'${rootMacro}'|g' ${condorCFG}
	sed -i 's|#filelist|'${fileListPath}'|g' ${condorCFG}
	sed -i 's|#outfile|'${outFile}'|g' ${condorCFG}
	sed -i 's|#outdir|'${writeOutDir}'|g' ${condorCFG}
	sed -i 's|#jobflavour|'${jobflavor}'|g' ${condorCFG}
	chmod +x ${condorCFG}

	condor_submit ${condorCFG}
#	farmoutAnalysisJobs  --fwklite --infer-cmssw-path antgc  ${runScript}
}


while IFS= read -r singleJobFileList
do
	echo -e "\n Preparing job from list file " ${singleJobFileList}

	jobBaseName=$(basename "${singleJobFileList}")
	jobBaseName="${jobBaseName%.*}"

	jobDir=${jobsDir}/${jobBaseName}/
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


	### make log and write directories ###
	mkdir -p ${logDir}
	mkdir -p ${writeOutDir}

	nFiles=$(sed -n '=' ${singleJobFileList} | wc -l)
	echo	-e	"\t # of files in base job = " ${nFiles}

	split -d -a 3 -l ${splitfiles} ${singleJobFileList} ${jobDir}/${jobBaseName}_

	for subJobList in $(find "${jobDir}" -name "${jobBaseName}_*");
	do
		echo -e "\n\n" ${subJobList}
		jobName=$(basename "${subJobList}")
		jobName="${jobName%.*}"
		echo -e	"\t Submitting "${subJobList}
		preSelectDtaset ${subJobList} ${jobName} ${jobDir} ${writeOutDir}
	done

done <${jobList}



echo "Submission complete!"
current_date_time=$(date +%Y-%m-%d_%H-%M-%S)
echo $current_date_time;
