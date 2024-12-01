#include <iostream>
#include <vector>
#include <fstream>

//тестирующий фреймворк
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../source/doctest.h"

//циклическая сортировка сортировка, сложность: O(n^2) в лучшем случае (если массив изначально отсортирован),
//в среднем и худшем случах сложность O(n^2)

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

bool check_sorted(std :: vector<int> &arr){
    bool ans = true;
    for (int i = 0; i < arr.size() - 1; i++){
        if (arr[i] > arr[i + 1]) ans = false;
    }

    return ans;
}

TEST_CASE("testing CycleSort() function"){
    std :: vector<int> test1 = {0, 0, 0, 0, 0, 0}, test2 = {1, 2, 3, 4, 5, 6}, test3 = {3, 2, 3, 2, 1, 2}, test4;
    CycleSort(test1);
    CycleSort(test2);
    CycleSort(test3);

    std :: fstream file_test4;
    int temp;
    file_test4.open("./tests/worst_case_cycle.txt");
    while(file_test4 >> temp) test4.push_back(temp);
    CycleSort(test4);

    CHECK(check_sorted(test1));
    CHECK(check_sorted(test2));
    CHECK(check_sorted(test3));
    CHECK(check_sorted(test4));
}