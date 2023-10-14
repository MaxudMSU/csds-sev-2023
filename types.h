#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <math.h>
#include <fstream>
#include <vector>

using namespace std;

class Area{
public:
    double x_left, y_top, x_right, y_down;
    Area (double a, double b, double c, double d): x_left(a),y_top(b),x_right(c),y_down(d){}
    int get_hx(double h){return (x_right-x_left)/h;}
    int get_hy(double h){return (y_top-y_down)/h;}
    double get_xleft(){return x_left;}
    double get_ytop(){return y_top;}
    double get_xright(){return x_right;}
    double get_ydown(){return y_down;}
    pair<double,double> cell_coord(int num, double h);
    // void set_area(double h);
};

double duff_eq(double t, double x, double y, double omega, double delta, double g){
    return x-pow(x,3)-delta*y+g*cos(omega*t);
}

class Point{
    
public:
    double x,y;
    Point(){}
    Point(float xx,float yy){x=xx; y=yy;}
    double getx() {return x;}
    double gety() {return y;}
    //метод для определения номера ячейки, в которой находится точка
    int cell(Area& a, int n, double h){
        if(x<a.x_left || x>a.x_right || y>a.y_top || y<a.y_down) return 0;
        int nomer = floor((fabs(a.y_top-y))/h)*n + (ceil ((fabs(a.x_left-x))/h));
        if (x==a.x_left) nomer++;
        if (y==a.y_down) nomer-=n;
        return nomer;
    }

    int cell2(double x_left, double y_top, double x_right, double y_down, int n, double h){
        if(x<x_left || x>x_right || y>y_top || y<y_down) return 0;
        int nomer = floor((fabs(y_top-y))/h)*n + (ceil ((fabs(x_left-x))/h));
        if (x==x_left) nomer++;
        if (y==y_down) nomer-=n;
        return nomer;
    }
    Point& julia(double a,double b){
        double xj=pow(x,2)-pow(y,2)+a;
        double yj=2*x*y+b;
        x=xj;
        y=yj;
        return *this;
    }
    Point julia1(double a, double b){
        double xj=pow(x,2)-pow(y,2)+a;
        double yj=2*x*y+b;
        return Point(xj,yj); 
    }
    Point& henon(double a,double b){
        double xh=1+y-a*pow(x,2);
        double yh=b*x;
        x=xh;
        y=yh;
        return *this;
    }
    Point henon1(double a, double b){
        double xh=1+y-a*pow(x,2);
        double yh=b*x;
        return Point(xh, yh);
    }

    Point homoMap(double a){
        double xhm=x+y+a*x*(1-x);
        double yhm=y+a*x*(1-x);
        return Point(xhm,yhm);
    }
    Point homoMapReverse(double a){
        double xhmr = x - y;
        double yhmr = y - a*(x-y)*(1-x+y);
        return Point(xhmr, yhmr);
    }
    
    

    Point duffing(double omega, double delta, double g){
        double period = (2*M_PI)/omega;
        double h = 0.01;
        int n = period/h;
        double k1,k2,k3,k4,m1,m2,m3,m4, xrez, yrez;
        for (int i=0; i<=n; i++){
            k1 = y;
            m1 = duff_eq(period, x, y, omega, delta, g);
            k2 = y + (h*m1)/2;
            m2 = duff_eq(period + h/2, x + (h*k1)/2, y + (h*m1)/2, omega, delta, g);
            k3 = y + (h*m2)/2;
            m3 = duff_eq(period + h/2, x + (h*k2)/2, y + (h*m2)/2, omega, delta, g);
            k4 = y + h*m3;
            m4 = duff_eq(period + h, x + h*k3, y + h*m3, omega, delta, g);
            x = x + (h/6)*(k1+2*k2+2*k3+k4);
            y = y +(h/6)*(m1+2*m2+2*m3+m4);
        }
        
        return Point(x, y);
    }

    bool isClose(Point p, double r){
        return ((pow(p.x-x,2)+(pow(p.y-y,2))) <= pow(r,2));
    }

    void print(){
        cout << x << ", " << y << endl;;
    }

    // void fprint(std::ofstream fout){
    //     fout << x << ", " << y << std::endl;
    // }

    void draw(int color);
};



double distance(Point a, Point b){
    return sqrt(pow(a.x - b.x,2) + pow(a.y - b.y,2));
}

Point mid(Point a, Point b){
    return Point((a.x+b.x)/2, (a.y+b.y)/2);
}


double angle(Point s, Point p1, Point p2){
    double sc_prod = (p1.x - s.x)*(p2.x - s.x) + (p1.y - s.y)*(p2.y - s.y);
    return acos(sc_prod/(distance(s, p1)*distance(s,p2)));
}

vector<Point> pointsFromFile(char* filename){
    ifstream inputFile(filename);
    vector<Point> numberPoints;
    double number1, number2;
    bool hasNumber1 = false;

    while (inputFile >> number2) {
        if (hasNumber1) {
            numberPoints.push_back(Point(number1, number2));
            hasNumber1 = false;
        }
        else{
            number1 = number2;
            hasNumber1 = true;
        }
    }
    inputFile.close();
    return numberPoints;
}

vector<int> vozvratFromFile(char* filename){
    ifstream inputFile(filename);
    vector<int> vozvrat_cells;
    double num;
    int counter = 1;
    // bool isFirst = 1;
    while (inputFile >> num){
        // if (isFirst) {
        //     continue;
        //     isFirst = 0;
        // }
        // cout << num << endl;
        if (counter == 3){
            int cel = (int)num;
            // cout << num << endl;
            vozvrat_cells.push_back(cel);
            // cout << "third" << endl;
            counter = 1;
        }
        else counter++;
    }
    inputFile.close();
    return vozvrat_cells;
}

// Point cell_coord(Area& a, double h, int cellnum){
//     int cell_x = cellnum / 
// }

pair<double,double> Area::cell_coord(int num, double h){
    int otstup_x, otstup_y;
    int hx=get_hx(h);
    int hy=get_hy(h);
    if(num%hx==0){
        otstup_x=hx-1;
        otstup_y=num/hx;
    }
    else{
        otstup_x=(num % hx)-1;
        otstup_y = (num/hx)+1;
    }
    // int hx=get_hx(h);
    // int otstup_x, otstup_y;
    // if(num%hx==0){
    //     otstup_x=hx-1;
    //     otstup_y=(num/hx)-1;
    // }
    // else{
    //     otstup_x=(num % hx)-1;
    //     otstup_y=floor(num/hx);
    // }
    double xl=x_left+h*otstup_x;
    double yt=y_top-h*otstup_y;
    // // return Point(xl, yt);
    return make_pair(xl, yt);
}

Point pairToPoint(pair<double,double> p){
    return Point(p.first, p.second);
}

vector<Point> unifCellPoints(Point corner, double h){
    vector<Point> res;
    double x_left = corner.x, y_top = corner.y + h;
    double s = h/5;

    // cout << corner.y+s << " " << y_top-s << " " << s << endl;
    // cout << x_left+s << " " << x_left+h-s << " " << endl;
    for (double i=corner.y+s; i<y_top; i+=s){
        for (double j=x_left+s; j<x_left+h; j+=s){
            // cout << i << ", " << j << " ";
            res.push_back(Point(j, i));
        }
        // cout << i << endl;
        // cout << endl;
    }
    return res;
}

template <typename T>
class TwoDimArray{
public:
    T** M; int m, n; 
    TwoDimArray(){
        n = m = 0; M = nullptr;
    }
    TwoDimArray(int _n){
        m = _n;
        n = _n;
        M = (T**) new T*[m];

        for (int i = 0; i < m; i++) 
            M[i] = (T*)new T[n];

        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                M[i][j] = 0;
    }
    TwoDimArray(Area& a, double h){
        m = a.get_hy(h);
        n = a.get_hx(h);
        M = (T**) new T*[m];

        for (int i = 0; i < m; i++)
            M[i] = (T*)new T[n];
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                M[i][j] = 1;
    }
    TwoDimArray(const TwoDimArray& _M){
        m = _M.m;
        n = _M.n;
        M = (T**) new T*[m];

        for (int i = 0; i < m; i++)
            M[i] = (T*) new T[n];

        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                M[i][j] = _M.M[i][j];
    }
    TwoDimArray operator=(const TwoDimArray& _M){
        if (n > 0){
            for (int i = 0; i < m; i++)
                delete[] M[i];
        }

        if (m > 0) delete[] M;
    
        m = _M.m;
        n = _M.n;
        M = (T**) new T*[m];
        for (int i = 0; i < m; i++)
            M[i] = (T*) new T[n];
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                M[i][j] = _M.M[i][j];
        return *this;
    }
    ~TwoDimArray(){
        if (n > 0){
            for (int i = 0; i < m; i++)
                delete[] M[i];
        }

        if (m > 0) delete[] M;
    }
    void division(Area& a, double h){
        int ny=0, nx;
        TwoDimArray<int> t(a,h/2);
        for (int i=0; i<m;i++){
            nx=0;
            for(int j=0;j<n;j++){
                t.M[i+ny][j+nx]=M[i][j];
                t.M[i+ny][j+1+nx]=M[i][j];
                t.M[i+1+ny][j+nx]=M[i][j];
                t.M[i+1+ny][j+1+nx]=M[i][j];
                nx++;
            }
            ny++;
        }
        *this=t;
    }
    void zero(int num, Area& a, double h){
        int hx=a.get_hx(h);
        int otstup_x, otstup_y;
        if(num%hx==0){
            otstup_x=hx-1;
            otstup_y=(num/hx)-1;
        }
        else{
            otstup_x=(num % hx)-1;
            otstup_y=floor(num/hx);
        }
        // cout << otstup_x << " " << otstup_y << endl;
        M[otstup_y][otstup_x]=0;
    }
};



int ones_amount(TwoDimArray<int> a){
    int temp=0;
    for (int i=0; i<a.m; i++)
        for (int j=0; j<a.n; j++)
            if (a.M[i][j]) temp++;
    return temp;
}

#endif // TYPES_H_INCLUDED
