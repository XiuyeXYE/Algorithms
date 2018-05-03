#include"MyOutput.hpp"


template<typename T>
int partition(T a[],int p,int r){
    
    T x = a[p];//����Ԫ������Ϊ�ڱ�
    int i=p+1;//����Ԫ�ص��¸�Ԫ�ؿ�ʼ�Ƚ�
    int j=r;
    

    //������++i,++j��(��)�ٱ�,����Ҫ
    while(true){
        //��Ҫa[i++]��a[j--]ִ�к�,i��+1,j��-1,���swap ���� ����.
        while( x > a[i] && i < j){
            i++;
        }
        while( x < a[j]){
            j--;
        }
        if(i>=j)break;
        swap(a+i,a+j);
        /**
         * �����ظ�������:�� 9 9 9 8 10
         * ����� i++ �� j--
         * ���ظ� 9 �Ƚ� 9 
         * ��Ϊ ����ѭ���� ���ȱȽ� > < ���� <= >=,�ٴ�+1 -1,
         * 
         * �᲻�� Խ�� �� ? �� i++ �� i > n,j--�� j<0 
         * 5 4 3 2 1 ; x = 5, i = 1 , arr[i++] < x;Լ�� i < j ,���� ���� i = j = 4; arr[j=4] < x;
         * i>=j (4>=4) break;arr[j=4] = x = 5;arr[0] = 1;return j=4;
         * 
         * 6 7 8 9 10; x = 6, i = 1 , arr[i] > 6, j = 4,a[j--] > x => j = 0 (6 > 6)
         * i>=j (1>=0) break; a[p=0] = a[j=0]; a[j=0] = x (=a[p]),return j=0;
         * 
         * 
         * 
         */ 
        i++;
        j--;
        // println("i=",i," ; j=",j);
    }
    
    a[p] = a[j];
    a[j] = x;
    // println(j);
    return j;
}

//ASC
template<typename T>
void QuickSort(T a[],int p,int r){
    if(p<r){

        int q = partition(a,p,r);
        // printlnArr(a,10);
        // println(q);
        QuickSort(a,p,q-1);
        QuickSort(a,q+1,r);
    }
}


int main(){

    constexpr int len = 10000;
    int a[len] = {0};//{1,2,3,4,5,6,7,8,9,10};
    createRandomArrayWithBase(a,len,len);
    // printlnArr(a,len);
    TimeSpec::begin();

    QuickSort(a,0,len-1);

    TimeSpec::print_cost_time();
    // printlnArr(a,len);
    return 0;
}

