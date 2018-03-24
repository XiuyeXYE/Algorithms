#include"MyOutput.hpp"

template<typename T>
void MyShakerSort(T a[],int len){

    int i=0;
    int j=len-1;
    while(i<j){
        //max ,min1,min2,min3
        //==> min1,min2,min3,max
        for(int m=i;m<j;m++){
            if(a[m] > a[m+1]){
                swap(a+m,a+m+1);
            }
        }
        for(int n=j;n>0;n--){
            if(a[n-1] > a[n]){
                swap(a+n-1,a+n);
            }
        }
        i++;
        j--;
    }
}


template<typename T>
void ShakerSort(T a[],int len){
    int i,left=0,right=len-1,shift=0;
    //shift�����ÿ��Լ��ٱȽϴ�����
    //�൱���Ż����㷨
    while(left<right){
        for(i=left;i<right;i++){
            if(a[i]>a[i+1]){
                swap(a+i,a+i+1);
                shift = i;
            }
        }
        right = shift;
        // println(shift);
        for(i=right;i>0;i--){
            if(a[i] < a[i-1]){
                swap(a+i,a+i-1);
                shift=i;
            }
        }
        left = shift;
    }
}

int main(){
    constexpr int len = 10000;
    //���鳤�Ȳ���Ϊ0,���򱨴�
    int a[len] = {0};
    createRandomArrayWithBase(a,len,len);
    // printlnArr(a,len);
    TimeSpec::begin();
    MyShakerSort(a,len);
    TimeSpec::print_cost_time();
    // printlnArr(a,len);
    println("=========================================");
    constexpr int len2 = 10;
    //���鳤�Ȳ���Ϊ0,���򱨴�
    int a2[len2] = {0};
    createRandomArrayWithBase(a2,len2,len2);
    printlnArr(a2,len2);
    TimeSpec::begin();
    MyShakerSort(a2,len2);
    TimeSpec::print_cost_time();
    printlnArr(a2,len2);
    println("=========================================");
    constexpr int len3 = 10000;
    //���鳤�Ȳ���Ϊ0,���򱨴�
    int a3[len3] = {0};
    createRandomArrayWithBase(a3,len3,len3);
    // printlnArr(a3,len3);
    TimeSpec::begin();
    ShakerSort(a3,len3);
    TimeSpec::print_cost_time();
    // printlnArr(a3,len3);
    println("=========================================");
    constexpr int len4 = 10;
    //���鳤�Ȳ���Ϊ0,���򱨴�
    int a4[len4] = {0};
    createRandomArrayWithBase(a4,len4,len4);
    printlnArr(a4,len4);
    TimeSpec::begin();
    ShakerSort(a4,len4);
    TimeSpec::print_cost_time();
    printlnArr(a4,len4);
    return 0;
}