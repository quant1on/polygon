#include <iostream>
#include <fstream>
#include <vector>

//тестирующий фреймворк
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../source/doctest.h"

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

bool check_sorted(std :: vector<int> &arr){
    bool ans = true;
    for (int i = 0; i < arr.size() - 1; i++){
        if (arr[i] > arr[i + 1]) ans = false;
    }

    return ans;
}

TEST_CASE("testing HeapSort() function"){
    std :: vector<int> test1 = {0, 0, 0, 0, 0}, test2 = {1, 2, 3, 4, 5, 6, 7, 8}, test3 = {32, 1, 343, 23, 12, 43, 65, 42, 43, 54}, test4;
    HeapSort(test1);
    HeapSort(test2);
    HeapSort(test3);

    std :: fstream file_test4;
    int temp;
    file_test4.open("./tests/worst_case_heap.txt");
    while(file_test4 >> temp) test4.push_back(temp);
    HeapSort(test4);


    CHECK(check_sorted(test1));
    CHECK(check_sorted(test2));
    CHECK(check_sorted(test3));
    CHECK(check_sorted(test4));
}