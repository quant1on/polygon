#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <queue>
#include <stack>
#include <unordered_map>
#include <algorithm>
#include <ctime>
typedef long double ld;

struct node{
    int id;
    ld x, y;
    std :: vector<std :: pair<node*, ld>> adj = {};

    node(ld lon, ld lat){
        x = lon;
        y = lat;
    }

    void add_neighbor(node* neighbor, ld weight){
        adj.push_back(std :: pair(neighbor, weight));
    }

    void print(){
        std :: cout << x << " " << y << ": ";
        for (auto i: adj){
            std :: cout << i.first->x << " " << i.first->y << " " << i.second <<"; ";
        }
        std :: cout << std :: endl;
    }

    void dry_print(){
        std :: cout << x << " " << y << "\n";
    }
};

struct graph{
    std :: vector<node*> nodes = {};
    std :: unordered_map<std :: string, node*> coords_in_nodes;
    
    ld get_dist(node* first, node* second){
        return sqrt(pow(first->x - second->x, 2) + pow(first->y - second->y, 2));
    }

    void unorientate(){
        for (auto i: nodes){
            for (auto j: i->adj){
                j.first->adj.push_back(std :: pair(i, j.second));
            }
        }
    }

    std :: string generateKey(ld &lon, ld &lat){
        return std :: to_string(lon) + std :: to_string(lat);
    }

    node* find_node(ld &lon, ld &lat){
        std :: string key = generateKey(lon, lat);
        node* found_node = nullptr;

        if (coords_in_nodes.find(key) != coords_in_nodes.end()) return coords_in_nodes[key];
        

        found_node = new node(lon, lat);
        found_node->id = nodes.size();
        nodes.push_back(found_node);
        coords_in_nodes[key] = found_node;

        return found_node;
    }

    void read_from_file(std :: fstream &file){
        std :: string line;
        char temp;
        ld lon, lat, weight;

        while (std :: getline(file, line)){
            std :: stringstream ss(line);

            ss >> lon >> temp >> lat >> temp;
            node* init_node = find_node(lon, lat);

            while(true){
                ss >> lon >> temp >> lat >> temp >> weight >> temp;
                if (ss.eof()) break;
                init_node->add_neighbor(find_node(lon, lat), weight);
            }
        }

        std :: cout << "Done! \n";
    }

    node* find_approx_node(ld lon, ld lat){
        ld min_dist = 900000, curr_dist;
        node* closest = nullptr;
        node* target = new node(lon, lat);

        for (auto n: nodes){
            curr_dist = get_dist(n, target);
            if (curr_dist < min_dist){
                min_dist = curr_dist;
                closest = n;
            }
        }

        delete target;
        return closest;
    }

    struct compareWays{
        bool operator()(std :: pair<node*, ld> &p1, std :: pair<node*, ld> &p2){
            return p1.second > p2.second;
        }
    };
    
    std :: vector<node*> bfs(node* n1, node* n2){
        std :: unordered_map<node*, node*> prev_in_path;
        std :: vector<node*> path;
        std :: unordered_map<node*, bool> visited;
        for (auto n: nodes){
            prev_in_path[n] = nullptr;
            visited[n] = false;
        }

        std :: queue<node*> q;

        q.push(n1);
        visited[n1] = true;

        while(!q.empty()){
            auto curr = q.front();
            q.pop();

            for (auto i: curr->adj){
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

    std :: vector<node*> dijkstra(node* n1, node* n2){
        std :: unordered_map<node*, ld> dists;
        std :: unordered_map<node*, node*> prev_in_path;
        std :: vector<node*> path = {};
        for (auto n: nodes){
            if (n == n1) dists[n] = 0;
            else dists[n] = 9000000;

            prev_in_path[n] = nullptr;
        }

        std :: priority_queue<std :: pair<node*, ld>, std :: vector<std :: pair<node*, ld>>, compareWays> pq;
        
        pq.push(std :: pair(n1, 0));
        while (!pq.empty()){
            auto curr = pq.top();
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
                if (dists[i.first] > curr.second + i.second){
                    prev_in_path[i.first] = curr.first;
                    dists[i.first] = dists[curr.first] + i.second;
                    pq.push(std :: pair(i.first, dists[i.first]));
                }
            }
        }

        return path;
    }

    std :: vector<node*> a_star(node* n1, node* n2){
        std :: unordered_map<node*, ld> dists, dists_h;
        std :: unordered_map<node*, node*> prev_in_path;
        std :: vector<node*> path = {};
        for (auto n: nodes){
            dists[n] = 9000000;
            dists_h[n] = 9000000;
            prev_in_path[n] = nullptr;
        }
        dists[n1] = 0;
        dists_h[n1] = get_dist(n1, n2);

        std :: priority_queue<std :: pair<node*, ld>, std :: vector<std :: pair<node*, ld>>, compareWays> pq;
        
        pq.push(std :: pair(n1, dists_h[n1]));
        while (!pq.empty()){
            auto curr = pq.top();
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
                if (dists[i.first] > raw_dist){
                    prev_in_path[i.first] = curr.first;
                    dists[i.first] = raw_dist;
                    dists_h[i.first] = dists[i.first] + get_dist(i.first, n2);
                    pq.push(std :: pair(i.first, dists_h[i.first]));
                }
            }
        }

        return path;
    }

    void print(){
        for (auto i: nodes) i->print();
    }

    ~graph(){
        for (node* i: nodes) delete i;
    }

};

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
        if (!found) return -1;
    }

    return sum;
}

int main(){
    std :: fstream file;
    file.open("spb_graph.txt");
    graph a;
    a.read_from_file(file);

    node* n1 = a.find_approx_node(30.369008, 59.885145);
    node* n2 = a.find_approx_node(30.337795,59.926835);

    ld sum_of_path;
    a.unorientate();

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