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

// Area wa(-1.5,1.5,1.5,-1.5); double gh=0.5; 
// set<int> vozvrat_set;

// TwoDimArray<int> vozvrat(wa,gh);  

// Класс, представляющий ориентированный граф

class Graph{
    int V;    // ���������� ������
    // list<int> *adj;    // ������������ ������ ������� ���������
    void SCCUtil(int u, int disc[], int low[], stack<int> *st, bool stackMember[], Area& wrkng, double h);
public:
    list<int> *adj;
    Graph(){};
    Graph(int V);
    void addEdge(int v, int w);
    void SCC(Area& wrkng, double h);
    void print_adj();
};

void Graph::print_adj(){
    // for (auto j=1; j <= V; j++){
    //     for (auto i = adj[j].begin(); i != adj[j].end(); ++i){
    //         list<int> temp = adj[j];
    //         cout << temp[j] << " ";
    //     }

    // }
    for (auto j=0; j < V; j++){
        cout << j << ": ";
        for (auto i = adj[j].begin(); i != adj[j].end(); ++i){
            cout << *i << " ";
        }
        cout << endl;
    }
}

Graph::Graph(int V){
    this->V = V;
    adj = new list<int>[V];
}

void Graph::addEdge(int v, int w){
    adj[v].push_back(w);
}

void Graph::SCCUtil(int u, int disc[], int low[], stack<int> *st, bool stackMember[], Area& wrkng, double h){
    static int time = 0;
    // vector<int> check;
    disc[u] = low[u] = ++time;
    st->push(u);
    stackMember[u] = true;
    list<int>::iterator i;

    for (i = adj[u].begin(); i != adj[u].end(); ++i){
        int v = *i;
        // cout << v << endl;
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
            cout << corner.first << endl << corner.second << endl; 
            cout << w << endl;
            // check.push_back(w);
            stackMember[w] = false;
            st->pop();
        }
        w = (int) st->top();
        if (count) {
            pair<double,double> corner = wrkng.cell_coord(w, h);
            cout << corner.first << endl << corner.second << endl; 
            cout << w << endl;
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
    // vector<int> check;
    stack<int> *st = new stack<int>();

    for (int i = 0; i < V; i++){
        disc[i] = NIL;
        low[i] = NIL;
        stackMember[i] = false;
    }

    for (int i = 0; i < V; i++)
        if (disc[i] == NIL)
            SCCUtil(i, disc, low, st, stackMember, wrkng, h);
    // return check;
}

vector<int> cell_dribling(int item, int leng){
    int lengnew = leng*2;
    int new1 = 2*item - 1 + ((item-1)/leng)*lengnew;
    int new2 = new1 + 1;
    int new3 = new1 + lengnew;
    int new4 = new2 + lengnew;
    return vector<int>{new1,new2,new3,new4}; 
}

void zaloop1(Area& area, double h, double para, double parb, vector<int> vozvrat){
    int cou = 1;
    double xdown=area.x_left, xup=area.x_right, ydown=area.y_down, yup=area.y_top;
    double xtmp = xdown;
    double ytmp = yup;
    double xckl = xdown;
    double yckl = yup;
    int pt = 6;
    int n_x=area.get_hx(h);
    int n_y=area.get_hy(h);
    Graph g(n_x*n_y);
    unordered_set<int> cell_set;
    while (yckl > ydown){
        while (xckl < xup){
            // if (find(vozvrat.begin(), vozvrat.end(), cou) == vozvrat.end()) continue;
            for (int i=0; i<pt; i++){
                for (int j=0; j<pt; j++){
                    Point tmp(xtmp, ytmp);
                    Point rz = tmp.duffing_e(-0.8, 1, 0.25, 0.3, 1);
                    int cell = rz.cell(area, n_x, h);
                    // cout << n_x << " " << h << endl;
                    // if (cell == 2) cout << "cell #" << cou << ": " << xtmp << ", " << ytmp << " -> " << rz.x << ", " << rz.y << endl;
                    // if (cou == 10){
                        // cout << "точка ";
                        // tmp.print();
                        // cout << "перешла в ";
                        // rz.print();
                    //     cout << "ячейка " << cell << endl; 
                    // }
                    xtmp = tmp.x;
                    ytmp = tmp.y;
                    double xrz = rz.x;
                    double yrz = rz.y;
                    xtmp += h/pt;
                    
                    if (!cell) continue;
                    else if (*find(vozvrat.begin(), vozvrat.end(), cell) != cell) {continue;}
                    // else if (find(begin(vozvrat), end(vozvrat), cell) != end(vozvrat)) {continue;}
                    else cell_set.insert(cell);
                    // if ((find(begin(vozvrat), end(vozvrat), cell) != end(vozvrat)) && (cell)) {
                    // if ((find(vozvrat.begin(), vozvrat.end(), cell) != vozvrat.end()) && (cell)) {
                        // cout << "AAAA" << endl;
                        // cell_set.insert(cell);
                    // }
                    // else cout << "chzh" << endl;
                    // if (cell) cell_set.insert(cell);
                    
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
                // if (cou == 8) cout << cel << " ";
                g.addEdge(cou - 1, cel - 1);
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
    // g.print_adj();

}

int main(int argc, char** argv){
    // double x_left = -1.5;
    // double y_down = -1.5;
    // double x_right = 1.5;
    // double y_top = 1.5;
    // double par_a = 0.2;
    // double par_b = -0.2;
    // double hh = 0.25;
    // bool isStart = 1;

    double x_left = atof(argv[1]);
    double y_down = atof(argv[2]);
    double x_right = atof(argv[3]);
    double y_top = atof(argv[4]);
    double par_a = atof(argv[5]);
    double par_b = atof(argv[6]);
    double hh = atof(argv[7]);
    // int pointsamo
    bool isStart = atof(argv[8]);

    Area wa(x_left, y_top, x_right, y_down);
    int n_x=wa.get_hx(hh);
    int n_y=wa.get_hy(hh);
    // cout << n_x*n_y << " ";
    if (isStart){
        vector<int> vozvrat_set(n_x*n_y);
        iota(begin(vozvrat_set), end(vozvrat_set), 1);
        zaloop1(wa, hh, par_a, par_b, vozvrat_set);
    }
    else{
        vector<int> vozvrat_set;
        // vector<int> new_vozvr = vozvratFromFile((char*)"cells_file.osip");
        vector<int> new_vozvr;
        int num;
        while (cin >> num){
            int cel = (int)num;
            new_vozvr.push_back(cel);
        }
        for (int elem : new_vozvr){
            vector<int> temp = cell_dribling(elem, n_x);
            vozvrat_set.insert(vozvrat_set.end(), temp.begin(), temp.end());
        }
        hh = hh/2;
        zaloop1(wa, hh, par_a, par_b, vozvrat_set);
    }
    return 0;
} 
