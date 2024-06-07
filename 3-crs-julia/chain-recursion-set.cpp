#include<iostream>
#include <set>
#include <list>
#include <stack>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include "../types.h"
#define NIL -1

using namespace std;

// Класс, представляющий ориентированный граф
// честно сп*зжено и модифицировано с этого сайта
// http://web.archive.org/web/20220702233458/http://espressocode.top/tarjan-algorithm-find-strongly-connected-components/
class Graph{
    int V;
    void SCCUtil(int u, int disc[], int low[], stack<int> *st, bool stackMember[], Area& wrkng, double h);
public:
    list<int> *adj;
    Graph(){};
    Graph(int V);
    void addEdge(int v, int w);
    void SCC(Area& wrkng, double h);
};

Graph::Graph(int V){
    this->V = V;
    adj = new list<int>[V];
}

void Graph::addEdge(int v, int w){
    adj[v].push_back(w);
}

void Graph::SCCUtil(int u, int disc[], int low[], stack<int> *st, bool stackMember[], Area& wrkng, double h){
    static int time = 0;
    disc[u] = low[u] = ++time;
    st->push(u);
    stackMember[u] = true;
    list<int>::iterator i;

    for (i = adj[u].begin(); i != adj[u].end(); ++i){
        int v = *i;
        if (disc[v] == -1){
            SCCUtil(v, disc, low, st, stackMember, wrkng, h);
            low[u]  = min(low[u], low[v]);
        }
        else if (stackMember[v] == true)
            low[u]  = min(low[u], disc[v]);
    }

    int w = 1;
    
    if (low[u] == disc[u]){
        int count = 0;
        while (st->top() != u){
            count = 1;
            w = (int) st->top();
            pair<double,double> corner = wrkng.cell_coord(w, h);
            cout << corner.first << " " << corner.second << " "; 
            cout << w << " ";
            stackMember[w] = false;
            st->pop();
        }
        w = (int) st->top();
        // костыль
        if (count) {
            pair<double,double> corner = wrkng.cell_coord(w, h);
            cout << corner.first << " " << corner.second << " "; 
            cout << w << " ";
        }
        count = 0;
        stackMember[w] = false;
        st->pop();
    }
}

void Graph::SCC(Area& wrkng, double h){
    int *disc = new int[V];
    int *low = new int[V];
    bool *stackMember = new bool[V];
    stack<int> *st = new stack<int>();

    for (int i = 0; i < V; i++){
        disc[i] = NIL;
        low[i] = NIL;
        stackMember[i] = false;
    }

    for (int i = 1; i <= V; i++)
        if (disc[i] == NIL)
            SCCUtil(i, disc, low, st, stackMember, wrkng, h);
}

vector<int> cell_dribling(int item, int leng){
    int lengnew = leng*2;
    int new1 = 2*item - 1 + ((item-1)/leng)*lengnew;
    int new2 = new1 + 1;
    int new3 = new1 + lengnew;
    int new4 = new2 + lengnew;
    return vector<int>{new1,new2,new3,new4}; 
}

void theLoop(Area& area, double h, double para, double parb, vector<int> vozvrat){
    int cou = 1;
    double xdown=area.x_left, xup=area.x_right, ydown=area.y_down, yup=area.y_top;
    double xtmp = xdown;
    double ytmp = yup;
    double xckl = xdown;
    double yckl = yup;
    int pt = 8;
    int n_x=area.get_hx(h);
    int n_y=area.get_hy(h);
    Graph g(n_x*n_y);
    unordered_set<int> cell_set;
    while (yckl > ydown){
        while (xckl < xup){
            for (int i=0; i<pt; i++){
                for (int j=0; j<pt; j++){
                    Point tmp(xtmp, ytmp);
                    Point rz = tmp.julia1(para, parb);
                    int cell = rz.cell(area, n_x, h);
                    xtmp = tmp.x;
                    ytmp = tmp.y;
                    double xrz = rz.x;
                    double yrz = rz.y;
                    xtmp += h/pt;
                    
                    if (!cell) continue;
                    else if (*find(vozvrat.begin(), vozvrat.end(), cell) != cell) {continue;}
                    else cell_set.insert(cell);
                }
                xtmp = xckl;
                ytmp -= h/pt;
            }
            xckl += h;
            xtmp = xckl;
            ytmp = yckl;
            // cout << cou << ": ";
            for (int cel : cell_set){
                // cout << cel << " ";
                g.addEdge(cou, cel);
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
    g.SCC(area, h);
}

int main(int argc, char** argv){
    double x_left = atof(argv[1]);
    double y_down = atof(argv[2]);
    double x_right = atof(argv[3]);
    double y_top = atof(argv[4]);
    double par_a = atof(argv[5]);
    double par_b = atof(argv[6]);
    double hh = atof(argv[7]);
    bool isStart = atof(argv[8]);

    Area wa(x_left, y_top, x_right, y_down);
    int n_x=wa.get_hx(hh);
    int n_y=wa.get_hy(hh);
    if (isStart){
        vector<int> vozvrat_set(n_x*n_y);
        iota(begin(vozvrat_set), end(vozvrat_set), 1);
        theLoop(wa, hh, par_a, par_b, vozvrat_set);
    }
    else{
        vector<int> vozvrat_set;
        vector<int> new_vozvr = vozvratFromFile((char*)"file.osip", 3);
        for (int elem : new_vozvr){
            vector<int> temp = cell_dribling(elem, n_x);
            vozvrat_set.insert(vozvrat_set.end(), temp.begin(), temp.end());
        }
        hh = hh/2;
        theLoop(wa, hh, par_a, par_b, vozvrat_set);
    }
    return 0;
} 
