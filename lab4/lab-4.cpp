#include <iostream>
#include <vector>
#include <limits>
#include <ctime>

//тестирующий фреймворк
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../source/doctest.h"

// реализация алгоритма кластеризации массива посредством полного перебора всех комбинаций значений этого массива
// кол-во кластеров = 6
// общая асимптотика ~= O(6^n), общий расход памяти (в абсолютном пике) = 147 + 14n
// условимся, что n - кол-во элементов в массиве, все значения расхода памяти указаны в байт

// структура масок кластеров
struct cluster_masks{
    std::vector<short> curr_mask;   // текущая маска, расход = 2 * n
    bool done;                      // были ли рассмотрены всевозможные маски, расход = 1
    int n;                          // расход = 4
    bool encounter[6];              // массив, который показывает, встретился ли номер k-ого кластера в маске (см. check_mask()), расход = 6 * 1 = 6
    cluster_masks(int n_){          // расход = 2 * n + 5
        n = n_;
        curr_mask = std::vector<short> (n, 0);
        done = false;
    }

    // по факту, маска - это число в шестеричной системе счисления длиной n (значение маски на k-ой позиции задает,
    // в каком кластере находится k-ый элемент массива, номера кластеров - от 0 до 5)
    // это означает, что взятие следующей маски можно реализовать как взятие следующего подходящего числа в шестеричной системе
    // асимптотика = O(k * n) ~= O(n), где k - кол-во повторений взятия след маски
    void get_next(){
        if (curr_mask[0] == 6){ // это случится лишь тогда, когда маска примет вид 6 0 0 0...0 0 0, что будет означать,
            done = true;        // что все маски до данной были рассмотрены
            return;
        }

        curr_mask[n-1]++;                       // увеличиваем младший разряд
        if (curr_mask[n-1] == 6){               // надо ли обновлять следующие разряды
            for (int i = n-2; i >= 0; i--){     // в лучшем случае O(1), в худшем = O(n), берем O(n)
                if (curr_mask[i + 1] == 6){     // обновление текущего разряда, если предыдущий превысил 5
                    curr_mask[i]++;
                    curr_mask[i+1] = 0;
                }

                if (curr_mask[i] < 6) break;    // если текущий разряд меньше 6, то следующие обновления не требуют
            }
        }

        if (!check_mask()) get_next(); // если маска не подходит (см. описание check_mask()), то берем следующую маску, возможная асимптотика = O(n)
    }

    // функция проверки маски на условие "каждый подмассив на выходе должен содержать как минимум один элемент"
    // по факту, это проверка на то, содержит ли маска каждое число от 0 до 5 (что означает, что есть хотя бы по 1 числу в каждом кластере)
    // общая асимптотика = O(n)
    bool check_mask(){
        for (int i = 0; i < 6; i++) encounter[i] = false;               // O(6) ~= O(1)
        for (int i = 0; i < n; i++) encounter[curr_mask[i]] = true;     // O(n)
        for (int i = 0; i < 6; i++) if(!encounter[i]) return false;     // O(6) ~= O(1)
        return true;
    }
};

// функция подсчета метрики для данной маски
// асимптотика ~= O(n), расход =  120
double get_score(std :: vector<short> &mask, std :: vector<double> &arr){
    std :: vector<double> sum_of_cluster(6, 0), // массив для хранения суммы элементов в каждом кластере, расход = 6 * 8 = 48
    cluster_average(6);                         // массив для хранения среднего значения кластера, расход = 6 * 8 = 48
    std :: vector<int> cluster_size(6, 0);      // массив для хранения размера каждого кластера, расход = 6 * 4 = 24
    for (int i = 0; i < arr.size(); i++){       // O(n)
        sum_of_cluster[mask[i]] += arr[i];
        cluster_size[mask[i]]++;
    }

    for (int i = 0; i < 6; i++) cluster_average[i] = sum_of_cluster[i]/cluster_size[i]; // вычисление средних значений, O(6) ~= O(1)

    double mean_score = 0;
    for (int i = 0; i < arr.size(); i ++) mean_score += abs(arr[i] - cluster_average[mask[i]]); // подсчет метрики, O(n)

    return mean_score;
}
// сама функция кластеризации
// асимптотика ~= O(6^n) , расход = 2n + 11 + 16 + 4n + 8n + 120 = 147 + 14n
std :: vector<std :: vector<double>> clusterize(std :: vector<double> &arr){
    if (arr.size() < 6) return {};          // если меньше 6 элементов -> будет хотя бы 1 кластер с 0 элементами
    cluster_masks masks(arr.size());        // 2n + 5 + 6 = 2n + 11
    masks.get_next();                       // так как изначально маска задается 0 0 0 ... 0 0, возьмем след. подходящую, O(n)
    double best_score = std :: numeric_limits<double> :: max(), current_score; // 8 + 8 = 16
    std :: vector<short> best_mask;         // сюда запишем лучшую маску, так что 4n

    while(!masks.done){                     // так как рассмотрим абсолютно все маски -> рассмотрим все числа шестеричной системы от 0 до 5 5 5 ... 5 -> 6^n вариантов, O(6^n)
        current_score = get_score(masks.curr_mask, arr); // O(n), расход = 120
        if (current_score < best_score){    // если нашли маску лучше -> обновляем
            best_score = current_score;
            best_mask = masks.curr_mask;
        }
        masks.get_next(); // O(n)
    }

    std :: vector<std :: vector<double>> answer(6);                              // финальное разбиение, 8n
    for (int i = 0; i < arr.size(); i++) answer[best_mask[i]].push_back(arr[i]); // O(n)

    return answer;
}
//тестики (время выполнения ~20 сек)
TEST_CASE("testing clusterization function"){
    std :: vector<double> test1 = {1, 1, 1, 1, 1, 1, 1},    // 7 одинаковых элементов
    test2 = {},     // проверка с пустым массивом данных / массивом данных с менее, чем 6 числами
    test3 = {-100.98, -107.85, -28, -25.94, 52.9, 50.6, 1.864, 0, 526, 1000};   // 10 различных элементов
    
    std :: vector<std :: vector<double>> ans1 = {{1, 1}, {1}, {1}, {1}, {1}, {1}},
    ans2 = {},
    ans3 = {{-100.98, -107.85}, {-28, -25.94}, {52.9, 50.6}, {1.864, 0}, {526}, {1000}};
    
    CHECK(clusterize(test1) == ans1);
    CHECK(clusterize(test2) == ans2);
    CHECK(clusterize(test3) == ans3);
}