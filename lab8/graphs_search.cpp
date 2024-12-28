#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <algorithm>
#include <ctime>
typedef long double ld;

struct node{ // структура узла
    int id; 
    ld x, y; // координаты узла
    std :: vector<std :: pair<node*, ld>> adj = {}; // массив соседей

    node(ld lon, ld lat){ // конструктор для быстрого задания координат нового узла
        x = lon;
        y = lat;
    }

    void add_neighbor(node* neighbor, ld weight){ // функция для добавления соседей, O(1)
        adj.push_back(std :: pair(neighbor, weight));
    }

    void print(){ // функция для вывода координат данного узла и координат всех его соседей (с весами соответствующих ребер)
        std :: cout << x << " " << y << ": ";
        for (auto i: adj){
            std :: cout << i.first->x << " " << i.first->y << " " << i.second <<"; ";
        }
        std :: cout << std :: endl;
    }

    void dry_print(){ // функция вывода координат узла
        std :: cout << x << " " << y << "\n";
    }
};

struct graph{
    std :: vector<node*> nodes = {}; // массив всех узлов в графе
    std :: unordered_map<std :: string, node*> coords_in_nodes; // хеш-таблица для сопоставления координат и узла
    

    // эвристика по евклидову расстоянию
    // так как мы находимся относительно на плоской поверхности при подсчете расстояний между узлами,
    // можем сказать, что координаты, которые идут по сферическому пространству, сопоставимы с 
    // координатами в декартовой плоскости
    ld get_dist(node* first, node* second){
        return sqrt(pow(first->x - second->x, 2) + pow(first->y - second->y, 2));
    } 


    // функция для удаления ориентированности графа; принцип - добавить для всех существующих
    // ориентированных ребер обратные
    void unorientate(){
        for (auto i: nodes){
            for (auto j: i->adj){
                j.first->adj.push_back(std :: pair(i, j.second));
            }
        }
    }


    // функция генерации ключа на основе координат узла для хеш-таблицы coords_in_nodes
    std :: string generateKey(ld &lon, ld &lat){
        return std :: to_string(lon) + std :: to_string(lat);
    }

    // функция нахождения узла по координатам, O(1), +88 байт
    node* find_node(ld &lon, ld &lat){
        std :: string key = generateKey(lon, lat);
        node* found_node = nullptr; // +8

        // если соответствующий узел есть -> возвращаем указатель на него
        if (coords_in_nodes.find(key) != coords_in_nodes.end()) return coords_in_nodes[key]; // O(1) - поиск
        
        // если такого узла нет -> создаем его
        found_node = new node(lon, lat); // +80
        found_node->id = nodes.size();
        nodes.push_back(found_node);
        coords_in_nodes[key] = found_node;

        return found_node;
    }

    // функция чтения узлов из файла
    void read_from_file(std :: fstream &file){
        std :: string line;
        char temp; // для того, чтобы хранить , ; :
        ld lon, lat, weight;

        while (std :: getline(file, line)){ // читаем построчно
            std :: stringstream ss(line);

            ss >> lon >> temp >> lat >> temp; // берем изначальный узел (координаты до ":")
            node* init_node = find_node(lon, lat); // находим его (или создаем)

            while(true){ // находим соседей данного узла и добавляем их в список смежных вершин
                ss >> lon >> temp >> lat >> temp >> weight >> temp;
                if (ss.eof()) break;
                init_node->add_neighbor(find_node(lon, lat), weight);
            }
        }

        std :: cout << "Done! \n";
    }

    // функция нахождения приблизительного (близжайшего по координатам) узла без создания новых, 88 байт
    node* find_approx_node(ld lon, ld lat){
        ld min_dist = 900000, curr_dist;
        node* closest = nullptr;
        node* target = new node(lon, lat); // +88 (указатель + node)

        for (auto n: nodes){ // O(v), где v - кол-во вершин
            curr_dist = get_dist(n, target);
            if (curr_dist < min_dist){
                min_dist = curr_dist;
                closest = n;
            }
        }

        delete target;
        return closest;
    }

    // структура для создания функтора, который будет использоваться для поддержания
    // отсортированного состояния в приоритетной очереди
    struct compareWays{
        bool operator()(std :: pair<node*, ld> &p1, std :: pair<node*, ld> &p2){
            return p1.second > p2.second;
        }
    };
    
    // метод нахождения пути на основе обхода bfs, O(v + e), где v - кол-во вершин, e - кол-во ребер, 232 + 8v байт в худшем случае
    std :: vector<node*> bfs(node* n1, node* n2){
        std :: unordered_map<node*, node*> prev_in_path; // хеш-таблица для сопоставления предыдущего узла в пути, +56
        std :: vector<node*> path; // сам путь, +24
        std :: unordered_map<node*, bool> visited; // сопоставление посещения узла и самого узла, +56

        for (auto n: nodes){ // O(v)
            prev_in_path[n] = nullptr;
            visited[n] = false;
        }

        std :: queue<node*> q; // используем очередь, чтобы рассматривать все пути в ширину, +80 

        q.push(n1); // посещаем первый узел (начальный)
        visited[n1] = true;

        while(!q.empty()){ // в худшем случае пройдемся по всем вершинам и ребрам: O(v + e)
            auto curr = q.front(); // +8
            q.pop(); // берем узел спереди

            for (auto i: curr->adj){ // рассматриваем всех его соседей, худший случай O(e)
                if (i.first == n2){ // если есть конечный узел -> нашли путь
                    // восстанавливаем путь по предыдущим в пути узлам
                    path.push_back(n2);
                    prev_in_path[n2] = curr;
                    node* prev = n2; // +8

                    while(prev != n1){ // в худшем случае пройдемся по всем вершинам O(v)
                        prev = prev_in_path[prev];
                        path.push_back(prev);
                    }

                    std :: reverse(path.begin(), path.end());
                    return path;
                }
                if (!visited[i.first]){ // если еще не посетили данный узел
                    q.push(i.first); // добавляем в очередь, +8, в общем случае (худшем) + 8 * (v - 1)
                    visited[i.first] = true; // значит, теперь мы его посетили
                    prev_in_path[i.first] = curr; // для данного узла мы знаем предыдущий в пути от n1
                }
            }
        }

        return path; // если пути нет, то здесь оно вернет пустой массив (заполненный вернет при условии нахождения n2)
    }


    // метод нахождения пути на основе обхода dfs
    // все почти* то же самое, что и в bfs, но вместо очереди стек, чтобы сначала рассматривать пути в глубину, O(v + e), 232 + 8v байт
    // * - см. порядок рассмотрения смежных узлов
    std :: vector<node*> dfs(node* n1, node* n2){

        std :: unordered_map<node*, node*> prev_in_path;
        std :: vector<node*> path;
        std :: unordered_map<node*, bool> visited;
        for (auto n: nodes){
            prev_in_path[n] = nullptr;
            visited[n] = false;
        }

        std :: stack<node*> q;

        q.push(n1);
        visited[n1] = true;

        while(!q.empty()){
            auto curr = q.top();
            q.pop();
            
            // здесь рассматриваем узлы в обратном порядке, чтобы на верху стека находился узел, который
            // шел первее в массиве смежных узлов
            for (int j = curr->adj.size() - 1; j >= 0; j--){
                auto i = curr->adj[j];

                if (i.first == n2){
                    path.push_back(n2);
                    prev_in_path[n2] = curr;
                    node* prev = n2;

                    while(prev != n1){
                        prev = prev_in_path[prev];
                        path.push_back(prev);
                    }

                    std :: reverse(path.begin(), path.end());
                    return path;
                }

                if (!visited[i.first]){
                    q.push(i.first);
                    visited[i.first] = true;
                    prev_in_path[i.first] = curr;
                }

            }
        }

        return path;
    }

    // функция нахождения кратчайшего пути на основе алгоритма Дейкстры, 200 + 32v байт, O(v) + O((v + e) * log(v)) = O((v + e) * log(v))
    std :: vector<node*> dijkstra(node* n1, node* n2){
        std :: unordered_map<node*, ld> dists; // сопоставление узла и кратчайшего расстояния от n1 до него, +56
        std :: unordered_map<node*, node*> prev_in_path; // сопоставления узла и прыдудщего ему в пути, +56
        std :: vector<node*> path = {}; // сам путь, +24
        for (auto n: nodes){ // O(v)
            dists[n] = 9000000; // ставим всем узлам условно бесконечное расстояние (то есть любое другое
                                // реальное расстояние будет меньше данного)

            prev_in_path[n] = nullptr;
        }
        dists[n1] = 0; // кроме начального узла: для него расстояние 0

        // приоритетная очередь, в которой сортировка будет происходит по такому прицнипу: узел с кратчайшим расстояние будет
        // в приоритете; для сортировки используем функтор compareWays (см. выше)
        std :: priority_queue<std :: pair<node*, ld>, std :: vector<std :: pair<node*, ld>>, compareWays> pq; // +32 + (худший случай) 32v
        
        pq.push(std :: pair(n1, 0)); // добавляем начальную вершину в очередь

        // по факту, после извлечения вершины из очереди, мы считаем, что нашли минимальное до нее расстояние ->
        // повторного обновления ее соседей не будет -> временная сложность складывается из суммарного рассмотрения
        // вершин и их ребер: O(v * log(v)) + O(e * log(v)) -> O((e + v) * log(v))
        while (!pq.empty()){ 
            auto curr = pq.top(); // +32
            pq.pop(); // берем передний узел (с минимальным на данный момент расстоянием в очереди), O(log(v))

            // если нашли конечный узел -> строим путь; данный путь будет минимальным 
            if (curr.first == n2){ 
                node* i = n2;
                while(i != n1){
                    path.push_back(i);
                    i = prev_in_path[i];
                }

                path.push_back(n1);

                std :: reverse(path.begin(), path.end());

                return path;
            }

            // рассматриваем всех соседей, O(e * log(v)) в общем случае (суммарно: мы обрабатываем каждого соседа, потенциально
            // добавляя каждого в очередь)
            for (auto i: curr.first->adj){
                if (dists[i.first] > curr.second + i.second){ // если нашли более короткий путь через curr
                    prev_in_path[i.first] = curr.first; // меняем путь до данного узла на путь через curr
                    dists[i.first] = dists[curr.first] + i.second; // изменяем минимальное расстояние
                    pq.push(std :: pair(i.first, dists[i.first])); // смотрим обновление данного соседа в очереди, O(log(v)) в худшем случае, +32
                }
            }
        }

        return path;
    }

    // функция нахождения пути на основе модифицированного алгоритма Дейкстры (с использованием эвристики)
    // в силу схожести с алгоритмом Дейкстры, можно считать, что асимптотика та же - O((v + e) * log(v)),
    // 256 + 32v байт
    std :: vector<node*> a_star(node* n1, node* n2){
        std :: unordered_map<node*, ld> dists, dists_h; // создаем также хеш-таблицу с предполагаемыми расстояниями, 56 + 56 = 112 
        std :: unordered_map<node*, node*> prev_in_path; // +56
        std :: vector<node*> path = {}; // +24
        for (auto n: nodes){
            dists[n] = 9000000;
            dists_h[n] = 9000000;
            prev_in_path[n] = nullptr;
        }
        dists[n1] = 0;
        dists_h[n1] = get_dist(n1, n2);

        // сейчас в очереди храним не расстояние от начального узла до данного, а предположительное расстояние
        // (минимальное от n1 до данного + эвристика)
        std :: priority_queue<std :: pair<node*, ld>, std :: vector<std :: pair<node*, ld>>, compareWays> pq; // +32 + (худший случай) 32v
        
        pq.push(std :: pair(n1, dists_h[n1])); // принцип действия очень схож с дейкстрой
        while (!pq.empty()){
            auto curr = pq.top(); // +32
            pq.pop();

            if (curr.first == n2){
                node* i = n2;
                while(i != n1){
                    path.push_back(i);
                    i = prev_in_path[i];
                }

                path.push_back(n1);

                std :: reverse(path.begin(), path.end());

                return path;
            }

            for (auto i: curr.first->adj){ 
                ld raw_dist = dists[curr.first] + i.second;
                if (dists[i.first] > raw_dist){ // однако тут мы записываем не только минимальное расстояние от n1,
                    prev_in_path[i.first] = curr.first;
                    dists[i.first] = raw_dist;
                    dists_h[i.first] = dists[i.first] + get_dist(i.first, n2); // но и минимальное предположительное
                    pq.push(std :: pair(i.first, dists_h[i.first]));
                }
            }
        }

        return path;
    }

    // функция вывода всех узлов в графе
    void print(){
        for (auto i: nodes) i->print();
    }

    // деструктор; нужен, так как в find_node использовали new
    ~graph(){
        for (node* i: nodes) delete i;
    }

};

// функция нахождения суммы весов ребер на пути
ld get_path_sum(std :: vector<node*> &path){
    ld sum = 0;
    bool found;
    if (path.empty()) return 0;
    for (int i = 0; i < path.size() - 1; i++){
        found = false;
        for (auto j: path[i]->adj){
            if (path[i + 1] == j.first){
                sum += j.second;
                found = true;
                break;
            }
        }
        if (!found) return -1; // если вдруг есть несмежные узлы -> возвращаем -1
    }

    return sum;
}

int main(){
    std :: fstream file;

    // можно ввести другой файл с графом (тестовые файлы test_file.txt, test_file_2.txt)
    file.open("spb_graph.txt");
    graph a;
    a.read_from_file(file); // читаем узлы из файла


    // вводим координаты нужных узлов
    node* n1 = a.find_approx_node(30.369008, 59.885145);
    node* n2 = a.find_approx_node(30.337795,59.926835); // находим приблизительные узлы

    ld sum_of_path;
    a.unorientate(); // так как данный граф слабосвязный -> сделаем его неориентированным, чтобы гарантировать, что
                     // для каждой пары узлов найдется путь

    // для каждого метода поиска пути засечем время в мс и выведем размер пути (кол-во вершин), а также сумму весов ребер на пути
    int start = clock();
    std :: vector<node*> path = a.bfs(n1, n2);
    std :: cout << "bfs: " << clock() - start << ", num of nodes: " << path.size() << ", path sum = " << get_path_sum(path) << std :: endl;
    //for (auto i: d) i->dry_print();

    start = clock();
    path = a.dfs(n1, n2);
    std :: cout << "dfs: " << clock() - start << ", num of nodes: " << path.size() << ", path sum = " << get_path_sum(path) << std :: endl;
    //for (auto i: d) i->dry_print();

    start = clock();
    path = a.dijkstra(n1, n2);
    std :: cout << "dijkstra: " << clock() - start << ", num of nodes: " << path.size() << ", path sum = " << get_path_sum(path) << std :: endl;
    //for (auto i: d) i->dry_print();

    start = clock();
    path = a.a_star(n1, n2);
    std :: cout << "a_star: " << clock() - start << ", num of nodes: " << path.size() << ", path sum = " << get_path_sum(path) << std :: endl;
    //for (auto i: d) i->dry_print();
}