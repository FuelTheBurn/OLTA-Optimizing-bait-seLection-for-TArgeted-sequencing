#include <iostream>
#include <bits/stdc++.h>
#include <stdio.h>
#include <time.h>


#include <fstream>
#include <string>
#include <mutex>

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
ll DNALength;//500000 100 60 is good test for random
ll windowLength=120;
ll maxRadius=40;//40
ll lenientRadius=80;//80, hitByCoverSpace radius
ll lenientRadius2=80;//80, early termination
ll overlapCount=15;
ll bypassHyperparameter=40;//around maxRadius, skips wfc
string ipf;//inputfile
string opf;//outputfile
//vector<ll> searchBreadths={-1,5,7,8,3,1,1};//make it so that if its out of bounds, its 1
//vector<ll> searchBreadths={-1,20,25,30,10,10,10,10,10};//make it so that if its out of bounds, its 1
//vector<ll> searchBreadths={-1,1000,1000,1000,1000,3,2,1};//make it so that if its out of bounds, its 1

// if u have 100 close together, it is more that one of them will trigger if they are spread out, so maybe try scrambling

bool deterministic=false;

vector<ll> searchBreadths;

vector<pair<int,ll>> hitByCoverSpace;//index

mutex updateCentroid;
vector<uint8_t> bestCentroid;
vector<vector<uint8_t>> compressedCovers;
ll bestStage=0;
ll bestRad=0;
vector<pair<ll,ll>> threadDomain2;

//const ll naiveChecks=min(DNAAmount-1,100);


vector<string> stringDNA;

// vector<bitset<windowLength>> DNA1(DNAAmount);//what is this for? i dont think its being used
// vector<bitset<windowLength>> DNA2(DNAAmount);



//have to do with the windows
vector<vector<uint8_t>> windowDNA;
ll windowCount=0;
vector<ll> leftSpan;
vector<ll> rightSpan;
vector<ll> weight;
vector<ll> realIndex;
vector<ll> permaLeftSpan;
vector<ll> permaRightSpan;
vector<ll> stringStart;
vector<ll> stringEnd;
vector<pair<pair<ll,ll>,pair<ll,ll>>> threadDomain;
ll stringCount;
long long numThreads;
vector<thread> threads;
ll usedThreads;

//have to do with the individual indexes
vector<ll> indexMap;
map<ll,ll> mapToIndex;
vector<bool> covered;

//thread_local vector<uint8_t> solution;

ll rangeStart=0;//inclusive
ll rangeEnd=0;//exclusive
//0 is A, 1 is T, 2 is C, 3 is G
void printB(vector<uint8_t > a){
    for (uint8_t value : a) {
        std::cout << " " << static_cast<int>(value);
    }
    cout<<endl;

}

int slowCompare(ll window1,ll window2){
    ll hammingDistance=0;
    for(int i=0;i<windowLength;i++){
        if(windowDNA[window1][i]==4||(windowDNA[window1][i]!=windowDNA[window2][i])){
            hammingDistance++;
        }
    }
    return hammingDistance;
}
int slowCompare2(vector<uint8_t> window1,ll window2){
//    cout<<"slowCompare2 "<<endl;
//
//    for (uint8_t value : window1) {
//        std::cout << " " << static_cast<int>(value);
//    }
//    cout<<endl;
//    cout<<" vs "<<endl;
//    for (uint8_t value : windowDNA[window2]) {
//        std::cout << " " << static_cast<int>(value);
//    }
    ll hammingDistance=0;
    for(int i=0;i<windowLength;i++){
        if((window1[i]!=windowDNA[window2][i])||(windowDNA[window2][i]==4)){
            //cout<<"increase hd";
            hammingDistance++;
        }
    }
    //cout<<endl;
    //cout<<"hd "<<hammingDistance<<endl;
    return hammingDistance;
}
ll firstUncovered=0;
ll coverThis;
//void clearSolution(){
//    solution.clear();
//}
bool wfc(vector<ll> indexes){

    indexes.pb(0);
    //cout<<"wfc preindex"<<endl;//now we add the first uncovered in

    //print(indexes);
    for(int i=0;i<indexes.size();i++){
        indexes[i]=hitByCoverSpace[indexes[i]].s;
    }
    //cout<<"wfc actualIndex"<<endl;
    //print(indexes);
    if(indexes.size()>=3){//early returns
        if(slowCompare(indexes[indexes.size()-2],indexes[indexes.size()-3])>lenientRadius2){//remember, last index is the original from indexes.pb(0);
            //cout<<"early terminated wfc"<<endl;
            return false;
        }
//        else{
//            cout<<"they are "<<slowCompare(indexes[indexes.size()-2],indexes[indexes.size()-3])<<" apart"<<endl;
//        }

    }
    vector<vector<ll>> char_freq(windowLength,vector<ll>(5,0));
    for(ll i=0;i<indexes.size();i++){
        for(int j=0;j<windowLength;j++) {
            char_freq[j][(int) windowDNA[indexes[i]][j]]++;
        }
    }
    vector<vector<ll>> scoreboard;
    for(int l=0;l<windowLength;l++){
        for (uint8_t n = 0; n < 4; n++) {//we dont consider N, which is 4
            scoreboard.pb({l,n,char_freq[l][n]});
            //index, character, frequency
        }
    }
    sort(scoreboard.begin(), scoreboard.end(), [](const auto& a, const auto& b) {
        return a[2] > b[2];
    });
    //cout<<"scoreboard"<<endl;
//    for(const auto& entry: scoreboard){
//        cout<<entry[0]<<" "<<entry[1]<<" "<<entry[2]<<endl;
//    }
    int max_iter=1;
    for(int i=0;i<max_iter;i++){
        //int decidedStart=0;
        vector<uint8_t> solution(windowLength,-1);
        //solution.clear();
        //solution.resize(windowLength,-1);
        vector<int> distances(indexes.size(),windowLength);
        vector<bool> decided(windowLength,false);
        for(int j=0;j<windowLength;j++){
            //cout<<"fixed "<<j<<" indexes"<<endl;
            int maxLength=0;
            int maxIndex=0;
            for(int k=0;k<indexes.size();k++){
                if(distances[k]>maxLength){
                    maxLength=distances[k];
                    maxIndex=indexes[k];//maxIndex is the current worst off
                }
            }
//            cout<<"decided max index"<<endl;
//            cout<<maxIndex<<endl;
            assert(maxIndex<windowCount);
            int changedIndex=0;
            uint8_t newCharacter=0;
            //cout<<"start scoreboard loop"<<endl;

            for(int i=0;i<scoreboard.size();i++){//you must loop through entire scoreboard because maxIndex changes
                const auto& entry=scoreboard[i];
                if(!decided[entry[0]]&&((windowDNA[maxIndex][entry[0]]==entry[1])||(windowDNA[maxIndex][entry[0]]==4))){
                    //cout<<"this was the decider"<<endl;
                    changedIndex=entry[0];
                    newCharacter=entry[1];
                    assert(newCharacter<4);//Cant be N
                    solution[changedIndex]=newCharacter;

                    decided[entry[0]]=1;
                    break;
                }
            }
            for(int k=0;k<indexes.size();k++) {
                if (windowDNA[indexes[k]][changedIndex] == newCharacter) {
                    distances[k]--;
                }
            }
        }
        for(int i=0;i<windowLength;i++){
            if(decided[i]==0){
                print(indexes);
                        cout<<"scoreboard"<<endl;
                        for(const auto& entry: scoreboard){
                            cout<<entry[0]<<" "<<entry[1]<<" "<<entry[2]<<endl;
                        }
                        cerr<<"error"<<endl;
            }
            assert(decided[i]==1);
        }
        int max_dist=*max_element(distances.begin(),distances.end());

        if(max_dist<=maxRadius){
            //cout<<"valid sol"<<endl;
            //cout<<bestStage<<endl;
            if(bestStage<indexes.size()||(bestStage==indexes.size()&&max_dist<bestRad)){
                updateCentroid.lock();
                if(bestStage<indexes.size()||(bestStage==indexes.size()&&max_dist<bestRad)){
                    //cout<<"wfc new best with ";
                    //print(indexes);
                    //cout<<"generated radius "<<max_dist<<endl;
                    //cout<<"the generated centroid "<<endl;
                    //printB(solution);
                    bestStage=indexes.size();
                    bestCentroid=solution;
                    bestRad=max_dist;
//                        cout<<"scoreboard"<<endl;
//                        for(const auto& entry: scoreboard){
//                            cout<<entry[0]<<" "<<entry[1]<<" "<<entry[2]<<endl;
//                        }
                }
                updateCentroid.unlock();
            }
            //cout<<"wfc success"<<endl;
            return true;
        }
        else{
            //cout<<"wfc fail"<<endl;
            return false;
        }
    }
}



void bruteForceGenerate(vector<ll> indexes,ll lockedUpto){
    //cout<<"bfg"<<endl;
    //print(indexes);
    //cout<<lockedUpto<<endl;ac
    //
    //cout<<(ll)hitByCoverSpace.size()<<endl;
    for(ll i=lockedUpto+1;i<min(lockedUpto+searchBreadths[indexes.size()+1]+1ll,(ll)hitByCoverSpace.size());i++){
        indexes.pb(i);
        if((hitByCoverSpace[i].f<=bypassHyperparameter)||wfc(indexes)){
//            if((hitByCoverSpace[i].f<=bypassHyperparameter)){
//                cout<<"bypassHyperparameter"<<endl;
//            }

            bruteForceGenerate(indexes,i);
        }

        indexes.pop_back();
    }
}
mutex updateCoverSpace;



void searchForCoverSpace(ll start,ll end){
    for(int i=max(firstUncovered,start);i<=end;i++){
        if(weight[i]==0){
            continue;
        }
        int hamDist=slowCompare(coverThis, i);
        if (hamDist <= lenientRadius){
            updateCoverSpace.lock();
            hitByCoverSpace.emplace_back(hamDist,i);
            updateCoverSpace.unlock();
        }
    }

}


vector<ll> actualCentroidSize;
vector<ll> cumulativeCentroidSize;
vector<ll> predictedCentroidSize;
mutex acs;
ll actualCentroidS=0;

void fillWeights(ll start,ll end){
    //cout<<"fillWeights "<<start<<" "<<end<<endl;
    for(ll i=max(firstUncovered,start);i<=end;i++){
        //cout<<"iterate through fill"<<endl;
        if(weight[i]==0){
            continue;
        }
        if (slowCompare2(bestCentroid, i) <= maxRadius){
            acs.lock();
            actualCentroidS++;
            acs.unlock();
            //cout<<"fill this and its neighbors "<<i<<endl;
            ll leftS=leftSpan[i]+i;
            ll rightS=rightSpan[i]+i;
            assert(leftS>=0);
            assert(leftS<windowCount);
            assert(rightS>=0);
            assert(rightS<windowCount);
            for(ll j=leftS;j<=rightS;j++){
                weight[j]=0;
            }
        }
    }
}

void centroidGenerationHelper(ll s1,ll s2,ll e1,ll e2){
    vector<ll> indexes;
    for(int i=s1;i<=min(((ll)hitByCoverSpace.size())-1ll,e1);i++){
        ll secondLoopStart=i+1;
        ll secondLoopEnd=searchBreadths[2]-1;
        indexes.pb(i);
        if(i==s1){
            secondLoopStart=s2;
        }
        else{
            secondLoopEnd=e2;
        }
        for(int j=secondLoopStart;j<=min((ll)hitByCoverSpace.size()-1ll,secondLoopEnd);j++){
            indexes.pb(j);
//            cout<<"startHelper"<<endl;
//            print(indexes);
            wfc(indexes);//gets rid of bypassed error
            bruteForceGenerate(indexes,j);
            indexes.pop_back();
        }
        indexes.pop_back();
    }
}

void generateCentroid(){
    //cout<<"generate centroid for "<<coverThis<<endl;

    threads.clear();
    for(auto& a:threadDomain2){
        threads.emplace_back([a](){
            searchForCoverSpace(a.f,a.s);
        });
    }
    for(auto &a:threads){
        a.join();
    }
    threads.clear();
    cout<<"got hitByCoverSpace, size "<<hitByCoverSpace.size()<<endl;

    sort(hitByCoverSpace.begin(),hitByCoverSpace.end());
    //printPair(hitByCoverSpace);
    if(hitByCoverSpace.size()==0){
        //solution.clear();
        cout<<"something went wrong, cover space is 0"<<endl;
        cerr<<"something went wrong, cover space is 0"<<endl;
    }
    else if(hitByCoverSpace.size()==1){
        //solution.clear();
        cout<<"cover space is 1"<<endl;
        wfc({});//gets rid of bypassed error
    }
    else {
        //solution.clear();
        wfc({1});//gets rid of bypassed error
//        for (auto &a: threadDomain) {
//            threads.emplace_back([a]() {
//                clearSolution();
//            });
//        }
//        for (auto &a: threads) {
//            a.join();
//        }
        threads.clear();
        for (auto &a: threadDomain) {
            threads.emplace_back([a]() {
                centroidGenerationHelper(a.f.f, a.f.s, a.s.f, a.s.s);
            });
        }
        for (auto &a: threads) {

            a.join();
        }
    }
    threads.clear();
    hitByCoverSpace.clear();
    //cout<<"finished local centroid generation"<<endl;
}


//int compare(bitset<windowLength> &bs1a, bitset<windowLength> &bs2a, bitset<windowLength> &bs1b, bitset<windowLength> &bs2b){
//
//    return ((bs1a^bs1b) | (bs2a^bs2b)).count();
//}

void printCompressedWindows(){
    assert(windowCount==windowDNA.size());
    for(int i=0;i<windowCount;i++){
        //prints in little endian
        printB(windowDNA[i]);
        cout<<leftSpan[i]<<" to "<<rightSpan[i]<<endl;
        cout<<"Weight: "<<weight[i]<<endl;
    }
}




//void printCompressedCovers(){
//    if(compressedCovers1.size()!=compressedCovers2.size()) {
//        cerr << "compressed covers are different sizes" << endl;
//        cout << "compressed covers are different sizes" << endl;
//    }
//    else{
//        cout<<"There are "<<compressedCovers1.size()<<" covers"<<endl;
//    }
//    for(int i=0;i<compressedCovers1.size();i++){
//
//        //prints in little endian
//        cout<<compressedCovers1[i]<<endl;
//        cout<<compressedCovers2[i]<<endl;
//    }
//}

vector<string> baitCovering;

void stringToCompressed(){
    ll spacedOutIndexing=0;
    for(string &s:stringDNA){
        stringCount++;
        //stringStart.pb(compressedWindowLength);
        //cout<<compressedWindowLength<<endl;
        ll stringLength=s.size();
        ll compressedStringLength=stringLength-windowLength+1;
        if(stringLength<windowLength){
            std::cout<<"we have a problem, sequence too short\n";
            std::cerr<<"we have a problem, sequence too short with length"<<compressedStringLength<<"\n";
            std::cerr<<"undefined behavior may occur, please fix the input string\n";
        }
        vector<uint8_t> curWindowDNA;
        for(int i=0;i<stringLength;i++){
            if(s[i]=='A'){
                curWindowDNA.pb(0);
            }
            else if(s[i]=='T'){
                curWindowDNA.pb(1);
            }
            else if(s[i]=='C'){
                curWindowDNA.pb(2);
            }
            else if(s[i]=='G'){
                curWindowDNA.pb(3);
            }
            else if(s[i]=='N'){
                curWindowDNA.pb(4);
            }
            else{
                std::cout<<"we have a problem, not all ATCGN\n";
                std::cerr<<"we have a problem, not all ATCGN, there is an "<<stringDNA[i]<<"\n";
                std::cerr<<"undefined behavior may occur, please fix the input string\n";
            }

        }
//        cout<<"string length "<<compressedStringLength<<endl;


        ll i=0;
        vector<ll> starters;
        starters.pb(0);
        vector<ll> group;

        group.pb(0);
//        cout<<"forming starts with overcounting"<<endl;
        while(i<compressedStringLength){

            i+=windowLength-overlapCount+1;
            for(int j=0;j<overlapCount;j++){
                starters.pb(i+j);
                group.pb(i);
            }

        }

//        print(starters);
//        cout<<"correct overcounting"<<endl;
        while(!starters.empty()&&starters.back()>=compressedStringLength-1){//we dont include last index in group

            starters.pop_back();
            group.pop_back();
        }

        starters.pb(compressedStringLength-1);
        group.pb(-1);

//        print(starters);
//        cout<<"fill out spans"<<endl;
        for(int start=0;start<starters.size();start++){
            if(starters[start]==0){
                leftSpan.pb(0);
                rightSpan.pb(0);

            }
            else if(starters[start]==compressedStringLength-1){
                leftSpan.pb(0);
                rightSpan.pb(0);
            }
            else{
                ll backCount=0;
                ll frontCount=0;
                ll tempStart=start;
                tempStart--;
                while(tempStart>=0&&group[tempStart]==group[start]){
                    backCount--;
                    tempStart--;
                }

                leftSpan.pb(backCount);



                tempStart=start;
                tempStart++;
                while(tempStart<group.size()&&group[tempStart]==group[start]){
                    frontCount++;
                    tempStart++;
                }
                rightSpan.pb(frontCount);

            }
        }

//        cout<<"finished spans"<<endl;
//        cout<<"leftSpans"<<endl;
//        print(leftSpan);
//        cout<<"rightSpans"<<endl;
//        print(rightSpan);

        for(auto &start:starters){
            vector<uint8_t> cmpS;
            int Ncounter=0;
            for(int j=0;j<windowLength;j++){
                if(curWindowDNA[start+j]==4){
                    Ncounter++;
                }
                cmpS.pb(curWindowDNA[start+j]);
            }
            assert(Ncounter<=maxRadius);
            windowDNA.pb(cmpS);
            weight.pb(1);
            windowCount+=1;
        }
    }
}


void readInFile(){
    // Open a text file for reading
    //std::ifstream inputFile("megaresClean.txt");

    std::ifstream inputFile(ipf);

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
    while (std::getline(inputFile, line)) {
        if(line.size()<windowLength){
            cerr<<"line is less than window length"<<endl;
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
            else if(a=='N'||a=='n'){
                fileData+='N';
                Ns++;
            }
            else{
                badLetters++;
                cout<<a<<endl;
            }
        }
        if(fileData.size()!=line.size()){
            cerr<<" i dont even know anymore"<<endl;
        }
        stringDNA.pb(fileData);
    }

    // Close the file
    cout<<"Total length: "<<totalLength<<endl;
    cout<<"A: "<<As<<" C: "<<Cs<<" G: "<<Gs<<" Ts "<<Ts<<" Ns "<<Ns<<endl;
    if(badLetters==0){
        cout<<"all ACGTN"<<endl;
    }
    else{
        cerr<<"ERROR! some values not ACGTN"<<endl;
        cerr<<"problem amount "<<badLetters<<endl;
    }
    DNALength=totalLength-badLetters;
    inputFile.close();
}



vector<int> distributionTest(windowLength+1,0);
vector<int> distributionTest2(windowLength+1,0);
vector<string> uncompressedCovers;
void writeOutput();
void uncompress();
ll syottiClusters=0;
clock_t start;
vector<ll> indexUsed;

void generateRandomString(ll amount, ll length){
    //DNALength=amount*length;
    for(int i=0;i<amount;i++){
        string stringRandDNA="";
        for(int j=0;j<length;j++){
            int randomNumber=rand()%4;
            if(randomNumber==0){
                stringRandDNA+="A";
            }
            else if(randomNumber==1){
                stringRandDNA+="T";
            }
            else if(randomNumber==2){
                stringRandDNA+="C";
            }
            else{
                stringRandDNA+="G";
            }
        }
        stringDNA.pb(stringRandDNA);
    }
}

vector<ll> searchBucketStart;
vector<ll> searchBucketEnd;

int main(){
    cin>>windowLength;
    cin>>maxRadius;
    cin>>lenientRadius;
    cin>>lenientRadius2;
    cin>>overlapCount;
    cin>>bypassHyperparameter;
    int searchBreadthsSize;
    cin>>searchBreadthsSize;
    for(int i=0;i<searchBreadthsSize;i++){
        ll sb;
        cin>>sb;
        searchBreadths.pb(sb);
    }
    cin>>ipf;
    cin>>opf;
    numThreads = std::thread::hardware_concurrency();


    cout<<"start"<<endl;
    srand(1);//I use 1 for all base tests
    //EITHER readInFile or generateRandomString, BUT NOT BOTH
    readInFile();
    //generateRandomString(100,1500);
    cout<<"finish loading in string"<<endl;
    cout<<"DNA length: "<<DNALength<<endl;


    cout<<"Total Search start"<<endl;
    start = clock();
    stringToCompressed();

    cout<<"finished compressing string"<<endl;
    cout<<"windowCount "<<windowCount<<endl;
    assert(windowCount==windowDNA.size());
    assert(leftSpan.size()==rightSpan.size());
    assert(windowCount==leftSpan.size());
    //printCompressedWindows();
    clock_t end = clock();
    double elapsed = double(end - start)/CLOCKS_PER_SEC;
    printf("Time measured: %.3f seconds\n", elapsed);
    for(int i=0;i<windowCount;i++){
        searchBreadths.pb(1);
    }
    cout<<"threadDomain2"<<endl;
    ll usedThreads2=min(windowCount,numThreads);
    for(ll i=0;i<usedThreads2;i++){
        ll start=i*(windowCount)/usedThreads2;
        ll end=(i+1ll)*(windowCount)/usedThreads2-1ll;
        threadDomain2.pb({start,end});
    }
    printPair(threadDomain2);

    ll startCombo=0;
    for(int i=1;i<searchBreadths[1];i++){//index 0 will always be original string
        for(int j=0;j<searchBreadths[2];j++){
            startCombo++;
        }
    }
    ll threadDomainLength=max(1ll,(numThreads+startCombo-1)/numThreads);//we round up
    ll counter=0;
    pair<pair<ll,ll>,pair<ll,ll>> tempDomain;
    for(int i=1;i<searchBreadths[1];i++){
        for(int j=i+1;j<i+searchBreadths[2];j++){
            counter++;
            if(counter==1){
                tempDomain.f.f=i;
                tempDomain.f.s=j;
            }
            if(counter==threadDomainLength){
                tempDomain.s.f=i;
                tempDomain.s.s=j;
                threadDomain.pb(tempDomain);
                counter=0;
            }

        }
    }
    tempDomain.s.f=searchBreadths[1];
    tempDomain.s.s=searchBreadths[1]+searchBreadths[2];
    threadDomain.pb(tempDomain);
    cout<<"threadDomain"<<endl;
    for(auto& a:threadDomain){
       cout<<"{"<<a.f.f<<"-"<<a.f.s<<" "<<a.s.f<<"-"<<a.s.s<<"} ";
    }
    cout<<endl;
    //the indexes from searchBreadths, assert(first<second)
    ll percentageDone=0;

    //cout<<"percentage done "<<percentageDone<<endl;

    while(firstUncovered<windowCount){
        if(percentageDone*windowCount/100>=firstUncovered){
            percentageDone++;
            //cout<<"percentage done "<<percentageDone<<endl;//does not exactly add to 100
        }
        //cout<<"firstUncovered "<<firstUncovered<<endl;
        if(weight[firstUncovered]!=0){
            cout<<"firstUncovered "<<firstUncovered<<endl;
            bestStage=0;
            bestRad=windowLength+5;
            ll leftStart=leftSpan[firstUncovered]+firstUncovered;
            ll rightStart=rightSpan[firstUncovered]+firstUncovered;
            for(int i=leftStart;i<=rightStart;i++){
                if(weight[i]==0){
                    continue;
                }
                coverThis=i;
                //cout<<"cover this "<<coverThis<<endl;
                generateCentroid();
//                cout<<"local best centroid is "<<endl;
//                for (uint8_t value : bestCentroid) {
//                    std::cout << " " << static_cast<int>(value);
//                }
//                cout<<endl;
            }
//            cout<<"best centroid is "<<endl;
//            for (uint8_t value : bestCentroid) {
//                std::cout << " " << static_cast<int>(value);
//            }
//            cout<<endl;
            cout<<"bestStage "<<bestStage<<endl;
            cout<<"bestRad "<<bestRad<<endl;
            threads.clear();
            for(auto& a:threadDomain2){
                threads.emplace_back([a](){
                    fillWeights(a.f,a.s);
                });
            }
            for(auto &a:threads){
                a.join();
            }
            threads.clear();
//            cout<<"weights"<<endl;
//            print(weight);
            compressedCovers.pb(bestCentroid);
            actualCentroidSize.pb(actualCentroidS);
            actualCentroidS=0;
            predictedCentroidSize.pb(bestStage);


        }
        firstUncovered++;
    }

    end = clock();
    elapsed = double(end - start)/CLOCKS_PER_SEC;

    printf("Time measured: %.3f seconds\n", elapsed);
    cout<<"Baits/Centroids: "<<compressedCovers.size()<<endl;
    cout<<"average cluster size with no overlap"<<DNALength/(compressedCovers.size()*1.0)<<endl;

    uncompress();
    writeOutput();
    vector<ll> cumulativePredicted;
    ll css=*max_element(actualCentroidSize.begin(),actualCentroidSize.end());
    ll cps=*max_element(predictedCentroidSize.begin(),predictedCentroidSize.end());
    cumulativeCentroidSize.rsz(css+5,0);
    cumulativePredicted.rsz(cps+5,0);

    for(auto& a:actualCentroidSize){
        cumulativeCentroidSize[a]++;
    }
    for(auto& a:predictedCentroidSize){
        cumulativePredicted[a]++;
    }
    cout<<"cumulative actual"<<endl;
    print(cumulativeCentroidSize);
    cout<<"cumulative predicted"<<endl;
    print(cumulativePredicted);
    return 0;
}

void uncompress(){
    string ATCG="";
    for(auto &a: compressedCovers){
        for(int i=0;i<windowLength;i++){
            if(static_cast<int>(a[i])==0){
                ATCG.pb('A');
            }
            else if(static_cast<int>(a[i])==1){
                ATCG.pb('T');
            }
            else if(static_cast<int>(a[i])==2){
                ATCG.pb('C');
            }
            else if(static_cast<int>(a[i])==3){
                ATCG.pb('G');
            }
            else{
                cout<<static_cast<int>(a[i])<<endl;
                assert(false);
            }
        }
        //cout<<"uncompressed to "<<ATCG<<endl;
        uncompressedCovers.pb(ATCG);
        ATCG.clear();
    }
    std::random_device rd;
    std::mt19937 g(deterministic ? 42 : rd());
    std::shuffle(uncompressedCovers.begin(), uncompressedCovers.end(), g);
    cout<<"finished uncompressing"<<endl;
}

void writeOutput(){//first line is the DNA
    cout<<"start writing output"<<endl;
    //next line is N, number of baits
    //next N lines are the baits
    ofstream outputFile(opf);

    // Check if the file is open
    if (!outputFile.is_open()) {
        std::cerr << "Unable to open the file." << std::endl;
    }
    for(auto &a: uncompressedCovers){
        outputFile<<a<<endl;
    }

    // Close the file
    outputFile.close();

    std::cout << "Data written to the file successfully." << std::endl;
}