#include <iostream>
#include <fstream>
#include <vector>

//тестирующий фреймворк
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../source/doctest.h"

//функция для построения бинарного дерева с условием, что дочерние узлы меньше родительских, сложность log(n);
//конкретный узел (который берется за текущий корень для рассмотрения поддерева) "топит" (спускает вниз по дереву)
//до того момента, пока условие не выполнится

//сложность O(log(n)), расход: 16 байт
void heapify(std :: vector<int> &arr, int curr_root, int not_cmplt){
    int greatest_prev = -1, greatest = curr_root, l, r; // 4 * 4 = 16, считаем, что текущий корень самый большой в поддереве,
                                                        //если нет, то меняем местами новый наибольший в поддереве и текущий корень,
                                                        //после чего обновляем поддерево, куда попал предыдущий корень

    //смысл в том, что если после итерации сравнения индекс наибольшего элемента в поддереве не поменялся,
    //то текущий корень действительно наибольший и можно закончить итерацию
    while (greatest_prev != greatest){ //в наихудшем случае (когда необходимо поставить изначальный корень
        greatest_prev = greatest;      //в самый низ дерева) O(log(n))
        greatest = curr_root;
        l = 2 * curr_root + 1;         // получаем индексы левого и правого детей
        r = 2 * curr_root + 2;

        if (l < not_cmplt && arr[greatest] < arr[l]) greatest = l; //получаем наибольший элемент в рассматриваемом узле
        if (r < not_cmplt && arr[greatest] < arr[r]) greatest = r;

        if (greatest != curr_root){ //меняем, если индекс наибольшего поменялся
            std :: swap(arr[curr_root], arr[greatest]);
            curr_root = greatest; //начинаем обновление поддерева
        }
    }
}

//собственно, сама сортировка. Основная идея заключается в том, что мы используем бинарное дерево (полное) с условием, что родительские
//узлы больше, чем дочерние. Таким образом, поддерживая это условие, мы можем получить наибольшее число в массиве, взяв элемент в корне.
//Потом, ставя этот элемент в конец и исключая его из дерева, получаем рабочий алгоритм, который путем поиска максимума в неотсортированной части
//сортирует массив.

//сложность ~O(n * log(n)), расход по памяти: 20 байт
void HeapSort(std :: vector<int> &arr){
    int n = arr.size(); //+4

    for (int i = n/2 - 1; i >= 0; i--) heapify(arr, i, n); //строим max heap снизу вверх, 16 байт, ~O(n * log(n))

    for (int i = n - 1; i > 0; i--){ //делаем перестановки в конец массива, откладывая наибольшие элементы, O(n * log(n))
        std :: swap(arr[i], arr[0]);

        heapify(arr, 0, i); // 16, O(n * log(n))
    }
}

//функция для проверки отсортированности массива
bool check_sorted(std :: vector<int> &arr){
    bool ans = true;
    for (int i = 0; i < arr.size() - 1; i++){
        if (arr[i] > arr[i + 1]) ans = false;
    }

    return ans;
}

TEST_CASE("testing HeapSort() function"){
    std :: vector<int> test1 = {0, 0, 0, 0, 0}, test2 = {1, 2, 3, 4, 5, 6, 7, 8}, test3 = {32, 1, 343, 23, 12, 43, 65, 42, 43, 54}, test4;
    HeapSort(test1); //массив с одинаковыми числами
    HeapSort(test2); //уже отсортированный массив
    HeapSort(test3); //общий случай

    std :: fstream file_test4;
    int temp;
    file_test4.open("./tests/worst_case_heap.txt");
    while(file_test4 >> temp) test4.push_back(temp);
    HeapSort(test4); //худший случай: уже отсортированный массив (в этом случае наибольшие элементы находятся при каждой итерации в конце,
                     //из-за чего каждый раз максимуму приходится "всплывать" вверх по дереву наибольшее кол-во раз)

    CHECK(check_sorted(test1));
    CHECK(check_sorted(test2));
    CHECK(check_sorted(test3));
    CHECK(check_sorted(test4));
}