#include <iostream>
#include <string>


int Partition(int List[], int low, int high){
    int pivot = List[low];
    while (low < high){
        while(low<high && List[high]>=pivot) --high;
        // std::cout << std::endl << "MyList[" << low << "]=" << List[high];
        List[low] = List[high]; //所以这个List[high]是小于List[low]的

        while(low<high && List[low]<=pivot) ++low;
        // std::cout << std::endl << "MyList[" << high << "]=" << List[low];
        List[high] = List[low];
    }
    List[low] = pivot;
    return low;
}

void QuickSort(int List[], int low, int high){
    if (low < high){
        int pivotpos = Partition(List, low, high);
        QuickSort(List, low, pivotpos-1);   //注意递归时不需要递归中间的这个数了
        QuickSort(List, pivotpos+1, high);  //注意递归时不需要递归中间的这个数了
    }
}

int main(int argc, char const *argv[])
{
    int Length = 20;
    int MyList[20] = {46,3,5,72,9,35,6,2,3,1,4,32,78,12,92,64,81,19,20,55};
    
    //print 数组
    std::cout << std::endl;
    for (int i = 0; i < Length; ++i){
        std::cout << MyList[i] << ",";
    }

    //快速排序函数
    QuickSort(MyList, 0, Length-1);

    ////print 数组
    std::cout << std::endl;
    for (int i = 0; i < Length; ++i){
        std::cout<< MyList[i] << ",";
    }
    return 0;
}