#include <iostream>
#include <vector>
#include <fstream>

//тестирующий фреймворк
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../source/doctest.h"

//циклическая сортировка, сложность: O(n^2), расход: 12 (далее в коде единицы будут указываться в байтах)

//основной концепт сортировки - расставить все элементы по нужным местам за минимальное кол-во перестановок, используя
//циклы: каждый раз, когда элемент встает на свое место, тот элемент, который стоял до этого так же перемещается на нужную
//позицию. Действия повторяются до того момента, как происходит перестановка с элементом, стоящим в начале цикла (что означает, что
//цикл замкнулся/закончился); далее берется следующий цикл (следующий начальный индекс цикла) и т. д.

void CycleSort(std :: vector<int> &arr){
    if (arr.size() == 0) return;
    int n = arr.size(), pos, element; // 4 * 3 = 12; pos - позиция, на которую текущий элемент должен встать,
                                      //element - элемент, для которого ищем нужную позицию
    for (int cycle_start = 0; cycle_start < n - 1; cycle_start++){ // O(n), общая асимптотика (из-за вложенных циклов, см. ниже) ~O(n^2)
        pos = cycle_start;          //начало цикла
        element = arr[cycle_start]; //берем начальный элемент цикла как первый на перестановку

        for (int i = cycle_start + 1; i < n; i++){ //O(n), считаем, на сколько надо сдвинуть pos, чтобы element был на своем месте
            if (arr[i] < element) pos++;
        }

        if (pos == cycle_start) continue;   //если оказались там же, где начинали, значит, элемент уже на правильном месте

        while (element == arr[pos]) pos++;  //если попали туда, где находится тот же элемент (копия), сдвигаем текущий элемент в конец

        if (cycle_start != pos) std :: swap(element, arr[pos]); //вставляем начальный элемент, берем следующий и ищем место для него

        while (pos != cycle_start){
            pos = cycle_start;  //начинаем искать с начала цикла

            for (int i = cycle_start + 1; i < n; i++){ //O(n), считаем pos для элемента
                if (arr[i] < element) pos++;
            }

            while (element == arr[pos]) pos++;          //если там уже есть копии, ставим в конец

            if (arr[pos] != element) std :: swap(element, arr[pos]); //для минимизации перестановок сначала проверяем, нужно ли это вообще
                                                                     //(если элемент равны, но перестановка по сути ничего не сделает)
        }
    }
}

bool check_sorted(std :: vector<int> &arr){ //функция проверки массива на отсортированность
    bool ans = true;
    for (int i = 0; i < arr.size() - 1; i++){
        if (arr[i] > arr[i + 1]) ans = false;
    }

    return ans;
}

TEST_CASE("testing CycleSort() function"){
    std :: vector<int> test1 = {0, 0, 0, 0, 0, 0}, test2 = {1, 2, 3, 4, 5, 6}, test3 = {3, 2, 3, 2, 1, 2}, test4;
    CycleSort(test1); //массив с равными элементами
    CycleSort(test2); //уже отсортированный массив
    CycleSort(test3); //общий кейс

    std :: fstream file_test4;
    int temp;
    file_test4.open("./tests/worst_case_cycle.txt");
    while(file_test4 >> temp) test4.push_back(temp);
    CycleSort(test4); //худший кейс для данной сортировки: отсортированный по невозрастанию массив

    CHECK(check_sorted(test1));
    CHECK(check_sorted(test2));
    CHECK(check_sorted(test3));
    CHECK(check_sorted(test4));
}