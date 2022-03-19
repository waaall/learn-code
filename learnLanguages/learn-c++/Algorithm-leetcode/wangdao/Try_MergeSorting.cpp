#include <iostream>
#include <string>


#define Length 20
int * B = new int[Length];

void Merge(int A[], int low, int mid, int high){
    int i, j, k;
    for (int k=low; k<=high; ++i) 
        B[k] = A[k];
    for (i=low,j=mid,k=i; i<=mid && j<=high; ++k){
        if (B[i] <= B[j]) A[k] = B[i++];
        else A[k] = B[j++];
    }
    while()
}

void MergeSort(int A[], int low, int high){
    if (low < high){
        int mid = (low + high) / 2;
        if (low < mid)
            MergeSort(A, low, mid);
        if (mid+1 < high)
            MergeSort(A, mid+1, high);
        Merge(A, low, mid, high);
    }
}

int main(int argc, char const *argv[])
{
    int MyList[Length] = {46,3,5,72,9,35,6,2,3,1,4,32,78,12,92,64,81,19,20,55};
    
    //print 数组
    std::cout << std::endl;
    for (int i = 0; i < Length; ++i){
        std::cout << MyList[i] << ",";
    }

    //快速排序函数
    MergeSort(MyList, 0, int(Length/2), Length-1);

    ////print 数组
    std::cout << std::endl;
    for (int i = 0; i < Length; ++i){
        std::cout<< MyList[i] << ",";
    }

    delete B;

    return 0;
}