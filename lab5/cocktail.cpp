#include <iostream>
#include <vector>
#include <fstream>

//тестирующий фреймворк
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../source/doctest.h"

//шейкерная сортировка, сложность: O(n) в лучшем случае (если массив изначально отсортирован),
//в среднем и худшем случах сложность ~O(n^2)

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

bool check_sorted(std :: vector<int> &arr){
    bool ans = true;
    for (int i = 0; i < arr.size() - 1; i++){
        if (arr[i] > arr[i + 1]) ans = false;
    }

    return ans;
}

TEST_CASE("testing CocktailShakerSort() function"){
    std :: vector<int> test1 = {0, 0, 0, 0, 0, 0}, test2 = {1, 2, 3, 4, 5, 6}, test3 = {21, 32, 1, 32, 4, 6, 34, 15}, test4;
    CocktailShakerSort(test1);
    CocktailShakerSort(test2);
    CocktailShakerSort(test3);

    std :: fstream file_test4;
    int temp;
    file_test4.open("./tests/worst_case_cocktail.txt");
    while(file_test4 >> temp) test4.push_back(temp);
    CocktailShakerSort(test4);

    CHECK(check_sorted(test1));
    CHECK(check_sorted(test2));
    CHECK(check_sorted(test3));
    CHECK(check_sorted(test4));
    
}