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
#include "Geom.h"

#define WIDTH 1500
#define HEIGHT 800
#define ZOOM 300
#define FILENAME "../out.ppm"
#define FILENAME_STEREO "../outstereo.ppm"
#define OBJNAME "../obj/africanhead.obj"
#define DISTANCE 10
#define DISTANCE_F 5
#define FOCALE 5


using namespace std;

Point lightdir(0.f,0.f,1.f);

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

                Point p = Point(stod(x), stod(y), -stod(z));
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
        float meanX=0;
        float meanY=0;
        float meanZ=0;
        for(Point p : points){
            meanX+=p.getX();
            meanY+=p.getY();
            meanZ+=p.getZ();
        }
        meanX/=points.size();
        meanY/=points.size();
        meanZ/=points.size();

        cout << meanX << " : " << meanY << " : " << meanZ << endl;
        for(Triangle t : triangles){
            cout << "avant : " << t.getA().getX() << endl;
            t.translate(-meanX,-meanY,-meanZ);
            cout << "apres : " << t.getA().getX() << endl;
        }
    }
    else {
        cout << "echec ouverture fichier obj";
    }

    cout << "nb points : " << points.size() << endl;

    flux.close();
}

float zVal(Triangle t, int x, int y){
    return (t.getA().getZ()+t.getB().getZ()+t.getC().getZ())/3.f;

}

Point norm(Triangle t){
    float x = (t.getB().getY()-t.getA().getY())*(t.getC().getZ()-t.getA().getZ()) - (t.getB().getZ()-t.getA().getZ())*(t.getC().getY()-t.getA().getY());
    float y = (t.getB().getZ()-t.getA().getZ())*(t.getC().getX()-t.getA().getX()) - (t.getB().getX()-t.getA().getX())*(t.getC().getZ()-t.getA().getZ());
    float z = (t.getB().getX()-t.getA().getX())*(t.getC().getY()-t.getA().getY()) - (t.getB().getY()-t.getA().getY())*(t.getC().getX()-t.getA().getX());
    float norm = sqrt(x*x+y*y+z*z);
    return Point(x/norm, y/norm, z/norm);
}

float crossp(Point p1, Point p2){
    return p1.getX()*p2.getX()+p1.getY()*p2.getY()+p1.getZ()*p2.getZ();
}


void drawTriangle(Triangle t, Vec3f* map, float* z_buffer){
    int x1, x2, x3, y1, y2, y3;

    float intensite = crossp(lightdir, norm(t));
    cout << intensite << endl;
    if(intensite<0){
        intensite = -intensite;
    }
    //intensite=1;

    x1= round(ZOOM*t.getA().getX()*DISTANCE_F/(t.getA().getZ()+DISTANCE))+WIDTH/2;
    x2= round(ZOOM*t.getB().getX()*DISTANCE_F/(t.getB().getZ()+DISTANCE))+WIDTH/2;
    x3= round(ZOOM*t.getC().getX()*DISTANCE_F/(t.getC().getZ()+DISTANCE))+WIDTH/2;
    y1= round(ZOOM*t.getA().getY()*DISTANCE_F/(t.getA().getZ()+DISTANCE))+HEIGHT/2;
    y2= round(ZOOM*t.getB().getY()*DISTANCE_F/(t.getB().getZ()+DISTANCE))+HEIGHT/2;
    y3= round(ZOOM*t.getC().getY()*DISTANCE_F/(t.getC().getZ()+DISTANCE))+HEIGHT/2;

    Triangle temp = Triangle(Point(x1, y1, t.getA().getZ()), Point(x2, y2, t.getB().getZ()), Point(x3, y3, t.getC().getZ()));
    float z;

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
                z=zVal(temp, j, y);
                if(z<z_buffer[y * WIDTH + j]) {
                    z_buffer[y * WIDTH + j]=z;
                    map[y * WIDTH + j][0] = intensite;
                    map[y * WIDTH + j][1] = intensite;
                    map[y * WIDTH + j][2] = intensite;
                }
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
                z=zVal(temp, j, y);
                if(z<z_buffer[y * WIDTH + j]) {
                    z_buffer[y * WIDTH + j]=z;
                    map[y * WIDTH + j][0] = intensite;
                    map[y * WIDTH + j][1] = intensite;
                    map[y * WIDTH + j][2] = intensite;
                }
            }
        }
    }
}


void writePPM(Vec3f *map, string file){
    ofstream ofs;
    ofs.open(file, ios::binary);

    ofs << "P6\n" << WIDTH << " " <<  HEIGHT << "\n255\n";

    for (size_t i = HEIGHT*WIDTH; i >0; --i) {
        for (size_t j = 0; j < 3; j++) {
            ofs << (char)(map[i][j]*255);
        }
    }

    ofs.close();

}

void stereo_rendering(Vec3f *map){
    Vec3f *stereomap= (Vec3f*)malloc(WIDTH*HEIGHT*sizeof(Vec3f));

    for (size_t i = 0; i < HEIGHT * WIDTH; ++i) {
        if((i%WIDTH)>FOCALE && (WIDTH-(i%WIDTH))>FOCALE) {
            stereomap[i][0] = map[i + FOCALE][0];
            stereomap[i][1] = 0;
            stereomap[i][2] = map[i - FOCALE][2];
        }else{
            stereomap[i]=map[i];
        }
    }

    writePPM(stereomap, FILENAME_STEREO);
}

int main(){
    srand(time(NULL));
    vector<Triangle> triangle;
    readOBJ(triangle);

    cout << "nb triangles : " << triangle.size() << endl;

    Vec3f *map= (Vec3f*)malloc(WIDTH*HEIGHT*sizeof(Vec3f));
    for (size_t i = 0; i < HEIGHT * WIDTH; ++i) {
        for (size_t j = 0; j < 3; j++) {
            map[i][j]=0;
        }
    }

    float *zbuffer = new float[WIDTH*HEIGHT];
    for(int i = WIDTH*HEIGHT; i >= 0; i--){
        zbuffer[i] = numeric_limits<float>::max();
    }

    for(Triangle t : triangle){
        drawTriangle(t, map, zbuffer);
    }

    writePPM(map, FILENAME);

    stereo_rendering(map);

    free(map);
	return 0;
}
