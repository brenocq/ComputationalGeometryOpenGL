#include "quickHull4D.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include <unistd.h>
#include <math.h>
#include <algorithm>
#include <iomanip>
#include "../utils.h"

QuickHull4D::QuickHull4D():
ConvexHull()
{}

QuickHull4D::~QuickHull4D()
{}

vector<Point*> QuickHull4D::run(){
    vector<Point*> S1, S2, S3, S4;
    Point *p[4];// Maximum or minimum points (belong to the convexHull)
    convexHull.clear();

    if(_container->getPoints().size()<4)
        return convexHull;

    cout << "---------- Calculate Convex Hull ----------\n";
    cout << setprecision(2);

    //----- Find min/max points ("left most", "right most")-----//
    p[0] = _container->getPoints()[0];
    p[1] = _container->getPoints()[0];
    p[2] = _container->getPoints()[0];
    p[3] = _container->getPoints()[0];
    for(auto point : _container->getPoints()){
        if(point->x()<p[0]->x())
            p[0] = point;
        if(point->x()>p[1]->x())
            p[1] = point;
        if(point->y()<p[2]->y())
            p[2] = point;
        if(point->y()>p[3]->y())
            p[3] = point;
    }

    for(auto pointA : p){
      int qtdEqual = 0;
      for(auto pointB : p){
        if(pointA==pointB)
          qtdEqual++;
      }
      if(qtdEqual>1){
        cout<<"[Warning] Same initial point on the QuickHull4D algorithm.\n";
        return convexHull;
      }

    }

    for(auto point : p){
      convexHull.push_back(point);
      firstPoints.push_back(point);// To draw
    }

    cout<<"\tFirst points: ";
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
            cout<<(i==0?"(":"")<<p[j]->getCord()[i]<<(i!=3?",":") ");
    }
    cout<<endl;

    //----- Divide points in two subsets -----//
    for(auto point : _container->getPoints()){
        float position = usePlaneEquation(vector<Point*>{p[0], p[1], p[2], p[3]}, point);

        cout<<"Position with ";
        for (int i = 0; i < 4; i++)
            cout<<(i==0?"(":"")<<point->getCord()[i]<<(i!=3?",":") ");
        cout<<"-> "<<position<<endl;


        // On the line or to one side
        if(position>0 && abs(position)>0.0001){
            S1.push_back(point);
        }else if(position<0 && abs(position)>0.0001){
            S2.push_back(point);
        }
    }

    //----- Divide points in for subsets -----//
    cout<<"\n------------- FIND HULL S1 -------------\n";
    findHull(S1, p[0], p[1], p[2], p[3]);
    cout<<"\n------------- FIND HULL S2 -------------\n";
    findHull(S2, p[3], p[2], p[1], p[0]);

    return convexHull;
}

void QuickHull4D::findHull(vector<Point*> points, Point* P, Point* Q, Point* R, Point* S){
    Point* C;
    vector<Point*> S1, S2, S3, S4;
    cout<<"\n-------------\nQtd points: "<<points.size()<<endl;
    cout<<"\tPlane point: ";
    for (int i = 0; i < 4; i++)
        cout<<(i==0?"(":"")<<P->getCord()[i]<<(i!=3?",":") ");
    for (int i = 0; i < 4; i++)
        cout<<(i==0?"(":"")<<Q->getCord()[i]<<(i!=3?",":") ");
    for (int i = 0; i < 4; i++)
        cout<<(i==0?"(":"")<<R->getCord()[i]<<(i!=3?",":") ");
    for (int i = 0; i < 4; i++)
        cout<<(i==0?"(":"")<<S->getCord()[i]<<(i!=3?",":")\n");

    cout<<"\tPoints Added: ";
    for(Point* point : points){
        for (int i = 0; i < 4; i++)
        cout<<(i==0?"(":"")<<point->getCord()[i]<<(i!=3?",":") ");
    }
    cout<<endl;


    if(points.size()==0)
        return;

    pair<float,Point*> farthestPoint;
    farthestPoint.first = 0;
    farthestPoint.second = points[0];

    //---- Find farthest point to the line ----//
    // TODO calculate distance to hyperPlane
    for(auto point : points){
         // Calculate distance
         float dist;
         dist = abs(distanceToHyperPlane(vector<Point*>{P, Q, R, S}, point));

         cout<<"Distance plane to ";
         for (int i = 0; i < 4; i++)
          cout<<(i==0?"(":"")<<point->getCord()[i]<<(i!=3?",":") ");

          cout<<" -> "<<dist<<endl;

         if(abs(farthestPoint.first)<abs(dist)){
             farthestPoint.second = point;
             farthestPoint.first = dist;
         }
    }
    cout<<"\tFarthest: ";

    for (int i = 0; i < 4; i++)
        cout<<(i==0?"(":"")<<farthestPoint.second->getCord()[i]<<(i!=3?",":")");

    cout<<" -> distance plane:"<<farthestPoint.first<<endl;
    C = farthestPoint.second;
    convexHull.push_back(C);

    float position=0;

    for(auto point : points){

      //---- Calculate subset 1 ----//
      position = usePlaneEquation(vector<Point*>{C, Q, R, S}, point);
      if(position>0 && abs(position)>0.0001){
          S1.push_back(point);
      }else{
        //---- Calculate subset 2 ----//
        position = usePlaneEquation(vector<Point*>{P, C, R, S}, point);
        if(position>0 && abs(position)>0.0001){
            S2.push_back(point);
        }else{
          //---- Calculate subset 3 ----//
          position = usePlaneEquation(vector<Point*>{P, Q, C, S}, point);
          if(position>0 && abs(position)>0.0001){
              S3.push_back(point);
          }else{
            //---- Calculate subset 4 ----//
            position = usePlaneEquation(vector<Point*>{P, Q, R, C}, point);
            if(position>0 && abs(position)>0.0001){
                S4.push_back(point);
            }
          }
        }
      }
    }

    cout<<"C, Q, R, S";
    findHull(S1, C, Q, R, S);
    cout<<"P, C, R, S";
    findHull(S2, P, C, R, S);
    cout<<"P, Q, C, S";
    findHull(S3, P, Q, C, S);
    cout<<"P, Q, R, C";
    findHull(S4, P, Q, R, C);
}

void QuickHull4D::draw(){
    /*if(convexHull.size()<2)
        return;

    // Set all points as black
    for(auto point : _container->getPoints()){
    point->setColor(0,0,0);
    }

    //----- Convex hull lines and points -----//
    sortConvexHull();
    if(convexHull.size()>1){
    glLineWidth(2);
    glColor3f(0.0, 0.0, 1.0);
    convexHull[0]->setColor(0,0,1);
    for (unsigned int i = 1; i < convexHull.size(); i++) {
      convexHull[i]->setColor(0,0,1);
      glBegin(GL_LINES);
      glVertex3f(convexHull[i]->x(), convexHull[i]->y(), 0.0);
      glVertex3f(convexHull[i-1]->x(), convexHull[i-1]->y(), 0.0);
      glEnd();
    }
    glBegin(GL_LINES);
    glVertex3f(convexHull.back()->x(), convexHull.back()->y(), 0.0);
    glVertex3f(convexHull.front()->x(), convexHull.front()->y(), 0.0);
    glEnd();
    }

    //----- First points -----//
    firstPointsA->setColor(1,0,0);
    firstPointsB->setColor(1,0,0);*/
}

float QuickHull4D::distanceToHyperPlane(vector<Point*> planePoints, Point* point){
  Point v1 = *(planePoints[1])- *(planePoints[0]);
  Point v2 = *(planePoints[2])- *(planePoints[1]);
  Point v3 = *(planePoints[3])- *(planePoints[2]);

  Point vectorPoint = *point - *(planePoints[0]);

  float i = v1.y()*v2.z()*v3.w() - v1.w()*v2.z()*v3.y();
  float j = v1.z()*v2.w()*v3.x() - v1.x()*v2.w()*v3.z();
  float k = v1.w()*v2.x()*v3.y() - v1.y()*v2.x()*v3.w();
  float l = v1.x()*v2.y()*v3.z() - v3.x()*v2.y()*v1.z();

  float norm = sqrt(i*i + j*j + k*k + l*l);

  if(norm==0)
    return 0;

  Point normalVec = Point(i/norm, j/norm, k/norm, l/norm);

  // Scalar product (projection)
  float dist = normalVec.x()*vectorPoint.x() +
               normalVec.y()*vectorPoint.y() +
               normalVec.z()*vectorPoint.z() +
               normalVec.w()*vectorPoint.w();

  return dist;
}

float QuickHull4D::usePlaneEquation(vector<Point*>planePoints, Point* point){
  Point v1 = *(planePoints[1])- *(planePoints[0]);
  Point v2 = *(planePoints[2])- *(planePoints[0]);
  Point v3 = *(planePoints[3])- *(planePoints[0]);

  float i = v1.y()*v2.z()*v3.w() - v1.w()*v2.z()*v3.y();
  float j = v1.z()*v2.w()*v3.x() - v1.x()*v2.w()*v3.z();
  float k = v1.w()*v2.x()*v3.y() - v1.y()*v2.x()*v3.w();
  float l = v1.x()*v2.y()*v3.z() - v3.x()*v2.y()*v1.z();

  float dot1 = utils::dotProduct(&v1,new Point(i,j,k,l));
  float dot2 = utils::dotProduct(&v2,new Point(i,j,k,l));
  float dot3 = utils::dotProduct(&v3,new Point(i,j,k,l));

  cout << "Dots: "<<dot1<<" "<<dot2<<" "<<dot3<<endl;

  // Calculate points position with plane equation
  return i*(point->x()-planePoints[0]->x()) +
         j*(point->y()-planePoints[0]->y()) +
         k*(point->z()-planePoints[0]->z()) +
         l*(point->w()-planePoints[0]->w());
}

float QuickHull4D::volume(vector<Point*>p){
    // https://www.mathpages.com/home/kmath664/kmath664.html
    // Only works for 4d-simplex (4d points)
    // I am not sure this is working...

    float det;
    float matrix[5][5];

    vector<Point> vectors;
    vectors.push_back(*(p[1])-*(p[0]));
    vectors.push_back(*(p[2])-*(p[0]));
    vectors.push_back(*(p[3])-*(p[0]));
    vectors.push_back(*(p[4])-*(p[0]));

    int size  = vectors.size();

    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        matrix[i][j]=vectors[i].getCord()[j];
      }
    }

    // Determinant calculation
    det = utils::determinant(matrix, 4);
}

bool QuickHull4D::pointInside(Point* t1, Point* t2, Point* t3, Point* t4, Point* t5, Point* p) {
    float baseVolume;
    float volumes[5]; //Volumes w.r.t. the point p

    std::vector<Point*> pointSet;
    std::vector<Point*> bufferSet;

    pointSet.push_back(t5);
    pointSet.push_back(t4);
    pointSet.push_back(t3);
    pointSet.push_back(t2);
    pointSet.push_back(t1);

    baseVolume = volume(vector<Point*>{t1, t2, t3, t4, t5});// Changed from hyperVolume to volume

    for (int i = 0; i < 4; i++) {
        bufferSet = pointSet; //Copying into bufferSet
        bufferSet.at(i) =  p;
        volumes[i] = volume(vector<Point*>{
                                bufferSet.at(0),
                                bufferSet.at(1),
                                bufferSet.at(2),
                                bufferSet.at(3),
                                bufferSet.at(4)});
    }
}
