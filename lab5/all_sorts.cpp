#include <iostream>
#include <vector>
#include <limits>
#include <cmath>

void CocktailShakerSort(std :: vector<int> &arr){
    bool swapped = true;
    int n = arr.size();
    for (int i = 0; swapped; i++){
        swapped = false;

        for (int j = i; j < n - 1 - i; j++){
            if (arr[j] > arr[j + 1]){
                std :: swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        if (!swapped) break;

        swapped = false;

        for (int j = n - i - 2; j > i; j--){
            if (arr[j-1] > arr[j]){
                std :: swap(arr[j], arr[j-1]);
                swapped = true;
            }
        }
    }
}

void CycleSort(std :: vector<int> &arr){
    if (arr.size() == 0) return;
    int n = arr.size(), pos, element;
    for (int cycle_start = 0; cycle_start < n - 1; cycle_start++){
        pos = cycle_start;
        element = arr[cycle_start];

        for (int i = cycle_start + 1; i < n; i++){
            if (arr[i] < element) pos++;
        }

        if (pos == cycle_start) continue;

        while (element == arr[pos]) pos++;

        if (cycle_start != pos) std :: swap(element, arr[pos]);

        while (pos != cycle_start){
            pos = cycle_start;

            for (int i = cycle_start + 1; i < n; i++){
                if (arr[i] < element) pos++;
            }

            while (element == arr[pos]) pos++;

            if (arr[pos] != element) std :: swap(element, arr[pos]);
        }
    }
}

void heapify(std :: vector<int> &arr, int curr_root, int not_cmplt){
    int greatest_prev = -2, greatest = -1, l, r;

    while (greatest_prev != greatest){
        greatest_prev = greatest;
        greatest = curr_root;
        l = 2 * curr_root + 1;
        r = 2 * curr_root + 2;

        if (l < not_cmplt && arr[greatest] < arr[l]) greatest = l;
        if (r < not_cmplt && arr[greatest] < arr[r]) greatest = r;

        if (greatest != curr_root){
            std :: swap(arr[curr_root], arr[greatest]);
            curr_root = greatest;
        }
    }
}

void HeapSort(std :: vector<int> &arr){
    int n = arr.size();

    for (int i = n/2 - 1; i >= 0; i--) heapify(arr, i, n);

    for (int i = n - 1; i > 0; i--){
        std :: swap(arr[i], arr[0]);

        heapify(arr, 0, i);
    }
}

int arr_max(std :: vector<int> &arr){
    int ans = std :: numeric_limits<int> :: min();
    for (int i = 0; i < arr.size(); i++) ans = std :: max(ans, arr[i]);
    return ans;
}

int arr_min_exception(std :: vector<int> &arr){
    int ans = 0;
    for (int i = 0; i < arr.size(); i++) ans = std :: min(ans, arr[i]);
    return ans;
}

void BucketSort(std :: vector<int> &arr){
    int n = arr.size(), M = arr_max(arr), m_mod = std :: abs(arr_min_exception(arr));
    std :: vector<std :: vector<int>> buckets(n + 1);
    int numerator, denominator = std :: max(1, M + m_mod);
    double factor;

    for (int i = 0; i < n; i++){
        numerator = arr[i] + m_mod;
        factor = numerator/denominator;
        buckets[floor(n * factor)].push_back(arr[i]);
    }

    for (int i = 0; i < n + 1; i++) HeapSort(buckets[i]);

    int index = 0;
    for (int i = 0; i < n + 1; i++){
        for (int j = 0; j < buckets[i].size(); j++) arr[index++] = buckets[i][j];
    }
}
