#include <iostream>
#include <vector>

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