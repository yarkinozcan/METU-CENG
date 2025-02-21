#include "the1.h"

// DO NOT CHANGE ABOVE THIS LINE!
// you may implement helper functions below

std::pair<int, int> partition(unsigned short *arr, const unsigned int size, long &swap) {
    int i = 1;          
    int j = 1;          
    int k = size - 1;  
    unsigned short pivot = arr[0];  

    while (j <= k) {
        if (arr[j] < pivot) {
            std::swap(arr[i], arr[j]);
            swap++;
            i++;
            j++;
        } else if (arr[j] > pivot) {
            std::swap(arr[j], arr[k]);
            swap++;
            k--;
        } else {
            j++;
        }
    }
    std::swap(arr[i-1],arr[0]);
    swap++;
    return std::make_pair(i - 1, j);
}

std::pair<unsigned short, unsigned int> select_k_with_quick_sort3(unsigned short *arr,
                                                                  const unsigned int size,
                                                                  const unsigned int index,
                                                                  long &swap) {
    unsigned int recursive_calls = 0;
    auto quicksort = [&](int low, int high, auto &&quicksort_ref) -> unsigned short {
        recursive_calls++;
        if (low < high) {
            auto [leftPivot, rightPivot] = partition(arr + low, high - low + 1, swap);
            leftPivot += low;  
            rightPivot += low;
            quicksort_ref(low, leftPivot - 1, quicksort_ref);  
            quicksort_ref(rightPivot, high, quicksort_ref);     
        }
        return arr[index];
    };
    quicksort(0, size - 1, quicksort);
    return std::make_pair(arr[index], recursive_calls);
}

std::pair<unsigned short, unsigned int> quick_select3(unsigned short *arr,
                                                      const unsigned int size,
                                                      const unsigned int index,
                                                      long &swap) {
    unsigned int recursive_calls = 0;
    int low = 0, high = size - 1;

    while (low <= high) {
        auto [leftPivot, rightPivot] = partition(arr + low, high - low + 1, swap);
        leftPivot += low;
        rightPivot += low;
        recursive_calls++;

        if (index < leftPivot) {
            high = leftPivot - 1;
        } else if (index >= rightPivot) {
            low = rightPivot;
        } else {
            return std::make_pair(arr[index], recursive_calls);
        }
    }

    return std::make_pair(arr[index], recursive_calls);
}