#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <ctime>
//#include "Point.h"
#include "Triangle.h"

#define WIDTH 1000
#define HEIGHT 800
#define ZOOM 1000
#define FILENAME "../out.ppm"
#define OBJNAME "../obj/duck.obj"
#define DISTANCE 30
#define DISTANCE_F 2


using namespace std;

template <size_t DIM, typename T> struct vec {
    vec() { for (size_t i=DIM; i--; data_[i] = T()); }
    T& operator[](const size_t i)       { assert(i<DIM); return data_[i]; }
    const T& operator[](const size_t i) const { assert(i<DIM); return data_[i]; }
private:
    T data_[DIM];
};

typedef vec<3, float> Vec3f;
typedef vec<2, int> Vec2i;

bool comp(Triangle t1, Triangle t2){
    return t1.meanZ()>t2.meanZ();
}

void readOBJ(vector<Triangle> &triangles) {
    ifstream flux(OBJNAME, ios::in);
    vector<Point> points;
    string buf;
    stringstream ss;
    string type, x, y, z;

    if (flux) {
        while (getline(flux, buf)) {
            ss = stringstream(buf);
            getline(ss, type, ' ');
            if (type == "v") {
                //point
                getline(ss, x, ' ');
                
                getline(ss, y, ' ');

                getline(ss, z, ' ');

                Point p = Point(stod(x), stod(y)+5, stod(z));
                points.push_back(p);
                //cout << "x=" << x << " y=" << y << " z=" << z;
            }
            else if (type == "f") {
                //triangle
                getline(ss, x, ' ');

                getline(ss, y, ' ');

                getline(ss, z, ' ');


                //cout << x << " " << y << " " << z << endl;
                Triangle t = Triangle(points.at(stoi(x)-1), points.at(stoi(y)-1), points.at(stoi(z)-1));
                triangles.push_back(t);
            }
        }
    }
    else {
        cout << "echec ouverture fichier obj";
    }

    cout << "nb points : " << points.size() << endl;

    flux.close();
}




void drawTriangle(Triangle t, Vec3f* map){
    int x1, x2, x3, y1, y2, y3;

    float c = (float)(rand()%100+155)/255.;

    x1= round(ZOOM*t.getA().getX()*DISTANCE_F/(t.getA().getZ()+DISTANCE));
    x2= round(ZOOM*t.getB().getX()*DISTANCE_F/(t.getB().getZ()+DISTANCE));
    x3= round(ZOOM*t.getC().getX()*DISTANCE_F/(t.getC().getZ()+DISTANCE));
    y1= round(ZOOM*t.getA().getY()*DISTANCE_F/(t.getA().getZ()+DISTANCE));
    y2= round(ZOOM*t.getB().getY()*DISTANCE_F/(t.getB().getZ()+DISTANCE));
    y3= round(ZOOM*t.getC().getY()*DISTANCE_F/(t.getC().getZ()+DISTANCE));

    if(y1>y2){
        swap(x1, x2);
        swap(y1, y2);
    }
    if(y1>y3){
        swap(x1, x3);
        swap(y1, y3);
    }
    if(y2>y3) {
        swap(x2, x3);
        swap(y2, y3);
    }

    int total_height = y3-y1+1;
    for (int y=y1; y<=y2; y++) {
        int segment_height = y2-y1+1;
        float alpha = (float)(y-y1)/total_height;
        float beta  = (float)(y-y1)/segment_height;
        Vec2i A;
        A[0] = x1 + (x3-x1)*alpha;
        A[1] = y1 + (y3-y1)*alpha;
        Vec2i B;
        B[0] = x1 + (x2-x1)*beta;
        B[1] = y1 + (y2-y1)*beta;
        if (A[0]>B[0]) std::swap(A, B);
        for (int j=A[0]; j<=B[0]; j++) {
            if(j<WIDTH && j>=0 && y<HEIGHT && y>=0) {
                map[y * WIDTH + j][0] = c;
                map[y * WIDTH + j][1] = c;
                map[y * WIDTH + j][2] = c;
            }
        }
    }
    for (int y=y2; y<=y3; y++) {
        int segment_height =  y3-y2+1;
        float alpha = (float)(y-y1)/total_height;
        float beta  = (float)(y-y2)/segment_height;
        Vec2i A;
        A[0] = x1 + (x3-x1)*alpha;
        A[1] = y1 + (y3-y1)*alpha;
        Vec2i B;
        B[0]= x2 + (x3-x2)*beta;
        B[1]= y2 + (y3-y2)*beta;
        if (A[0]>B[0]) std::swap(A, B);
        for (int j=A[0]; j<=B[0]; j++) {
            if(j<WIDTH && j>=0 && y<HEIGHT && y>=0) {
                map[y * WIDTH + j][0] = c;
                map[y * WIDTH + j][1] = c;
                map[y * WIDTH + j][2] = c;
            }
        }
    }
}


void writePPM(vector<Triangle> &triangles) {
    ofstream ofs;
    ofs.open(FILENAME, ios::binary);

    ofs << "P6\n" << WIDTH << " " <<  HEIGHT << "\n255\n";
    Vec3f * map= (Vec3f*)malloc(WIDTH*HEIGHT*sizeof(Vec3f));
    cout << "ok";
    for (size_t i = 0; i < HEIGHT * WIDTH; ++i) {
        for (size_t j = 0; j < 3; j++) {
            map[i][j]=0;
        }
    }
    sort(triangles.begin(), triangles.end(), comp);

    for(Triangle t : triangles){
        drawTriangle(t, map);
    }

    for (size_t i = HEIGHT*WIDTH; i >0; --i) {
        for (size_t j = 0; j < 3; j++) {
            ofs << (char)(map[i][j]*255);
        }
    }
    free(map);
    ofs.close();

}

int main(){
    srand(time(NULL));
    vector<Triangle> triangle;
    readOBJ(triangle);

    cout << "nb triangles : " << triangle.size() << endl;

    float *zbuffer = new float[WIDTH*HEIGHT];
    for(int i = WIDTH*HEIGHT; i >= 0; i--){
        zbuffer[i] = numeric_limits<float>::max();
    }

    writePPM(triangle);
	return 0;
}
