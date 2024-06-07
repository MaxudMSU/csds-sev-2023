#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <math.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Area{
public:
    float x_left, y_top, x_right, y_down;
    Area (float a, float b, float c, float d): x_left(a),y_top(b),x_right(c),y_down(d){}
    int get_hx(float h){return (x_right-x_left)/h;}
    int get_hy(float h){return (y_top-y_down)/h;}
    float get_xleft(){return x_left;}
    float get_ytop(){return y_top;}
    float get_xright(){return x_right;}
    float get_ydown(){return y_down;}
    pair<float,float> cell_coord(int num, float h);
    // void set_area(float h);
};

//уравнение Дуффинга
float duff_eq(float t, float x, float y, float alpha, float beta, float omega, float k, float b){
    return -alpha*x - beta*pow(x,3)- k*y + b*cos(omega*t);
}

class Point{
    
public:
    float x,y;
    Point(){}
    Point(float xx,float yy){x=xx; y=yy;}
    Point(const Point &point){x=point.x; y=point.y;}
    float getx() {return x;}
    float gety() {return y;}
    
    //метод для определения номера ячейки, в которой находится точка
    //область задаётся типом Area
    int cell(Area& a, int n, float h){
        if(x<a.x_left || x>a.x_right || y>a.y_top || y<a.y_down) return 0;
        int nomer = floor((fabs(a.y_top-y))/h)*n + (ceil ((fabs(a.x_left-x))/h));
        if (x==a.x_left) nomer++;
        if (y==a.y_down) nomer-=n;
        return nomer;
    }
    
    //метод для определения номера ячейки, в которой находится точка
    //область задаётся координатами углов
    int cell2(float x_left, float y_top, float x_right, float y_down, int n, float h){
        if(x<x_left || x>x_right || y>y_top || y<y_down) return 0;
        int nomer = floor((fabs(y_top-y))/h)*n + (ceil ((fabs(x_left-x))/h));
        if (x==x_left) nomer++;
        if (y==y_down) nomer-=n;
        return nomer;
    }
    
    //отображение Жюлиа
    //изменяется исходная точка
    Point& julia(float a,float b){
        float xj=pow(x,2)-pow(y,2)+a;
        float yj=2*x*y+b;
        x=xj;
        y=yj;
        return *this;
    }
    //отображение Жюлиа
    //строится новая точка
    Point julia1(float a, float b){
        float xj=pow(x,2)-pow(y,2)+a;
        float yj=2*x*y+b;
        return Point(xj,yj); 
    }
    //отображение Хенона
    //изменяется исходная точка
    Point& henon(float a,float b){
        float xh=1+y-a*pow(x,2);
        float yh=b*x;
        x=xh;
        y=yh;
        return *this;
    }
    //отображение Хенона
    //строится новая точка
    Point henon1(float a, float b){
        float xh=1+y-a*pow(x,2);
        float yh=b*x;
        return Point(xh, yh);
    }

    Point homoMap(float a){
        float xhm=x+y+a*x*(1-x);
        float yhm=y+a*x*(1-x);
        return Point(xhm,yhm);
    }
    Point homoMapReverse(float a){
        float xhmr = x - y;
        float yhmr = y - a*(x-y)*(1-x+y);
        return Point(xhmr, yhmr);
    }
    
    //отображение, созданное из уравнения Дуффинга
    //методом Рунге-Кутта
    Point duffing_rk(float alpha, float beta, float k, float b, float omega){
        float period = (2*M_PI)/omega;
        float shag = 0.2;
        float k1,k2,k3,k4,m1,m2,m3,m4, xrez, yrez;
        float t = 0;
        float x1 = x;
        float y1 = y;
        while (t <= period){
            k1 = y;
            m1 = duff_eq(t, x1, y1, alpha, beta, omega, k, b);
            k2 = y + (shag*m1)/2;
            m2 = duff_eq(t + shag/2, x + (shag*k1)/2, y + (shag*m1)/2, alpha, beta, omega, k, b);
            k3 = y + (shag*m2)/2;
            m3 = duff_eq(t + shag/2, x + (shag*k2)/2, y + (shag*m2)/2, alpha, beta, omega, k, b);
            k4 = y + shag*m3;
            m4 = duff_eq(t + shag, x + shag*k3, y + shag*m3, alpha, beta, omega, k, b);
            t = t + shag;
            x = x + (shag/6)*(k1+2*k2+2*k3+k4);
            y = y +(shag/6)*(m1+2*m2+2*m3+m4);
        }
        
        return Point(x1, y1);
    }
    
    //отображение, созданное из уравнения Дуффинга
    //методом Эйлера
    Point duffing_e(float alpha, float beta, float k, float b, float omega){
        float period = (2*M_PI)/omega;
        float shag = 0.2;
        float t = 0;
        float x1 = x;
        float y1 = y;

        while (t <= period){
            y1 = y1 + shag*duff_eq(t, x1, y1, alpha, beta, omega, k, b);
            x1 = x1 + shag*y1;
            t = t + shag;
        }
        
        return Point(x1, y1);
    }
    

    bool isClose(Point p, float r){
        return ((pow(p.x-x,2)+(pow(p.y-y,2))) <= pow(r,2));
    }

    void print(){
        cout << x << ", " << y << endl;;
    }

    void draw(int color);
};


float distance(Point a, Point b){
    return sqrt(pow(a.x - b.x,2) + pow(a.y - b.y,2));
}

Point mid(Point a, Point b){
    return Point((a.x+b.x)/2, (a.y+b.y)/2);
}


float angle(Point s, Point p1, Point p2){
    float sc_prod = (p1.x - s.x)*(p2.x - s.x) + (p1.y - s.y)*(p2.y - s.y);
    return acos(sc_prod/(distance(s, p1)*distance(s,p2)));
}

class ProjectivePoint{
    // float t1, t2;
    Point point;
    int map;
public:
    ProjectivePoint() {}
    ProjectivePoint(float x, float y, int num){point = Point(x,y); map = num;}
    // ProjectivePoint(Point3D p);
    Point get_point(){
        return point;
    }
    int get_map(){
        return map;
    }
    
    // void print(){
    //     cout << t1 << " " << t2 << " " << map << endl;
    // }
};

class Point3D{
    
public:
    float x, y, z;
    Point3D(){}
    Point3D(float xx,float yy, float zz){x=xx; y=yy; z=zz;}
    Point3D(vector<float> vec){
        x = vec[0];
        y = vec[1];
        z = vec[2];
    }
    // Point3D(ProjectivePoint point);
    
    vector<float> toVector(){
        return {x, y, z};
    }
    vector<float> matr_map(vector<vector<float>> matr){
        vector<float> vec = this->toVector();
        vector<float> res;
        for (int i=0; i<matr.size(); i++){
            float tmp = 0;
            for (int j=0; j<matr[i].size(); j++){
                tmp += matr[i][j]*vec[j];
            }
            res.push_back(tmp);
        }
        return res;
    }
    vector<float> toAbsVector(){
        return {abs(x), abs(y), abs(z)};
    }
    pair<Point3D, int> projectization(){
        vector<float> abs_coords = this->toAbsVector();
        vector<float> coords = this->toVector();
        auto max = max_element(abs_coords.begin(), abs_coords.end());
        auto index = distance(abs_coords.begin(), max);
        float real_max = coords[index];
        return make_pair(Point3D(x / real_max, y / real_max, z / real_max), index + 1);
    }
};

Point3D fromProj(ProjectivePoint prpoint){
    Point coords = prpoint.get_point();
    switch (prpoint.get_map()){
        case 1:
            // x = 1;
            // y = coords.x;
            // z = coords.y;
            return Point3D(1, coords.x, coords.y);
            // break;
        case 2:
            // x = coords.x;
            // y = 1;
            // z = coords.y;
            return Point3D(coords.x, 1, coords.y);
            // break;
        case 3:
            // x = coords.x;
            // y = coords.y;
            // z = 1;
            return Point3D(coords.x, coords.y, 1);
            // break;
        default:
            return Point3D();
    }
}

ProjectivePoint from3d(Point3D p3d){

// ProjectivePoint::ProjectivePoint(Point3D p){
    auto pp = p3d.projectization();
    Point3D dec_point = pp.first;
    int num = pp.second;
    // map = num;
    switch(num){
        case 1:
            // t1 = dec_point.y;
            // t2 = dec_point.z;
            // point = Point(dec_point.y, dec_point.z);
            // break;
            return ProjectivePoint(dec_point.y, dec_point.z, num);
        case 2:
            // t1 = dec_point.x;
            // t2 = dec_point.z;
            // point = Point(dec_point.z, dec_point.z);
            // break;
            return ProjectivePoint(dec_point.x, dec_point.z, num);
        case 3:
            // t1 = dec_point.x;
            // t2 = dec_point.y;
            // point = Point(dec_point.x, dec_point.y);
            // break;
            return ProjectivePoint(dec_point.x, dec_point.y, num);
        default:
            return ProjectivePoint();
    }
}


vector<Point> pointsFromFile(char* filename){
    ifstream inputFile(filename);
    vector<Point> numberPoints;
    float number1, number2;
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

vector<int> vozvratFromFile(char* filename, int n=1){
    ifstream inputFile(filename);
    vector<int> vozvrat_cells;
    float  num;
    int counter = 1;
    while (inputFile >> num){
        if (counter == n){
            int cel = (int)num;
            vozvrat_cells.push_back(cel);
            counter = 1;
        }
        else counter++;
    }
    inputFile.close();
    return vozvrat_cells;
}

pair<float,float> Area::cell_coord(int num, float h){
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
    float xl=x_left+h*otstup_x;
    float yt=y_top-h*otstup_y;
    // // return Point(xl, yt);
    return make_pair(xl, yt);
}

Point pairToPoint(pair<float,float> p){
    return Point(p.first, p.second);
}

vector<Point> unifCellPoints(Point corner, float h){
    vector<Point> res;
    float x_left = corner.x, y_top = corner.y + h;
    float s = h/5;

    // cout << corner.y+s << " " << y_top-s << " " << s << endl;
    // cout << x_left+s << " " << x_left+h-s << " " << endl;
    for (float i=corner.y+s; i<y_top; i+=s){
        for (float j=x_left+s; j<x_left+h; j+=s){
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
    TwoDimArray(Area& a, float h){
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
    void division(Area& a, float h){
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
    void zero(int num, Area& a, float h){
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
