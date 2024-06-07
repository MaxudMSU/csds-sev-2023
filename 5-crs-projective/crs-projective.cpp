#include<iostream>
#include <set>
#include <list>
#include <stack>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <bits/stdc++.h>
#include "../types.h"
#define NIL -1

using namespace std;

// честно сп*зжено с этого сайта
// https://leetcodethehardway.com/tutorials/graph-theory/tarjans-algorithm
struct find_SCC {
    int timer = 0;
    // store the in-time for DFS search
    vector<int> in_time;
    // stores the low-link value for every node
    vector<int> low_link;
    // checks whether a node is on the stack or not
    vector<bool> on_stack;
    // stack to store the currently available nodes
    stack<int> stk;
    // to store the final answer
    vector<vector<int>> res;

    // recursive function to do the DFS search on the graph
    void dfs(int u, vector<vector<int>> &graph) {
        // set the values for in_time and low_link, and put the node on stack
        in_time[u] = low_link[u] = timer;
        timer++;
        stk.push(u);
        on_stack[u] = true;

        // DFS to neighbours of current node
        for (int v : graph[u]) {
            // if the node is unvisited
            if (in_time[v] == -1) {
                dfs(v, graph);
                // update the low-link value for node u
                low_link[u] = min(low_link[u], low_link[v]);
                // else if the node was visited before, and is still on the stack
            } else if (on_stack[v]) {
                // update the low-link for node u
                low_link[u] = min(low_link[u], in_time[v]);
            }
        }

        // check if u is the root node for a SCC
        if (low_link[u] == in_time[u]) {
            vector<int> SCC;
            // all the nodes above u in the stack are in SCC of u
            while (stk.top() != u) {
                int v = stk.top();
                stk.pop();
                SCC.push_back(v);
                on_stack[v] = false;
            }
            // now removing u from stack and adding it to the SCC
            stk.pop();
            SCC.push_back(u);
            on_stack[u] = false;

            // adding the SCC to the answer
            res.push_back(SCC);
        }

        return;
    }

    // takes input of graph as adjacency list and returns the SCC of graph as
    // vectors
    vector<vector<int>> tarjans(vector<vector<int>> &adjacencyList) {
        int n = adjacencyList.size();
        in_time.resize(n, -1);
        low_link.resize(n, -1);
        on_stack.resize(n, false);

        for (int u = 0; u < n; u++) {
            if (in_time[u] == -1) dfs(u, adjacencyList);
        }

        return res;
    }
};

vector<int> cell_dribling(int item, int leng, int her=0, int map_num=0){
    item -= her;
    int lengnew = leng*2;
    int newher = map_num*lengnew*lengnew;
    int new1 = 2*item - 1 + ((item-1)/leng)*lengnew;
    int new2 = new1 + 1;
    int new3 = new1 + lengnew;
    int new4 = new2 + lengnew;
    return vector<int>{new1+newher,new2+newher,new3+newher,new4+newher}; 
}

vector<int> theLoop(Area area, float h, vector<vector<float>> matrix, vector<int> vozvrat_set){
    int cou = 1;
    // Area area(-1,1,-1,1);
    float xdown=-1, xup=1, ydown=-1, yup=1;
    int pt = 8;
    int n_x=area.get_hx(h);
    int n_y=area.get_hy(h);
    int V = n_x * n_y * 3;
    vector<vector<int>> graph(V);
    
    for (int num=0; num<3; num++){
        float xtmp = xdown;
        float ytmp = yup;
        float xckl = xdown;
        float yckl = yup;
        set<int> cell_set;
        while (yckl > ydown){
            while (xckl < xup){
                for (int i=0; i<pt; i++){
                    ytmp -= h/pt;
                    for (int j=0; j<pt; j++){
                        xtmp += h/pt;
                        //локальные координаты -> объект с номером карты
                        ProjectivePoint prtmp(xtmp, ytmp, num+1);
                        //объект с номером карты -> трехмерная точка
                        Point3D tmp = fromProj(prtmp);
                        //отображённая точка -> объект с номером карты
                        ProjectivePoint rz = from3d(tmp.matr_map(matrix));
                        //подсчёт номера ячейки этой точки
                        Point rz_local = rz.get_point();
                        int cell = rz_local.cell(area, n_x, h) + n_x*n_y*num;
                        
                        // xtmp = tmp.x;
                        // ytmp = tmp.y;
                        float xrz = rz_local.x;
                        float yrz = rz_local.y;
                      
                      
                        if (!cell) continue;
                        // строка ниже по логике нужна, но с ней не строятся некоторые ячейки, поэтому убрал 
                        else if (*find(vozvrat_set.begin(), vozvrat_set.end(), cell) != cell) {continue;}
                        else cell_set.insert(cell);
                    }
                    xtmp = xckl; 
                }
                xckl += h;
                xtmp = xckl;
                ytmp = yckl;
                // cout << cou << ": ";
                for (int cel : cell_set){
                    // cout << cel << " ";
                    graph[cou-1].push_back(cel-1);
                }
                // cout << endl;
                cou+=1;
                cell_set.clear();
            }   
            yckl -= h;
            xckl = xdown;
            xtmp = xckl;
            ytmp = yckl;
        }
    }
    find_SCC t = find_SCC();
    vector<vector<int>> ans = t.tarjans(graph);
    vector<int> res;
    for (auto x : ans) {
        if (x.size() > 1) {
            res.insert(res.end(), x.begin(), x.end());
        }
    }
    return res;
    // return {};
}

void files_out(vector<int> vozvrat, int nx, int ny, Area& area, float h){
    ofstream cells_file((char*)"cells_file.osip", ios::trunc);
    ofstream points_file((char*)"points_file.osip", ios::trunc);
    for (auto elem : vozvrat){
        int cell_num = elem + 1;
        int map_num = ceil((double)cell_num / (double)(nx*ny));
        cells_file << cell_num << endl;
        pair<float,float> corner = area.cell_coord(cell_num - nx*ny*(map_num-1), h);
        points_file << map_num << endl << corner.first << endl << corner.second << endl;
    }
    cells_file.close();
    points_file.close();
}

int main(int argc, char** argv){
    vector<vector<float>> matrix;
    for(int ii=0; ii<=2; ii++){
        vector<float> temp;
        for(int jj=1; jj<=3; jj++){
            temp.push_back(atof(argv[jj+3*ii]));
        }
        matrix.push_back(temp);
    }
    float hh = atof(argv[10]);
    bool isStart = atof(argv[11]);
    Area wa(-1,1,1,-1);
    int n_x=wa.get_hx(hh);
    int n_y=wa.get_hy(hh);

    if (isStart){
        
        vector<int> vozvrat_set(n_x*n_y*3);
        iota(begin(vozvrat_set), end(vozvrat_set), 1);
        vector<int> vozvrat = theLoop(wa, hh, matrix, vozvrat_set);
        files_out(vozvrat, n_x, n_y, wa, hh);
        // ofstream cells_file((char*)"cells_file.osip", ios::trunc);
        // ofstream points_file((char*)"points_file.osip", ios::trunc);
        // for (auto elem : vozvrat){
        //     int cell_num = elem + 1;
        //     int map_num = ceil((double)cell_num / (double)(n_x*n_y));
        //     cells_file << cell_num << endl;
        //     pair<float,float> corner = wa.cell_coord(cell_num - n_x*n_y*(map_num-1), hh);
        //     points_file << map_num << endl << corner.first << endl << corner.second << endl;
        // }
        // cells_file.close();
        // points_file.close();
    }
    else{
        vector<int> vozvrat_set;
        vector<int> new_vozvr = vozvratFromFile((char*)"cells_file.osip");
        for (int elem : new_vozvr){
            int maps = (elem - 1) / (n_x*n_y);
            int her = n_x*n_y*maps;
            vector<int> temp = cell_dribling(elem, n_x, her, maps);
            vozvrat_set.insert(vozvrat_set.end(), temp.begin(), temp.end());
        }
        
        hh = hh/2;
        n_x=wa.get_hx(hh);
        n_y=wa.get_hy(hh);
        vector<int> vozvrat = theLoop(wa, hh, matrix, vozvrat_set);
        files_out(vozvrat, n_x, n_y, wa, hh);

        // vector<int> new_vozvr = vozvratFromFile((char*)"cells_file.osip");
        // ofstream points_file((char*)"points_file.osip", ios::trunc);   
        // for (auto elem : new_vozvr){
        //     int cell_num = elem;
        //     int map_num = ceil((double)cell_num / (double)(n_x*n_y));
        //     // cells_file << cell_num << endl;
        //     pair<float,float> corner = wa.cell_coord(cell_num - n_x*n_y*(map_num-1), hh);
        //     points_file << map_num << endl << corner.first << endl << corner.second << endl;
        // }
        // // cells_file.close();
        // points_file.close();
    }
    return 0;
} 
