#include "the2.h"

// DO NOT CHANGE ABOVE THIS LINE!
// you may implement helper functions below

int getGroupIndex(const std::string& str, int group_start, int group_size) {
    int base = 27;
    int index = 0;

    for (int i = 0; i < group_size; ++i) {
        char ch = (group_start + i) < str.size() ? str[group_start + i] : ' ';
        int char_index = (ch == ' ') ? 0 : (ch - 'A' + 1);
        index = index * base + char_index;
    }
    return index;
}

int CountingSort(std::string* arr, int size, int group_start, int group_size, bool ascending) {
    int iterations = 0;

    int base = 27; 
    int k = pow(base, group_size);
    int* C = new int[k]();
    std::string* output = new std::string[size];

    for (int i = 0; i < size; ++i) {
        int index = getGroupIndex(arr[i], group_start, group_size);
        C[index]++;
        iterations++;
    }

    if (ascending) {
        for (int i = 1; i < k; ++i) {
            C[i] += C[i - 1];
            iterations++;
        }
    } else {
        for (int i = k - 2; i >= 0; --i) {
            C[i] += C[i + 1];
            iterations++;
        }
    }

    if (ascending) {
        for (int i = size - 1; i >= 0; --i) {
            int index = getGroupIndex(arr[i], group_start, group_size);
            output[--C[index]] = arr[i];
            iterations++;
        }
    } else {
        for (int i = size - 1; i >= 0; --i) {
            int index = getGroupIndex(arr[i], group_start, group_size);
            output[--C[index]] = arr[i];
            iterations++;
        }
    }

    for (int i = 0; i < size; ++i) {
        arr[i] = output[i];
        iterations++;
    }

    delete[] C;
    delete[] output;
    return iterations;
}

long multi_digit_string_radix_sort(std::string *arr,
                                   const int size,
                                   const bool ascending,
                                   const int group_size){
    int total_iterations = 0;
    int group_start;

    int max_len = 0;
    for (int i = 0; i < size; ++i) {
        max_len = std::max(max_len, (int)arr[i].size());
    }

    for (group_start = max_len - group_size; group_start >= 0; group_start -= group_size) {
        total_iterations += CountingSort(arr, size, group_start, group_size, ascending);
    }
    
    if(group_start != (-1*group_size)){
        total_iterations += CountingSort(arr, size, 0, (max_len%group_size), ascending);
    }
    
    return total_iterations;
}
