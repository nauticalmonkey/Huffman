make 
./hencode ./inputs/test1 ./outputs/test1.out
xxd -b ./outputs/test1.out
echo "\n\n"
xxd -b ./expected/test1.expected
