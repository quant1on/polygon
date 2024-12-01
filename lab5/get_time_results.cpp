#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>

#include "all_sorts.cpp"

int main(){
    int n, temp, start, total_time, k;
    std :: cout << "Enter humber of elements (in K): ";
    std :: cin >> n;
    std :: cout << "Enter the gap(in K): ";
    std :: cin >> k;
    n = std :: min(std :: max(n, 1), 1000);

    std :: vector<int> test_arr, whole_test_arr;
    std :: fstream file, data_file;
    file.open("./tests/big_test.txt");
    data_file.open("time_res_cycle.txt");
    while (file >> temp) whole_test_arr.push_back(temp);
    for (int i = 1; i <= n; i++){
        for (int j = 0; j < i * k * 1000; j ++) test_arr.push_back(whole_test_arr[j]);
        start = clock();
        CycleSort(test_arr);
        total_time = clock() - start;
        data_file << i * k * 1000 << " " << double(total_time) << std :: endl;
        test_arr.clear();
    }

    std :: cout << "Done!" << std :: endl;
}