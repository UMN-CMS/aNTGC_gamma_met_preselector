#!/bin/bash

searchPath=$1
writeDir=$2

function listNtuples(){
	SearchPath=$1
	WriteDir=$2
	outfile=${SearchPath/'crab_'/''}
	outfile=${WriteDir}/${outfile##*/}.txt

	find ${SearchPath} -name "*.root" -not -path '*/failed/*' -type f > ${outfile}
}

for directory in $(find "${searchPath}" -maxdepth 1 -mindepth 1 -type d -not -path '*/\.*' -and -not -path '*/xSecs');
do
	listNtuples ${directory} $2
done
