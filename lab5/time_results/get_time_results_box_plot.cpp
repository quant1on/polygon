#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>

#include "../all_sorts.cpp"

int main(){
    int n, temp, start, total_time, k;
    std :: cout << "Enter number of elements in set: ";
    std :: cin >> n;
    std :: cout << "Enter number of launches: ";
    std :: cin >> k;

    std :: vector<int> test_arr, whole_test_arr;
    std :: fstream file, data_file;
    file.open("../tests/big_test.txt");
    data_file.open("time_res_cycle_box_plot4.txt");
    while (file >> temp) whole_test_arr.push_back(temp);
    for (int i = 0; i < k; i++){
        for (int j = i * (whole_test_arr.size() - n)/k; j < i * (whole_test_arr.size() - n)/k + n; j++) test_arr.push_back(whole_test_arr[j]);
        start = clock();
        CycleSort(test_arr);
        total_time = clock() - start;
        data_file << total_time << std :: endl;
        test_arr.clear();
    }

    std :: cout << "Done!" << std :: endl;
}