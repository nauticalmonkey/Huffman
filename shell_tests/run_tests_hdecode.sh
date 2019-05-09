#!/bin/bash
for file in $( ls ~/CPE357/hw/hw3/src/huffman/hdecode_tests/inputs/* ); do
	 echo -n "Testing with ${file} -- "
	 BASE=`basename ${file} .in`
	 #program testing

	 ../hdecode ${file} ../hdecode_tests/outputs/${BASE}.out

	 if [ $? -eq 0 ]; then
		 echo "succeeded for running program"
	 fi
done




for file in $( ls ~/CPE357/hw/hw3/src/huffman/hdecode_tests/outputs/* ); do
	 echo -n "Testing with ${file} -- "
	 BASE=`basename ${file} .out`
	 #program testing

	 diff ${file} ../hdecode_tests/expected/${BASE}.huff.expected

	 if [ $? -eq 0 ]; then
		 echo "succeeded difference"
	 fi
done



