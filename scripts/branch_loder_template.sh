#!/bin/bash

while IFS=, read -r col1 col2
do
	# echo $col1    $col2
	if [[ $col1 == *"vector"* ]]; then
		bType=${col1#"vector<"}
		bType=${bType%">"}
		bName=${col2}
		echo "TTreeReaderVectorValue<${bType}> _${bName};"
		# echo "std::vector<${bType}> ${bName}_;"
	else
		btype=${col1}
		bName=${col2}
		echo "TTreeReaderAnyValue<${btype}> _${bName};"
		# echo "${btype}> ${bName}_;"
	fi
done < branch_list.txt
