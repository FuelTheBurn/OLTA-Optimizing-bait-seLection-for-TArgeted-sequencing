# OLTA: Optimizing bait seLection for TArgeted sequencing
Finds a bait cover that covers 100% of a given sequence.

Link to paper: TBA

[Intuition behind the algorithm](https://docs.google.com/document/d/1adsqc5_eGKedFmZoRJNOyW5d8rmos0pYJiEGGI2z050/edit?usp=sharing)

# How to set up on Windows with g++:
For Linux, the Linux equivalent commands can be used. Note that if you use Linux, you will have to recreate the cleaned fasta file, megaresPartitions\megaresClean25E4.txt (explained below) because of an issue where newlines on Linux and Windows are different.

First, clone the repo. Inside of the OLTA-Optimizing-bait-seLection-for-TArgeted-sequencing folder do the following:
```
cd MultithreadedGenerativeSearchV4WithInput
g++ -std=c++14 -O2 -pthread -o MultithreadedGenerativeSearchV4WithInput main.cpp
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
.\MultithreadedGenerativeSearchV4WithInput.exe megaresPartitions/megaresClean25E4.txt output.txt 120 40 5 4 10 10 10 2
```
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
megaresPartitions/megaresClean25E4.txt -> Input file
output.txt -> Output file
120 -> Bait length
40 -> Hamming distance mismatch tolerance
5 -> Bucket size, THIS ONE MATTERS MOST, EXPLANATION BELOW
4 -> Number of search breadths/branching factors (Just keep these as constants), because it is 4, 4 numbers will follow
10 -> First search breadth/branching factor
10 -> Second search breadth/branching factor
10 -> Third search breadth/branching factor
2 -> Fourth search breadth/branching factor
```
You can also run additional optional inputs like this:
```
.\MultithreadedGenerativeSearchV4WithInput.exe megaresPartitions/megaresClean25E4.txt output.txt 120 40 5 4 10 10 10 2 -lr1 80 -lr2 80 -bh 40 -t 8
```
Explanation of the additional optional inputs:
```
-lr1 80 -> Lenient radius, if not given, automatically set this to 2 x (Hamming distance mismatch tolerance)
-lr2 80 -> Lenient radius 2, if not given, automatically set this to 2 x (Hamming distance mismatch tolerance)
-bh 40 -> Bypass hyperparameter, if not given, automatically set this one to (Hamming distance mismatch tolerance)
-t 8 -> Number of threads, if not given, automatically set this one to (std::thread::hardware_concurrency();)
```
You can mix and match the optional inputs. The order does not matter so long as they are all at the end, so the following would also work:
```
.\MultithreadedGenerativeSearchV4WithInput.exe megaresPartitions/megaresClean25E4.txt output.txt 120 40 5 4 10 10 10 2 -t 8 -bh 40 -lr2 80 
```
There is a bait-time trade-off that can be adjusted depending on whether there is a need to reduce baits or reduce the running time by changing the bucket size. For our test configuration of bait length 120 and mismatch tolerance 40, experimentally, the minimum number of baits occurs at bucket size 30. Run time is the least at bucket size 1 and is positively correlated with bucket size. For bait length 60 and mismatch tolerance 6, the number of baits is the least around bucket size 10. For other bait lengths and mismatch tolerances, play around.
# The "modified" fasta file

The "modified" fasta file strips the sequence names, makes everything uppercase, and sets any non-ACTG nucleotide to N

To turn your fasta file into a modified fasta file compile dataCleaning:
```
g++ -std=c++14 -O2 -o dataCleaning dataCleaning.cpp
```
Ignore the warnings, as long as none of them are fatal

Now, I will recreate megaresPartitions\megaresClean25E4.txt, which is the first around 250000 nucleotides from megares_database_v3.00.fasta, from [https://www.meglab.org/megares](https://www.meglab.org/megares)

So let's delete megaresPartitions\megaresClean25E4.txt first:
```
del .\megaresPartitions\megaresClean25E4.txt
```
And now recreate it:
```
.\dataCleaning.exe megares_database_v3.00.fasta megaresPartitions\megaresClean25E4.txt 250000
```
If you wish to remove unknown nucleotides, add -rn like this:
```
.\dataCleaning.exe megares_database_v3.00.fasta megaresPartitions\megaresClean25E4.txt 250000 -rn
```

If you want to include every nucleotide in the entire file, put -1 as the length

Otherwise, the program will replace them with N, representing an unknown nucleotide. This means that if the hamming distance mismatch tolerance is set to 40, and bait length is set to 120, then in order for a bait to bind to a length 120 sequence with 5 N's in it, it must have a maximum hamming distance mismatch tolerance of 35 with the other known nucleotides.

Note that if the number of N's in any bait length subsequence in the input is greater than the hamming distance mismatch tolerance, the problem is undefined, and it will print "Note: the coverage will not be 100% due to there being regions with more Ns than the max hamming distance tolerance".

And we have restored the file we deleted

By "around", the program will count 250000 nucleotides, and if it ends in the middle of a sequence, it will include the rest of the current sequence.

If you want to number the probes (assign them IDs), first compile numberProbes.cpp

```
g++ -std=c++14 -O2 -o numberProbes numberProbes.cpp
```
Then run it by
```
.\numberProbes output.txt numberedOutput.txt
```

For information on coverage metrics like workload and redundancy, the analyzer repo can be found here: https://github.com/mminbay/olta_analyzer/
