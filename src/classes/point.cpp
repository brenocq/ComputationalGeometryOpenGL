#include <GL/glut.h>
#include <math.h>
#include "point.h"
#include <iostream>
#include <iomanip>

using namespace std;

Point::Point(){
    ID = 0;
    cord = {0,0,0,0};
    radius = 0.02;
    color = {0,0,0};
}

Point::Point(float _x, float _y, float _z, float _w){
    ID = 0;
    cord = {_x,_y,_z,_w};
    radius = 0.02;
    color = {0,0,0};
}

Point::~Point(){}

void Point::draw2D(){
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i+=5) {
        glVertex2d( radius*cos(i/180.0*M_PI)+cord[0], radius*sin(i/180.0*M_PI)+cord[1]);
    }
    glEnd();
}

void Point::print(int dimension){
  cout<<setprecision(2)<<"(";
  for(int i=0;i<dimension;i++){
    cout<<cord[i];
    if(i<dimension-1)
      cout<<",";
  }

  cout<<")";
}

Point Point::operator-(Point p){
    float x = cord[0]-p.x();
    float y = cord[1]-p.y();
    float z = cord[2]-p.z();
    float w = cord[3]-p.w();
    return Point(x,y,z,w);
}

//----- Setters -----//
void Point::setPoint(float _x, float _y, float _z, float _w){
   cord[0] = _x;
   cord[1] = _y;
   cord[2] = _z;
   cord[3] = _w;
}

void Point::setX(float _x){cord[0]=_x;}
void Point::setY(float _y){cord[1]=_y;}
void Point::setZ(float _z){cord[2]=_z;}
void Point::setW(float _w){cord[3]=_w;}
void Point::setColor(float r, float g, float b){
   color = {r,g,b};
}
void Point::setID(int _ID){ID = _ID;}
//----- Getters -----//
float Point::x(){return cord[0];}
float Point::y(){return cord[1];}
float Point::z(){return cord[2];}
float Point::w(){return cord[3];}
vector<float> Point::getCord(){return cord;}
int Point::getID(){return ID;}
