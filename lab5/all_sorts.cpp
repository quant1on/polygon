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