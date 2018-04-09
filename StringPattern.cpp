#include"MyOutput.hpp"

int BruteForceStrMatch(const char *s,const char *p){

    int strLen = strlen(s);
    int pLen = strlen(p);
    for(int i=0;i<strLen;i++){
        /**
         * Implementation one!
         * 
         */ 
        // bool matchOver = true;
        // for(int j=0;j<pLen && matchOver;j++){
        //     if(s[i+j] != p[j]){
        //         matchOver = false;                
        //     }
        // }
        // if(matchOver){
        //     return i;
        // }


        /**
         * Implementation two!
         * 
         * 是不是很简洁了呢
         */ 
        int j=0;
        //为什么j<pLen要放在前面?
        while(j<pLen && s[i+j]==p[j])j++;
        if(j==pLen){
            return i;
        }

    }

    return -1;
}


void MyGetNext(const char *p,int next[]){
    int pLen = strlen(p);
    int j=0;
    for(int i=1;i<pLen;i++){
        if(p[j] == p[i]){
            next[i] = j;
            j++;
        }
        else{
            j=0;
            next[i] = j;
        }
    }
}

void TestMyGetNext(char *p){
    int *next = new int[strlen(p)]();
    printlnArr(next,strlen(p));
    MyGetNext(p,next);
    printlnArr(next,strlen(p));
    delete [] next;
    
}

int MyMpStrMatch(const char *s,const char *p){
    int strLen = strlen(s);
    int pLen = strlen(p);
    int *next = new int[pLen]();
    // printlnArr(next,pLen);
    MyGetNext(p,next);
    // printlnArr(next,pLen);
    int j = 0;
    int i = 0;
    while(i<strLen){
        if(s[i] == p[j]){
            j++;
        }
        else{
            j = next[j];
            // println(j);
        }
        
        i++;
        if(j == pLen){
            return i-j;
        }
    }
    delete [] next;
    return -1;
}

void preMp(const char*p,int m,int mpNext[]){

    int i,j;
    i=0;
    j = mpNext[0] = -1;
    while(i<m){
        while(j > -1 && p[i] != p[j])
            j=mpNext[j];
        mpNext[++i] = ++j;
    }

}



int main(){
    char a[] = "abdabeabcabcabdabcabcabcefd";
    char p[] = "abcabcabc"; 
    println("string = ",a);
    println("pattern str = ",p);
    TimeSpec::begin();
    int index = BruteForceStrMatch(a,p);
    TimeSpec::print_cost_time();
    println("match first index = ",index);
    println("rest of match str = ",a+index);

    println("===================================");

    TestMyGetNext(p);
    int *next = new int[strlen(p)+1]();
    preMp(p,strlen(p),next);
    printlnArr(next,strlen(p)+1);
    delete []next;

    // println("===================================");
    // println("string = ",a);
    // println("pattern str = ",p);
    // TimeSpec::begin();
    // index = MyMpStrMatch(a,p);
    // TimeSpec::print_cost_time();
    // println("match first index = ",index);
    // println("rest of match str = ",a+index);

    println("===================================");
    char *a2 = "ctcaatcacaatcat";
    char *p2 = "caatcat";
    println("string = ",a2);
    println("pattern str = ",p2);
    TestMyGetNext(p2);
    next = new int[strlen(p2)+1]();
    preMp(p2,strlen(p2),next);
    printlnArr(next,strlen(p2)+1);
    delete []next;
    // TimeSpec::begin();
    // index = MyMpStrMatch(a2,p2);
    // TimeSpec::print_cost_time();
    // println("match first index = ",index);
    // println("rest of match str = ",a2+index);

    return 0;
}



