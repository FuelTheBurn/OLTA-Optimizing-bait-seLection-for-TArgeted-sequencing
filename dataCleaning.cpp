//#include <bits/stdc++.h>
#include <stdio.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>


using namespace std;



using ll = long long;
using ld = long double;
using db = double;
using str = string;

typedef long long ll;
typedef vector<ll> vl; 
typedef vector<pair<ll,ll>> vpl; 


#define FOR(i,a,b) for (int i = (a); i < (b); ++i)
#define F0R(i,a) FOR(i,0,a)
#define ROF(i,a,b) for (int i = (b)-1; i >= (a); --i)
#define R0F(i,a) ROF(i,0,a)
#define trav(a,x) for (auto& a: x) //will not work for bool
#define travBool(a,x) for (auto a: x)
#define print(x) trav(a,x){cout<<a<<" ";}cout<<endl; //will not work for bool
#define printPair(x) trav(a,x){cout<<"{"<<a.first<<" "<<a.second<<"} ";}cout<<endl; //will not work for bool
#define printBool(x) travBool(a,x){cout<<a<<" ";}cout<<endl;
#define print2d(x) trav(a,x){trav(b,a){cout<<b<<" ";}cout<<endl;}
#define print2dBool(x) trav(a,x){travBool(b,a){cout<<b<<" ";}cout<<endl;}
#define doPrefixSum2d(data, prefixSum,x,y){prefixSum.resize(x,vector<ll>(y,0));F0R(i,x){prefixSum[i][0]=0;}F0R(j,y){prefixSum[0][j]=0;}FOR(i,1,x){FOR(j,1,y){prefixSum[i][j]=data[i][j]+prefixSum[i-1][j]+prefixSum[i][j-1]-prefixSum[i-1][j-1];}}}//x and y are the size of prefixSum and data including the first row of 0s
#define printQueue(q){auto copyOfQ=q;if(copyOfQ.empty()){cout<<"empty";}while(!copyOfQ.empty()){cout<<copyOfQ.front()<<" ";copyOfQ.pop();}cout<<endl;}
#define printDeque(q){auto copyOfQ=q;if(copyOfQ.empty()){cout<<"empty";}while(!copyOfQ.empty()){cout<<copyOfQ.front()<<" ";copyOfQ.pop_front();}cout<<endl;}
#define printDequePair(q){auto copyOfQ=q;if(copyOfQ.empty()){cout<<"empty";}while(!copyOfQ.empty()){cout<<"{"<<copyOfQ.front().first<<" "<<copyOfQ.front().second<<"} ";copyOfQ.pop_front();}cout<<endl;}
#define printStack(q){auto copyOfQ=q;if(copyOfQ.empty()){cout<<"empty";}while(!copyOfQ.empty()){cout<<copyOfQ.top()<<" ";copyOfQ.pop();}cout<<endl;} //printed backwards, last in first out occurs first in the print

#define pb push_back
#define pf push_front //for deques
#define rsz resize
#define sz(x) int(x.size())
#define all(x) begin(x), end(x)
#define f first
#define s second

const int MOD = 1e9+7; // 998244353; //this is a completely different unrelated prime number
//const __int128 MOD = 13000000000000019; prime number, for hashing shit
const int MX = 2e5+5;
const ll INF = 1e18; // not too close to LLONG_MAX
const ld PI = acos((ld)-1);

bool debug=false;
bool sortDecreasing(const ll& x, const ll& y){// same as greater<int>(), built in c++ comparator
    return x>=y;
}
bool ysort(const pair<ll,ll>& x, const pair<ll,ll>& y){//sorts pairs of x and y
    return x.s<=y.s;
}

bool ckmin(ll& a, ll b){ return b < a ? a = b, true : false; }//returns false if a was unchanged
bool ckmin(int& a, int b){ return b < a ? a = b, true : false; }//returns false if a was unchanged
bool ckmax(ll& a, ll b){ return b > a ? a = b, true : false; }//returns false if a was unchanged
bool ckmax(int& a, int b){ return b > a ? a = b, true : false; }//returns false if a was unchanged

//We just always do all, DNALength will be set by length of input
//Actually DNALength does not really even matter, compressedWindowLength is the one that really matters


//const ll naiveChecks=min(DNAAmount-1,100);


vector<string> stringDNA;
ll DNALength;
string opFile;
string inpFile;
void readInFile(){
    // Open a text file for reading
    
    std::ifstream inputFile(inpFile);
    //std::ifstream inputFile("input.txt");

    // Check if the file is open
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
    }
    // Read and print the contents of the file
    std::string line;
    ll As=0;
    ll Cs=0;
    ll Gs=0;
    ll Ts=0;
    ll Ns=0;
    ll badLetters=0;
    ll totalLength=0;
    ll lineCounter=0;
    while (totalLength<DNALength && std::getline(inputFile, line)) {
    	lineCounter++;
    	if(lineCounter%2==1){
    		continue;
    	}
    	//std::cout << line << std::endl;
    	string fileData="";
		for(char &a:line){
			totalLength++;
			if(a=='A'||a=='a'){
				fileData+='A';
				As++;
			}
			else if(a=='C'||a=='c'){
				fileData+='C';
				Cs++;
			}
			else if(a=='G'||a=='g'){
				fileData+='G';
				Gs++;
			}
			else if(a=='T'||a=='t'){
				fileData+='T';
				Ts++;
			}
			else{
				fileData+='N';
				Ns++;
				badLetters++;
				cout<<a<<endl;
			}
		}

			//cout<<fileData.size()<<endl;
		stringDNA.pb(fileData);

		

    }

    // Close the file
    cout<<"Total length: "<<totalLength<<endl;
    cout<<"A: "<<As<<" C: "<<Cs<<" G: "<<Gs<<" Ts "<<Ts<<" Ns "<<Ns<<endl;
    if(badLetters==0){
    	cout<<"all ACGT"<<endl;
    }
    else{
    	cerr<<"ERROR! some values not ACGT"<<endl;
    	cerr<<"problem amount "<<badLetters<<endl;
    }
    DNALength=totalLength-badLetters;
    inputFile.close();
}
void writeOutput();

int main(int argc, char* argv[]) {
	if(argc!=4){
		cerr<<"Wrong number of inputs!!!"<<endl;
	}
	inpFile=string(argv[1]);
	opFile=string(argv[2]);
	DNALength=stoll(argv[3]);
	if(DNALength==-1){
		DNALength=1e18;
	}
	readInFile();
	// std::random_device rd;
    // std::mt19937 g(rd());

    // // Shuffle the vector
    // std::shuffle(stringDNA.begin(), stringDNA.end(), g);

    // Print the shuffled vector
    // for (const auto& word : stringDNA) {
    //     std::cout << word << " ";
    // }
    // std::cout << std::endl;
	writeOutput();
}
void writeOutput(){//first line is the DNA
	//next line is N, number of baits
	//next N lines are the baits
	
    ofstream outputFile(opFile);

    // Check if the file is open
    if (!outputFile.is_open()) {
        std::cerr << "Unable to open the file." << std::endl;
    }
    for(int i=0;i<stringDNA.size();i++){
    	outputFile<<stringDNA[i]<<endl;
    }
    
    // Write data to the file
    outputFile.close();
    
    std::cout << "Data written to the file successfully." << std::endl;	
}