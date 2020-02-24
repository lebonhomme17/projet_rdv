#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <cassert>
//#include "Point.h"
#include "Triangle.h"

#define WIDTH 400
#define HEIGHT 400
#define FILENAME "out.ppm"
#define OBJNAME "../obj/cube.obj"
#define DISTANCE 4
#define DISTANCE_F 1


using namespace std;

template <size_t DIM, typename T> struct vec {
    vec() { for (size_t i=DIM; i--; data_[i] = T()); }
    T& operator[](const size_t i)       { assert(i<DIM); return data_[i]; }
    const T& operator[](const size_t i) const { assert(i<DIM); return data_[i]; }
private:
    T data_[DIM];
};

typedef vec<3, float> Vec3f;

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
                getline(ss, x, ' ');
                
                getline(ss, y, ' ');
                getline(ss, y, ' ');

                getline(ss, z, ' ');
                getline(ss, z, ' ');
                Point p = Point(stod(x), stod(y), stod(z));
                points.push_back(p);
                //cout << "x=" << x << " y=" << y << " z=" << z;
            }
            else if (type == "f") {
                //triangle
                getline(ss, x, ' ');
                getline(ss, x, ' ');

                getline(ss, y, ' ');
                getline(ss, y, ' ');

                getline(ss, z, ' ');
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
    double x1, x2, x3, y1, y2, y3;
    x1= WIDTH*t.getX().getX()*DISTANCE_F/(t.getX().getZ()+DISTANCE);
    x2= WIDTH*t.getY().getX()*DISTANCE_F/(t.getX().getZ()+DISTANCE);
    x3= WIDTH*t.getZ().getX()*DISTANCE_F/(t.getX().getZ()+DISTANCE);
    y1= HEIGHT*t.getX().getY()*DISTANCE_F/(t.getX().getZ()+DISTANCE);
    y2= HEIGHT*t.getY().getY()*DISTANCE_F/(t.getX().getZ()+DISTANCE);
    y3= HEIGHT*t.getZ().getY()*DISTANCE_F/(t.getX().getZ()+DISTANCE);

    map[(int)(x1*WIDTH+y1)][0]=1.;
    map[(int)(x2*WIDTH+y2)][0]=1.;
    map[(int)(x3*WIDTH+y3)][0]=1.;
}


void writePPM(vector<Triangle> &triangles) {
    ofstream ofs; // save the framebuffer to file
    ofs.open(FILENAME, ios::binary);
    ofs << "P6\n" << WIDTH << " " <<  HEIGHT << "\n255\n";
    Vec3f map[HEIGHT*WIDTH];
    for (size_t i = 0; i < HEIGHT * WIDTH; ++i) {
        for (size_t j = 0; j < 3; j++) {
            map[i][j]=0;
        }
    }
    for(Triangle t : triangles){
        drawTriangle(t, map);
    }
    for (size_t i = 0; i < HEIGHT * WIDTH; ++i) {
        for (size_t j = 0; j < 3; j++) {
            ofs << (char)(map[i][j]*255);
        }
    }
    ofs.close();
}

int main(){
    vector<Triangle> triangle;
    readOBJ(triangle);

    cout << "nb triangles : " << triangle.size() << endl;

    writePPM(triangle);
	return 0;
}
