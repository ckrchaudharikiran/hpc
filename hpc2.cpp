#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped = false;
    do {
        swapped = false;
        #pragma omp parallel for shared(arr, swapped)
        for (int i = 0; i < n - 1; ++i) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
    } while (swapped);
}

void parallelMergeSort(vector<int>& arr) {
    if (arr.size() <= 1) return;
    int mid = arr.size() / 2;
    vector<int> left(arr.begin(), arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.end());

    #pragma omp parallel sections
    {
        #pragma omp section
        parallelMergeSort(left);
        #pragma omp section
        parallelMergeSort(right);
    }

    merge(left.begin(), left.end(), right.begin(), right.end(), arr.begin());
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter the elements:\n";
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    vector<int> arr_copy = arr;

    auto start = high_resolution_clock::now();
    parallelBubbleSort(arr);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Parallel Bubble Sort took " << duration.count() << " milliseconds." << endl;

    cout << "Is Sorted: " << boolalpha << is_sorted(arr.begin(), arr.end()) << endl;

    arr = arr_copy;

    start = high_resolution_clock::now();
    parallelMergeSort(arr);
    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    cout << "Parallel Merge Sort took " << duration.count() << " milliseconds." << endl;

    cout << "Is Sorted: " << boolalpha << is_sorted(arr.begin(), arr.end()) << endl;

    return 0;
}
