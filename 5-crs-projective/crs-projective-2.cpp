#include <bits/stdc++.h>
// #include <windows.h>
#include <math.h>
#include <iostream>
#include <ctime>
#include <map>
#include <vector>
#include <set>
#include "../types.h"

#define L 2
#define H 2
#define v 1 //������ ������ ��������� ���������

// #define a11 2
// #define a12 -1
// #define a13 2

// #define a21 4
// #define a22 -3
// #define a23 2

// #define a31 -1
// #define a32 0
// #define a33 -2

#define scale 100
// #define iterations 2


using namespace std;

map<long long,set<long long>> g;
map<long long,set<long long>> gr;
vector<char> used;
vector<long long> order, component;

void dfs1 (long long v1) {
	used[v1] = true;
	for (set<long long>::iterator i=g[v1].begin(); i!=g[v1].end(); ++i)
		if (!used[ *i ])
			dfs1 (*i);
	order.push_back (v1);
}

void dfs2 (long long v1) {
	used[v1] = true;
	component.push_back (v1);
	for (set<long long>::iterator i=gr[v1].begin(); i!=gr[v1].end(); ++i)
		if (!used[ *i ])
			dfs2 (*i);
}

int main(int argc, char** argv){
    // vector<vector<float>> matrix;
    // for(int ii=0; ii<=2; ii++){
    //     vector<float> temp;
    //     for(int jj=1; jj<=3; jj++){
    //         temp.push_back(atof(argv[jj+3*ii]));
    //     }
    //     matrix.push_back(temp);
    // }
    double a11 = atof(argv[1]);
    double a12 = atof(argv[2]);
    double a13 = atof(argv[3]);

    double a21 = atof(argv[4]);
    double a22 = atof(argv[5]);
    double a23 = atof(argv[6]);

    double a31 = atof(argv[7]);
    double a32 = atof(argv[8]);
    double a33 = atof(argv[9]);

    int iterations = atof(argv[10]);

    // time_t start=time(NULL);

    double u=double(v);
    long long num_cells=(L/u)*(H/u);
    //cout<<num_cells<<endl;
    long long comps=0;
    map<long long, long long> global_colors; //���������� ������ #���� 2, �� ������ ����������, ���� 0 - ����� �� ����������
    for (int it=1; it<=100; it++) //��������
    {
        //cout<<"===========ITERATION"<<it<<endl;
        comps=0;
        map<long long, set<long long>> trans;
        map<long long, set<long long>> trans_rev;
        map<long long, long long> global_colors_new; //���������� ������ #���� 2, �� ������ ����������, ���� 0 - ����� �� ����������
        for (long long cell=1; cell<=num_cells*3; cell++)//������� ����� ��� ���������� ���� ������
        {
            if (global_colors.count(cell)==1)
            {
                if (global_colors[cell]==0) continue;
            }
            if (cell<=num_cells)
            {
                double yc_0=-1+((cell-1)%int(L/u))*u;
                double zc_0=1-floor((cell-1)/(L/u))*u;
                double du=double(u)/11;
                for (double i=du; i<u; i+=du)
                {
                    for (double j=du; j<u; j+=du)
                    {
                        double x_new=0;
                        double y_new=0;
                        double z_new=0;
                        double x_next=a11+(yc_0+i)*a12+(zc_0-j)*a13;
                        double y_next=a21+(yc_0+i)*a22+(zc_0-j)*a23;
                        double z_next=a31+(yc_0+i)*a32+(zc_0-j)*a33;
                        long long new_cell=0;
                        if ((abs(x_next)>abs(y_next)) and (abs(x_next)>abs(z_next)))
                        {
                            x_new=1;
                            y_new=y_next/x_next;
                            z_new=z_next/x_next;
                            new_cell=L/u*floor((1-z_new)/u)+ceil((y_new+1)/u);
                            trans[cell].insert(new_cell);
                            trans_rev[new_cell].insert(cell);
                            //cout<<cell<<" to "<<new_cell<<endl;
                        }
                        if ((abs(y_next)>abs(x_next)) and (abs(y_next)>abs(z_next)))
                        {
                            x_new=x_next/y_next;
                            y_new=1;
                            z_new=z_next/y_next;
                            new_cell=L/u*floor((1-z_new)/u)+ceil((x_new+1)/u)+num_cells;
                            trans[cell].insert(new_cell);
                            trans_rev[new_cell].insert(cell);
                            //cout<<cell<<" to "<<new_cell<<endl;
                        }
                        if ((abs(z_next)>abs(x_next)) and (abs(z_next)>abs(y_next)))
                        {
                            x_new=x_next/z_next;
                            y_new=y_next/z_next;
                            z_new=1;
                            new_cell=L/u*floor((1-y_new)/u)+ceil((x_new+1)/u)+2*num_cells;
                            trans[cell].insert(new_cell);
                            trans_rev[new_cell].insert(cell);
                            //cout<<cell<<" to "<<new_cell<<endl;
                        }
                    }
                }
            }
            else if (cell<=num_cells*2)
            {
                double xc_0=-1+((cell-num_cells-1)%int(L/u))*u;
                double zc_0=1-floor((cell-num_cells-1)/(L/u))*u;
                double du=double(u)/11;
                for (double i=du; i<u; i+=du)
                {
                    for (double j=du; j<u; j+=du)
                    {
                        double x_new=0;
                        double y_new=0;
                        double z_new=0;
                        double x_next=(xc_0+i)*a11+a12+(zc_0-j)*a13;
                        double y_next=(xc_0+i)*a21+a22+(zc_0-j)*a23;
                        double z_next=(xc_0+i)*a31+a32+(zc_0-j)*a33;
                        long long new_cell=0;
                        if ((abs(x_next)>abs(y_next)) and (abs(x_next)>abs(z_next)))
                        {
                            x_new=1;
                            y_new=y_next/x_next;
                            z_new=z_next/x_next;
                            new_cell=L/u*floor((1-z_new)/u)+ceil((y_new+1)/u);
                            trans[cell].insert(new_cell);
                            trans_rev[new_cell].insert(cell);
                            //cout<<cell<<" to "<<new_cell<<endl;
                        }
                        if ((abs(y_next)>abs(x_next)) and (abs(y_next)>abs(z_next)))
                        {
                            x_new=x_next/y_next;
                            y_new=1;
                            z_new=z_next/y_next;
                            new_cell=L/u*floor((1-z_new)/u)+ceil((x_new+1)/u)+num_cells;
                            trans[cell].insert(new_cell);
                            trans_rev[new_cell].insert(cell);
                            //cout<<cell<<" to "<<new_cell<<endl;
                        }
                        if ((abs(z_next)>abs(x_next)) and (abs(z_next)>abs(y_next)))
                        {
                            x_new=x_next/z_next;
                            y_new=y_next/z_next;
                            z_new=1;
                            new_cell=L/u*floor((1-y_new)/u)+ceil((x_new+1)/u)+2*num_cells;
                            trans[cell].insert(new_cell);
                            trans_rev[new_cell].insert(cell);
                            //cout<<cell<<" to "<<new_cell<<endl;
                        }
                    }
                }
            }
            else
            {
                double xc_0=-1+((cell-num_cells*2-1)%int(L/u))*u;
                double yc_0=1-floor((cell-num_cells*2-1)/(L/u))*u;
                double du=double(u)/11;
                for (double i=du; i<u; i+=du)
                {
                    for (double j=du; j<u; j+=du)
                    {
                        //cout<<"coords "<<xc_0+i<<" "<<yc_0-j<<" "<<1<<endl;
                        double x_new=0;
                        double y_new=0;
                        double z_new=0;
                        double x_next=(xc_0+i)*a11+(yc_0-j)*a12+a13;
                        double y_next=(xc_0+i)*a21+(yc_0-j)*a22+a23;
                        double z_next=(xc_0+i)*a31+(yc_0-j)*a32+a33;
                        long long new_cell=0;
                        if ((abs(x_next)>abs(y_next)) and (abs(x_next)>abs(z_next)))
                        {
                            x_new=1;
                            y_new=y_next/x_next;
                            z_new=z_next/x_next;
                            new_cell=L/u*floor((1-z_new)/u)+ceil((y_new+1)/u);
                            trans[cell].insert(new_cell);
                            trans_rev[new_cell].insert(cell);
                            //<<"new coords "<<x_new<<" "<<y_new<<" "<<z_new<<endl;
                            //cout<<cell<<" to "<<new_cell<<endl;
                        }
                        if ((abs(y_next)>abs(x_next)) and (abs(y_next)>abs(z_next)))
                        {
                            x_new=x_next/y_next;
                            y_new=1;
                            z_new=z_next/y_next;
                            new_cell=L/u*floor((1-z_new)/u)+ceil((x_new+1)/u)+num_cells;
                            trans[cell].insert(new_cell);
                            trans_rev[new_cell].insert(cell);
                            //cout<<"new coords "<<x_new<<" "<<y_new<<" "<<z_new<<endl;
                            //cout<<cell<<" to "<<new_cell<<endl;
                        }
                        if ((abs(z_next)>abs(x_next)) and (abs(z_next)>abs(y_next)))
                        {
                            x_new=x_next/z_next;
                            y_new=y_next/z_next;
                            z_new=1;
                            new_cell=L/u*floor((1-y_new)/u)+ceil((x_new+1)/u)+2*num_cells;
                            trans[cell].insert(new_cell);
                            trans_rev[new_cell].insert(cell);
                            //cout<<"new coords "<<x_new<<" "<<y_new<<" "<<z_new<<endl;
                            //cout<<cell<<" to "<<new_cell<<endl;
                        }
                    }
                }
            }
        }
        // cout<<"Simv obraz +"<<endl;
        /*for (int i=1; i<=num_cells*3; i++)
        {
            cout<<i<<" : ";
            for (set<long long>::iterator iter=trans[i].begin(); iter!=trans[i].end(); iter++)
            {
                cout<<*iter;
                cout<<" ";
            }
            cout<<endl;
        }*/
        g=trans;
        gr=trans_rev;
        long long n=num_cells*3;
        used.assign (n+1, false);
        for (long long i=1; i<=n; ++i)
            if (!used[i])
                dfs1 (i);
        used.assign (n+1, false);
        for (long long i=1; i<=n; ++i) {
            long long v1 = order[n-i];
            if (!used[v1]) {
                dfs2 (v1);
                int flag=0;
                if (component.size()==1)
                {
                    long long cll=component.at(0);
                    for (set<long long>::iterator itc=trans[cll].begin(); itc!=trans[cll].end(); itc++ )
                    {
                        if (*itc==cll)
                        {
                            flag=1;
                            break;
                        }
                    }
                }
                if ((component.size()>1) or flag==1)
                {
                    comps++;
                    for (vector<long long>::iterator itc = component.begin(); itc!=component.end(); itc++)
                    {
                        global_colors[*itc]=2;
                    }
                }
                component.clear();
            }
        }
        // cout<<"comps++"<<endl;
        if (it==iterations) break;
        for (long long i=1; i<=num_cells*3; i++)
        {
            if (global_colors.count(i)==0)
            {
                global_colors_new[int((i-1)/(L/u))*(L/u)*4+2*((i-1)%int(L/u))+1]=0;
                global_colors_new[int((i-1)/(L/u))*(L/u)*4+2*((i-1)%int(L/u))+2]=0;
                global_colors_new[int((i-1)/(L/u))*(L/u)*4+(2*L/u)+2*((i-1)%int(L/u))+1]=0;
                global_colors_new[int((i-1)/(L/u))*(L/u)*4+(2*L/u)+2*((i-1)%int(L/u))+2]=0;
            }
            if (global_colors.count(i)==1)
            {
                if (global_colors[i]==0)
                {
                    global_colors_new[int((i-1)/(L/u))*(L/u)*4+2*((i-1)%int(L/u))+1]=0;
                    global_colors_new[int((i-1)/(L/u))*(L/u)*4+2*((i-1)%int(L/u))+2]=0;
                    global_colors_new[int((i-1)/(L/u))*(L/u)*4+(2*L/u)+2*((i-1)%int(L/u))+1]=0;
                    global_colors_new[int((i-1)/(L/u))*(L/u)*4+(2*L/u)+2*((i-1)%int(L/u))+2]=0;
                    continue;
                }
            }
        }
        //cout<<"gl_colors new ++"<<endl;
        u=u/2;
        global_colors=global_colors_new;
        used.clear();
        order.clear();
        num_cells*=4;
        //cout<<"new it"<<endl;
    }

    Area wa(-1,1,1,-1);
    int n_x=wa.get_hx(u);
    int n_y=wa.get_hy(u);
    
    ofstream points_file((char*)"points_file.osip", ios::trunc);
    ofstream cells_file((char*)"cells_file.osip", ios::trunc);
    for (const auto& [key, value] : global_colors){
        if (value == 2){
            // std::cout << key << "\n";
            int cell_num = key;
            int map_num = ceil((double)cell_num / (double)(n_x*n_y));
            cells_file << cell_num << endl;
            pair<float,float> corner = wa.cell_coord(cell_num - n_x*n_y*(map_num-1), u);
            points_file << map_num << endl << corner.first << endl << corner.second << endl;
        }
    }
    cells_file.close();
    points_file.close();

    return 0;
}
