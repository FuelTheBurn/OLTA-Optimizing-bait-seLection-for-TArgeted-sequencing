# OLTA: Optimizing bait seLection for TArgeted sequencing
OLTA is a tool for computing capture probes for the targeted enrichment of DNA sequences.

Link to paper: TBA

[Intuition behind the algorithm](https://docs.google.com/document/d/1adsqc5_eGKedFmZoRJNOyW5d8rmos0pYJiEGGI2z050/edit?usp=sharing)

## Problem definition & limitations

OLTA is designed to solve the [Minimum Bait Cover problem](https://academic.oup.com/bioinformatics/article/38/Supplement_1/i177/6617487?login=true) as defined by Alanko _et al._ The algorithm does not account for practical considerations not reflected in the abstraction of the problem, including but not limited to:
* GC bias
* Repeat content
* Interference among probes

Candidate probes produced by OLTA should be postprocessed for the specifics of your application.

## Setup:

OLTA needs [g++](https://gcc.gnu.org) for compilation. After cloning the repository, run the following:
```
cd MultithreadedGenerativeSearchV4WithInput
g++ -std=c++14 -O2 -pthread -o MultithreadedGenerativeSearchV4WithInput main.cpp
```
Ignore the warnings, as long as none of them are fatal

The program was written as a CLion project, so the executable needs to be moved outside.
```
cd ..
move ./MultithreadedGenerativeSearchV4WithInput/MultithreadedGenerativeSearchV4WithInput.exe .
```
The main program depends on two helper programs for data preprocessing and output formatting. These can be compiled as follows:
```
g++ -std=c++14 -O2 -o dataCleaning dataCleaning.cpp
g++ -std=c++14 -O2 -o numberProbes numberProbes.cpp
```

## Usage
**Preprocessing:** Input sequences need to be preprocessed prior to running the main algorithm. This is to replace any non-ATGC characters with 'N' characters, which does not match with itself as per the problem definition. 

```
Preprocess sequences to replace non-ATGC characters.
Usage: numberProbes <input> <output> <nt>
    Positional arguments:
        input    Path to the input sequences, in FASTA format
        output   Path where the output will be written.
        nt       Number of nucleotides to be extracted and cleaned from the provided input file. If -1, the entire file will be extracted and cleaned. If nt nucleotides from the start of the file end up in the middle of a sequence, that entire sequence will be included as well.
    Optional arguments:
        -rn      If provided, non-ATGC characters are removed entirely instead of being replaced.
```
**Finding a bait cover:** The cleaned-up input file can then be used as an input for the main program.
```
Compute a set of capture probes for the provided sequences.
Usage: MultithreadedGenerativeSearchV4WithInput <input> <output> <L> <d> <b> <k> <K_1> <K_2> ... <K_k>
    Positional arguments:
        input    Path to the input sequences, outputted by numberProbes.
        output   Path where the output will be written.
        L        Length of the baits to be produced.
        d        Maximum hamming distance for bait binding.
        b        Bucket size for OLTA.
        k        Number of search breadths for OLTA. There should be exactly k positional arguments following this.
        K_i      i'th search breadth for OLTA.

    Optional arguments:
        -lr1     Lenient radius 1 (default: d * 2).
        -lr2     Lenient radius 2 (default: d * 2).
        -bh      Bypass hyperparameter (default: d).
        -t       Number of threads (default: std::thread::hardware_concurrency())
```
Increasing the `b` and `k` parameters generally decreases the number of probes at the expense of increased running time. For `L = 120`, we recommend the settings `b = 10`, `k = 4`, `K = [10, 10, 10, 2]`.

Note that if the number of N's in any bait length subsequence in the input is greater than the hamming distance mismatch tolerance, it is impossible to cover this sequence and the program will print "Note: the coverage will not be 100% due to there being regions with more Ns than the max hamming distance tolerance".

**Numbering probes:** OLTA's default output is a `.txt` file where every line contains one of the computed probes. You can assign sequential ID's to these probes with one of the provided helper programs.
```
Assign ID's to probes.
Usage: numberProbes <input> <output>
    Positional arguments:
        input    Path to the input probes outputted by MultithreadedGenerativeSearchV4WithInput
        output   Path where the output will be written.
```

### Example
As an example, we will create a bait set for the sequences in `megaresPartitions/megaresClean25E4.txt`, which are the first approximately 250000 nucleotides from `megares_database_v3.00.fasta`, from [https://www.meglab.org/megares](https://www.meglab.org/megares)

For demonstrative purposes, we start by deleting the file.
```
rm ./megaresPartitions\megaresClean25E4.txt
```
To recreate it, we run the following:
```
./dataCleaning.exe megares_database_v3.00.fasta megaresPartitions/megaresClean25E4.txt 250000
```
To create a bait set with the parameters `L = 120`, `d = 50`, `b = 5`, `K = [10, 10, 10, 2]`, we run the following:
```
./MultithreadedGenerativeSearchV4WithInput.exe ./megaresPartitions/megaresClean25E4.txt ./output.txt 120 40 5 4 10 10 10 2
```
To assign IDs to the outputted baits, we run:
```
./numberProbes ./output.txt ./numberedOutput.txt
```
For information on coverage metrics like workload and redundancy, the analyzer repo can be found here: https://github.com/mminbay/olta_analyzer/
