#!/bin/bash
for file in $( ls ../inputs/* ); do
	 echo -n "Testing with ${file} -- "
	 BASE=`basename ${file}`
	 #program testing

	diff `xxd ../outputs/${BASE}.out` `xxd ../expected/${BASE}.expected` 

	if [ $? -eq 0 ]; then
		 echo "succeeded"

	fi


done



