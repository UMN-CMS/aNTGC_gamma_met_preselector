#!/bin/bash

searchPath=$1
writeDir=$2

function listNtuples(){
	SearchPath=$1
	WriteDir=$2
	outfile=${WriteDir}/${SearchPath##*/}.txt

	find ${path} -name "*.root" -not -path '*/failed/*' -type f
}

for directory in $(find "${searchPath}" -maxdepth 1 -mindepth 1 -type d -not -path '*/\.*' -and -not -path '*/xSecs');
do
	listNtuples ${directory} $2
done
