#!/bin/bash
for file in $( ls ~/CPE357/hw/hw3/src/huffman/hencode_tests/inputs/* ); do
	 echo -n "Testing with ${file} -- "
	 BASE=`basename ${file} .in`
	 #program testing

	 ../hencode ${file} ../hencode_tests/outputs/${BASE}.out

	 if [ $? -eq 0 ]; then
		 echo "succeeded for running program"
	 fi
done




for file in $( ls ~/CPE357/hw/hw3/src/huffman/hencode_tests/outputs/* ); do
	 echo -n "Testing with ${file} -- "
	 BASE=`basename ${file} .out`
	 #program testing

	 diff ${file} ../hencode_tests/expected/${BASE}.expected

	 if [ $? -eq 0 ]; then
		 echo "succeeded difference"
	 fi
done



