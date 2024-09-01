#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

vector<string> stringDNA;

string inputFileName;
string outputFileName;

void readInFile() {
    std::ifstream inputFile(inputFileName);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        stringDNA.push_back(line);
    }

    inputFile.close();
}

void writeOutput() {
    std::ofstream outputFile(outputFileName);

    if (!outputFile.is_open()) {
        std::cerr << "Unable to open the file." << std::endl;
        return;
    }
    if(stringDNA.size()>=100000000){
    	std::cerr<<"error, more than 100000000 baits"<<std::endl;
    }
    // First line is the DNA
    for (int i = 0; i < stringDNA.size(); ++i) {
        outputFile << "probe-" << std::setw(8) << std::setfill('0') << i << endl;
        outputFile << stringDNA[i] << endl; // Output each line from the stringDNA vector
    }

    outputFile.close();
    std::cout << "Data written to the file successfully." << std::endl;
}

int main() {
    cin >> inputFileName;
    
    cin >> outputFileName;

    readInFile();
    writeOutput();

    return 0;
}
