#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
//#include "Point.h"
#include "Triangle.h"

#define WIDTH 400
#define HEIGHT 400
#define FILENAME "out.ppm"
#define OBJNAME "../obj/cube.obj"

using namespace std;


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
            else if (type == "vt") {
                //texture
            }
            else if (type == "vn") {
                //normale
            }
            else if (type == "f") {
                //triangle
                getline(ss, x, ' ');
                getline(ss, x, ' ');

                getline(ss, y, ' ');
                getline(ss, y, ' ');

                getline(ss, z, ' ');
                getline(ss, z, ' ');

                cout << x << " " << y << " " << z << endl;
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

void writePPM() {
    ofstream ofs; // save the framebuffer to file
    ofs.open(FILENAME, ios::binary);
    ofs << "P6\n" << WIDTH << " " <<  HEIGHT << "\n255\n";
    for (size_t i = 0; i < HEIGHT * WIDTH; ++i) {
        for (size_t j = 0; j < 3; j++) {
            ofs << (char)(50*j);
        }
    }
    ofs.close();
}

int main(){
    writePPM();
    vector<Triangle> triangle;
    readOBJ(triangle);

    cout << "nb triangles : " << triangle.size() << endl;
	return 0;
}
