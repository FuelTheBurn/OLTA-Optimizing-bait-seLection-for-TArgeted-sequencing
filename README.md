# generative-bait-clustering
Finds a bait cover that covers 100% of a given sequence.
# How to set up on Windows:
Clone the repo. Inside of it do the following:
```
cd MultithreadedGenerativeSearchV4WithInput
g++ -Wall -g -O2 -pthread -o MultithreadedGenerativeSearchV4WithInput main.cpp
```
Ignore the warnings, as long as none of them are fatal

I wrote MultithreadedGenerativeSearchV4WithInput as a CLion project, so we need to move our executable outside
```
cd ..
move .\MultithreadedGenerativeSearchV4WithInput\MultithreadedGenerativeSearchV4WithInput.exe .
```
# Finding a bait cover
The program takes in a "modified" fasta file, in this example, it is megaresParitions/megaresClean25E4.txt

The input reading is done with c++ cin

The following input is suboptimal for the least number of baits/fast for demonstration purposes, to know what inputs you should use, read the paper
```
.\MultithreadedGenerativeSearchV4WithInput.exe 120 40 80 80 1 40 4 10 10 10 2 megaresParitions/megaresClean25E4.txt output.txt
```
In the wall of print statements, towards the end, you should see something that looks like this:
```
Time measured: 2.327 seconds
Baits/Centroids: 457
```
This means that the program has found a bait covering with 457 baits.

output.txt contains the baits, it is now 457 lines long, and each line has 120 characters
# The "modified" fasta file
To turn your fasta file into a modified fasta file compile dataCleaning:
```
g++ -Wall -g -O2 -o dataCleaning dataCleaning.cpp
```
Ignore the warnings, as long as none of them are fatal

Now, I will recreate megaresPartitions\megaresClean25E4.txt, which is the first around 250000 nucleotides from megares_database_v3.00.fasta, from [https://www.meglab.org/megares](https://www.meglab.org/megares) (need to add citation for data sets)

So let's delete megaresPartitions\megaresClean25E4.txt first:
```
del .\megaresPartitions\megaresClean25E4.txt
```
And now recreate it:
```
.\dataCleaning.exe
250000
megares_database_v3.00.fasta
```
Now there will be a wall of print statements, and at the end, the program might display something like:
```
ERROR! some values not ACGT
problem amount 30
```
This error is fine, it just means that there are nucleotides in the fasta file that are not ACGT, and the program will replace them with N, representing an unknown nucleotide

Afterwards, type in the output location:
```
megaresPartitions\megaresClean25E4.txt
```
And we have restored the file we deleted

By "around", the program will count 250000 nucleotides, and if it ends in the middle of a sequence, it will include the rest of the current sequence.

If you want to include the entire sequence, put -1 as the length

If you want to automate these processes, or confirm that the baits generated actually cover the sequence 100%, open up this project in anaconda prompt and look at the jupyter notebook.
