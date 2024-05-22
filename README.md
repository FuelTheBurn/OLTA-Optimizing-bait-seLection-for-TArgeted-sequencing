# generative-bait-clustering
cd MultithreadedGenerativeSearchV4WithInput
g++ -Wall -g -O2 -pthread -o MultithreadedGenerativeSearchV4WithInput main.cpp
Ignore the warnings, as long as none of them are fatal
cd ..
move .\MultithreadedGenerativeSearchV4WithInput\MultithreadedGenerativeSearchV4WithInput.exe .

.\MultithreadedGenerativeSearchV4WithInput.exe
120
40
80
80
1
40
4
10
10
10
2
megaresParitions\megaresClean25E4.txt
output.txt
Time measured: 2.327 seconds
Baits/Centroids: 457

g++ -Wall -g -O2 -o dataCleaning dataCleaning.cpp
Ignore the warnings, as long as none of them are fatal

del .\megaresPartitions\megaresClean25E4.txt

