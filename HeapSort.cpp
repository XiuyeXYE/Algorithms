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
        //先调整堆,
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
        //检查堆中是否有不满足的子堆.
        adjustNext = false;
        for(int i=lastNonLeafNodeIdx;i>=0;i--)
        {
            int leftChildIdx = 2*i+1;
            int rightChildIdx = 2*i+2;
            if(leftChildIdx < len){
                if(rightChildIdx < len){
                    if(a[i] < a[leftChildIdx] || a[i] < a[rightChildIdx]){
                        //如果不满足就继续调整
                        adjustNext = true;
                        break;
                    }
                }
                else{
                    if(a[i] < a[leftChildIdx]){
                        //如果不满足就继续调整
                        adjustNext = true;
                        break;
                    }
                }
            }
        }
    }
}

/**
 * 调整一次堆,但不意味着,仅仅一次
 * 堆就调整成为大(小)顶堆,一般那是不可能的.
 * 有可能一次就调整好了.
 * 
 * 调整函数是该算法的核心
 * 这是规律,记住思想或者过程
 * 
 * 这个函数大顶堆
 * 
 */ 
template<typename T>
void adjustHeap(T a[],int i,int len){
    T temp = a[i];
    //从上往下
    for(int k=i*2+1;k<len;k=k*2+1){
        if(k+1 < len && a[k] < a[k+1]){//左右节点比较
            k++;
        }
        if(a[k] > temp){//子节点和根节点比较
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
 * 堆排序:
 * 堆 ==> 一颗完美二叉树 (逻辑结构)
 * 存储结构 ==> 线性序列 即 数组 (物理结构).
 * 性质:
 * 1.a[i] 为父节点 则 子节点为 左 a[2*i+1] 右 a[2*i+2]
 * 2.第一个末尾非叶子节点 (度不为0) 的索引(或者下标序号) len(a)/2[整除] - 1;
 * 
 * 
 * 调整大(小)顶堆:
 * 找到a[2*i+1] a[2*i+2] a[i]中的最大值(最小值),交换值,让a[i]保持最大值(最小值)
 * 这样反复调整整个二叉树,就成了大顶堆(小顶堆),这不是二叉排序树,只管堆顶节点值大于(小于)
 * 左右子节点就行了.
 * 
 * 排序过程:
 * 大(小)顶堆已经就序了,就是尾节点和顶点(root)交换,然后排除尾节点,调整除了尾节点以外的堆,
 * 使之成为大(小)顶堆,现在交换这个堆的尾节点和顶点(root),然后再次调整,如此反复只剩下一个节点后,
 * 整个数组的序就已经排好了.
 * 
 * @Reference:
 * @URL = https://www.cnblogs.com/chengxiao/p/6129630.html
 * 
 */ 
template<typename T>
void HeapSort(T a[],int len){
    //对整个完美二叉树的堆进行调整,使之成为堆
    //从下到上,从左到右.
    //整个循环构成了大顶堆
    for(int i=len/2-1;i>=0;i--){
        adjustHeap(a,i,len);
    }
    //交换堆顶元素与末尾元素+调整堆结构
    for(int i=len-1;i>0;i--){
        swap(a,a+i);//将堆顶元素与末尾元素进行交换
        //交换数据后,调整一次就行了,但要从根节点开始调整一次
        //为什么这里调整一次就行了呢?
        //除了根节点不满足,其余的节点都满足,只需要从上到下(三个节点三个节点的调整)调整一次
        //就可以了.画图推导下 除了根节点已经形成的堆的 调节过程.
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
    println("================堆排序===================");
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
