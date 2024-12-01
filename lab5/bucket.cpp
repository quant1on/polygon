#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <cmath>

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

bool check_sorted(std :: vector<int> &arr){
    bool ans = true;
    for (int i = 0; i < arr.size() - 1; i++){
        if (arr[i] > arr[i + 1]) ans = false;
    }

    return ans;
}

TEST_CASE("testing BucketSort() function"){
    std :: vector<int> test1 = {0, 0, 0, 0, 0}, test2 = {1, 2, 3, 4, 5, 6}, test3 = {-123, 12, 21, -3, -231, 321, 1}, test4;
    BucketSort(test1);
    BucketSort(test2);
    BucketSort(test3);
    
    std :: fstream file_test4;
    int temp;
    file_test4.open("./tests/worst_case_bucket.txt");
    while(file_test4 >> temp) test4.push_back(temp);
    BucketSort(test4);
    
    CHECK(check_sorted(test1));
    CHECK(check_sorted(test2));
    CHECK(check_sorted(test3));
    CHECK(check_sorted(test4));
}