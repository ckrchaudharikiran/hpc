#include <iostream>
#include <vector>
#include <omp.h>

// Parallel Bubble Sort
void parallelBubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    bool swapped;
    
    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        
        #pragma omp parallel for shared(arr, swapped)
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                #pragma omp critical
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        
        if (!swapped)
            break;
    }
}

// Merge function for merge sort
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    std::vector<int> leftArr(n1);
    std::vector<int> rightArr(n2);
    
    for (int i = 0; i < n1; ++i)
        leftArr[i] = arr[left + i];
    
    for (int j = 0; j < n2; ++j)
        rightArr[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            ++i;
        } else {
            arr[k] = rightArr[j];
            ++j;
        }
        ++k;
    }
    
    while (i < n1) {
        arr[k] = leftArr[i];
        ++i;
        ++k;
    }
    
    while (j < n2) {
        arr[k] = rightArr[j];
        ++j;
        ++k;
    }
}

// Parallel Merge Sort
void parallelMergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, left, mid);
            #pragma omp section
            parallelMergeSort(arr, mid + 1, right);
        }
        
        merge(arr, left, mid, right);
    }
}

int main() {
    int n;
    std::cout << "Enter the number of elements: ";
    std::cin >> n;
    
    std::vector<int> arr(n);
    std::cout << "Enter the elements: ";
    for (int i = 0; i < n; ++i)
        std::cin >> arr[i];
    
    // Parallel Bubble Sort
    std::cout << "Parallel Bubble Sort:" << std::endl;
    parallelBubbleSort(arr);
    for (int num : arr)
        std::cout << num << " ";
    std::cout << std::endl;
    
    // Parallel Merge Sort
    std::vector<int> mergeArr(arr);
    std::cout << "Parallel Merge Sort:" << std::endl;
    parallelMergeSort(mergeArr, 0, n - 1);
    for (int num : mergeArr)
        std::cout << num << " ";
    std::cout << std::endl;
    
    return 0;
}
