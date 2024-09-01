# OLTA: Optimizing bait seLection for TArgeted sequencing
Finds a bait cover that covers 100% of a given sequence.
# How to set up on Windows:
Clone the repo. Inside of the OLTA-Optimizing-bait-seLection-for-TArgeted-sequencing folder do the following:
```
cd MultithreadedGenerativeSearchV4WithInput
g++ -O2 -pthread -o MultithreadedGenerativeSearchV4WithInput main.cpp
```
Ignore the warnings, as long as none of them are fatal

I wrote MultithreadedGenerativeSearchV4WithInput as a CLion project, so we need to move our executable outside
```
cd ..
move .\MultithreadedGenerativeSearchV4WithInput\MultithreadedGenerativeSearchV4WithInput.exe .
```
# Finding a bait cover
The program takes in a "modified" fasta file, in this example, it is megaresParitions/megaresClean25E4.txt

NOTE: THIS TAKES DOES NOT TAKE IN A FASTA FILE, I WILL EXPLAIN LATER HOW TO CONVERT A FASTA FILE TO A "MODIFIED" FASTA FILE

The input reading is done with c++ cin

The following input is suboptimal for the least number of baits/fast for demonstration purposes, to know what inputs you should use, read the paper
```
.\MultithreadedGenerativeSearchV4WithInput.exe
120
40
80
80
5
40
4
10
10
10
2
megaresPartitions/megaresClean25E4.txt
output.txt
```
NOTE: DO NOT TYPE EVERYTHING ON ONE LINE, IT WON'T WORK

In the wall of print statements, towards the end, you should see something that looks like this:
```
Time measured: 6.073 seconds
Baits/Centroids: 422
```
This means that the program has found a bait covering with 457 baits.

output.txt contains the baits, it is now 457 lines long, and each line has 120 characters

Explanation of the inputs:
```
.\MultithreadedGenerativeSearchV4WithInput.exe
120 -> Bait length
40 -> Hamming distance mismatch tolerance
80 -> Lenient radius, set this to 2 x (Hamming distance mismatch tolerance)
80 -> Lenient radius 2, set this to 2 x (Hamming distance mismatch tolerance)
5 -> Bucket size, THIS ONE MATTERS MOST, EXPLANATION BELOW
40 -> Bypass hyperparameter, set this one to (Hamming distance mismatch tolerance)
4 -> Number of search breadths/branching factors (Just keep these as constants)
10 -> First search breadth/branching factor
10 -> Second search breadth/branching factor
10 -> Third search breadth/branching factor
2 -> Fourth search breadth/branching factor
megaresPartitions/megaresClean25E4.txt -> Input file
output.txt -> Output file
```
There is a bait-time trade-off that can be adjusted depending on whether there is a need to reduce baits or reduce the running time by changing the bucket size. For our test configuration of bait length 120 and mismatch tolerance 40, the minimum number of baits occurs at bucket size 30. Run time is the least at bucket size 1 and is positively correlated with bucket size. For bait length 60 and mismatch tolerance 6, the number of baits is the least around bucket size 10. For other bait lengths and mismatch tolerances, play around.
# The "modified" fasta file

The "modified" fasta file strips the sequence names, makes everything uppercase, and sets any non-ACTG nucleotide to N

To turn your fasta file into a modified fasta file compile dataCleaning:
```
g++ -O2 -o dataCleaning dataCleaning.cpp
```
Ignore the warnings, as long as none of them are fatal

Now, I will recreate megaresPartitions\megaresClean25E4.txt, which is the first around 250000 nucleotides from megares_database_v3.00.fasta, from [https://www.meglab.org/megares](https://www.meglab.org/megares)

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
NOTE: AGAIN, DO NOT TYPE EVERYTHING ON ONE LINE, IT WON'T WORK

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

If you want to number the probes (assign them IDs), first compile numberProbes.cpp

```
g++ -O2 -o numberProbes numberProbes.cpp
```
Then run it by
```
.\numberProbes
output.txt
numberedOutput.txt
```
NOTE: AGAIN, DO NOT TYPE EVERYTHING ON ONE LINE, IT WON'T WORK

If you want to automate these processes, do the fasta conversion and run the main program together, find that the typing of these commands becomes too tedious, or confirm that the baits generated actually cover the sequence 100%, open up this project in anaconda prompt and look at the jupyter notebook, BaitCoverNotebook.ipynb.

For information on coverage metrics like workload and redundancy, the analyzer repo can be found here: https://github.com/mminbay/olta_analyzer/
