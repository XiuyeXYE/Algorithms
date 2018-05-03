#include"MyOutput.hpp"


template<typename T>
int idxMax(T a[],int i,int j){
    return a[i] > a[j]?i:j;
}

template<typename T>
void MyAdjustHeap(T a[],int len){
    int lastNonLeafNodeIdx = len/2 - 1;
    bool adjustNext = true;
    while(adjustNext){
        //�ȵ�����,
        for(int i=lastNonLeafNodeIdx;i>=0;i--){
            int leftChildIdx = 2*i+1;
            int rightChildIdx = 2*i+2;
            if(leftChildIdx < len){
                if(rightChildIdx < len){
                    int childMaxIdx = idxMax(a,leftChildIdx,rightChildIdx);
                    int maxIdx = idxMax(a,i,childMaxIdx);
                    if(i != maxIdx){
                        swap(a+i,a+maxIdx);
                    }
                }
                else{
                    if(a[i] < a[leftChildIdx]){
                        swap(a+i,a+leftChildIdx);
                    }
                }
            }
        }
        //�������Ƿ��в�������Ӷ�.
        adjustNext = false;
        for(int i=lastNonLeafNodeIdx;i>=0;i--)
        {
            int leftChildIdx = 2*i+1;
            int rightChildIdx = 2*i+2;
            if(leftChildIdx < len){
                if(rightChildIdx < len){
                    if(a[i] < a[leftChildIdx] || a[i] < a[rightChildIdx]){
                        //���������ͼ�������
                        adjustNext = true;
                        break;
                    }
                }
                else{
                    if(a[i] < a[leftChildIdx]){
                        //���������ͼ�������
                        adjustNext = true;
                        break;
                    }
                }
            }
        }
    }
}

/**
 * ����һ�ζ�,������ζ��,����һ��
 * �Ѿ͵�����Ϊ��(С)����,һ�����ǲ����ܵ�.
 * �п���һ�ξ͵�������.
 * 
 * ���������Ǹ��㷨�ĺ���
 * ���ǹ���,��ס˼����߹���
 * 
 * ��������󶥶�
 * 
 */ 
template<typename T>
void adjustHeap(T a[],int i,int len){
    T temp = a[i];
    //��������
    for(int k=i*2+1;k<len;k=k*2+1){
        if(k+1 < len && a[k] < a[k+1]){//���ҽڵ�Ƚ�
            k++;
        }
        if(a[k] > temp){//�ӽڵ�͸��ڵ�Ƚ�
            a[i] = a[k];
            i=k;
        }
        else{
            break;
        }
    }
    a[i] = temp;
}


/**
 * ������:
 * �� ==> һ������������ (�߼��ṹ)
 * �洢�ṹ ==> �������� �� ���� (����ṹ).
 * ����:
 * 1.a[i] Ϊ���ڵ� �� �ӽڵ�Ϊ �� a[2*i+1] �� a[2*i+2]
 * 2.��һ��ĩβ��Ҷ�ӽڵ� (�Ȳ�Ϊ0) ������(�����±����) len(a)/2[����] - 1;
 * 
 * 
 * ������(С)����:
 * �ҵ�a[2*i+1] a[2*i+2] a[i]�е����ֵ(��Сֵ),����ֵ,��a[i]�������ֵ(��Сֵ)
 * ����������������������,�ͳ��˴󶥶�(С����),�ⲻ�Ƕ���������,ֻ�ܶѶ��ڵ�ֵ����(С��)
 * �����ӽڵ������.
 * 
 * �������:
 * ��(С)�����Ѿ�������,����β�ڵ�Ͷ���(root)����,Ȼ���ų�β�ڵ�,��������β�ڵ�����Ķ�,
 * ʹ֮��Ϊ��(С)����,���ڽ�������ѵ�β�ڵ�Ͷ���(root),Ȼ���ٴε���,��˷���ֻʣ��һ���ڵ��,
 * �������������Ѿ��ź���.
 * 
 * @Reference:
 * @URL = https://www.cnblogs.com/chengxiao/p/6129630.html
 * 
 */ 
template<typename T>
void HeapSort(T a[],int len){
    //�����������������Ķѽ��е���,ʹ֮��Ϊ��
    //���µ���,������.
    //����ѭ�������˴󶥶�
    for(int i=len/2-1;i>=0;i--){
        adjustHeap(a,i,len);
    }
    //�����Ѷ�Ԫ����ĩβԪ��+�����ѽṹ
    for(int i=len-1;i>0;i--){
        swap(a,a+i);//���Ѷ�Ԫ����ĩβԪ�ؽ��н���
        //�������ݺ�,����һ�ξ�����,��Ҫ�Ӹ��ڵ㿪ʼ����һ��
        //Ϊʲô�������һ�ξ�������?
        //���˸��ڵ㲻����,����Ľڵ㶼����,ֻ��Ҫ���ϵ���(�����ڵ������ڵ�ĵ���)����һ��
        //�Ϳ�����.��ͼ�Ƶ��� ���˸��ڵ��Ѿ��γɵĶѵ� ���ڹ���.
        adjustHeap(a,0,i);
    }

}


int main(){

    constexpr int len = 10;
    int a[len] = {0};
    createRandomArrayWithBase(a,len,len);
    printlnArr(a,len);
    TimeSpec::begin();
    MyAdjustHeap(a,len);
    TimeSpec::print_cost_time();
    printlnArr(a,len);
    println("===================================");
    constexpr int len2 = 10;
    // int a2[len2] = {9,9,7,5,7,0,0,3,5,6};
    int a2[len2] = {9,7,9,5,6,7,0,3,5,0};
    createRandomArrayWithBase(a2,len2,len2);
    printlnArr(a2,len2);
    TimeSpec::begin();
    // HeapSort(a2,len2);
    for(int i = len2/2-1;i>=0;i--){
        adjustHeap(a2,i,len2);
    }
    TimeSpec::print_cost_time();
    printlnArr(a2,len2);
    println("================������===================");
    constexpr int len3 = 10000;
    // int a2[len2] = {9,9,7,5,7,0,0,3,5,6};
    int a3[len3] = {0};
    createRandomArrayWithBase(a3,len3,len3);
    // printlnArr(a3,len3);
    TimeSpec::begin();
    HeapSort(a3,len3);
    TimeSpec::print_cost_time();
    // printlnArr(a3,len3);
    println("========================");
    constexpr int len4 = 10;
    // int a2[len2] = {9,9,7,5,7,0,0,3,5,6};
    int a4[len4] = {0};
    createRandomArrayWithBase(a4,len4,len4);
    printlnArr(a4,len4);
    TimeSpec::begin();
    HeapSort(a4,len4);
    TimeSpec::print_cost_time();
    printlnArr(a4,len4);


    return 0;
}
