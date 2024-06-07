

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

vector<int> cell_dribling(int item, int leng){
    int lengnew = leng*2;
    int new1 = 2*item - 1 + ((item-1)/leng)*lengnew;
    int new2 = new1 + 1;
    int new3 = new1 + lengnew;
    int new4 = new2 + lengnew;
    return vector<int>{new1,new2,new3,new4}; 
}

vector<int> theLoop(Area& area, float h, float para, float parb, vector<int> vozvrat_set){
    int cou = 1;
    float xdown=area.x_left, xup=area.x_right, ydown=area.y_down, yup=area.y_top;
    float xtmp = xdown;
    float ytmp = yup;
    float xckl = xdown;
    float yckl = yup;
    int pt = 8;
    int n_x=area.get_hx(h);
    int n_y=area.get_hy(h);
    int V = n_x * n_y;
    vector<vector<int>> graph(V);
    set<int> cell_set;
    while (yckl > ydown){
        while (xckl < xup){
            for (int i=0; i<pt; i++){
                ytmp -= h/pt;
                for (int j=0; j<pt; j++){
                    xtmp += h/pt;
                    Point tmp(xtmp, ytmp);
                    // Point rz = tmp.duffing_e(-0.8, 1, 0.25, 0.3, 1);
                    Point rz = tmp.duffing_e(-1, 1, 0.25, 0.3, 1);
                    int cell = rz.cell(area, n_x, h);
                    // xtmp = tmp.x;
                    // ytmp = tmp.y;
                    float xrz = rz.x;
                    float yrz = rz.y;
                    
                    
                    if (!cell) continue;
                    // строка ниже по логике нужна, но с ней не строятся некоторые ячейки, поэтому убрал 
                    // else if (*find(vozvrat_set.begin(), vozvrat_set.end(), cell) != cell) {continue;}
                    else cell_set.insert(cell);
                }
                xtmp = xckl;
                
            }
            xckl += h;
            xtmp = xckl;
            ytmp = yckl;
            // cout << cou << ": ";
            for (int cel : cell_set){
            //     cout << cel << " ";
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
    find_SCC t = find_SCC();
    vector<vector<int>> ans = t.tarjans(graph);
    vector<int> res;
	  for (auto x : ans) {
      if (x.size() > 1) {
        res.insert(res.end(), x.begin(), x.end());
      }
	  }
    return res;
}

int main(int argc, char** argv){
    float x_left = atof(argv[1]);
    float y_down = atof(argv[2]);
    float x_right = atof(argv[3]);
    float y_top = atof(argv[4]);
    float par_alpha = atof(argv[5]);
    float par_beta = atof(argv[6]);
    float par_k = atof(argv[7]);
    float par_b = atof(argv[8]);
    float par_omega = atof(argv[9]);
    float hh = atof(argv[10]);
    bool isStart = atof(argv[11]);

    Area wa(x_left, y_top, x_right, y_down);
    int n_x=wa.get_hx(hh);
    int n_y=wa.get_hy(hh);
    if (isStart){
        vector<int> vozvrat_set(n_x*n_y);
        iota(begin(vozvrat_set), end(vozvrat_set), 1);
        vector<int> vozvrat = theLoop(wa, hh, 0, 0, vozvrat_set);
        ofstream cells_file((char*)"cells_file.osip", ios::trunc);
        ofstream points_file((char*)"points_file.osip", ios::trunc);
        for (auto elem : vozvrat){
            cells_file << elem + 1 << endl;
            pair<float,float> corner = wa.cell_coord(elem + 1, hh);
            points_file << corner.first << endl << corner.second << endl;
        }
        cells_file.close();
        points_file.close();
    }
    else{
        vector<int> vozvrat_set;
        vector<int> new_vozvr = vozvratFromFile((char*)"cells_file.osip");
        for (int elem : new_vozvr){
            vector<int> temp = cell_dribling(elem, n_x);
            vozvrat_set.insert(vozvrat_set.end(), temp.begin(), temp.end());
        }
        hh = hh/2;
        vector<int> vozvrat = theLoop(wa, hh, 0, 0, vozvrat_set);
        ofstream cells_file((char*)"cells_file.osip", ios::trunc);
        ofstream points_file((char*)"points_file.osip", ios::trunc);
        for (auto elem : vozvrat){
            cells_file << elem << endl;
            pair<float,float> corner = wa.cell_coord(elem, hh);
            points_file << corner.first << endl << corner.second << endl;
        }
        cells_file.close();
        points_file.close();
    }
    return 0;
} 
