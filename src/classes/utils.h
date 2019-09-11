#ifndef UTILS_H
#define UTILS_H
#include "point.h"

namespace utils{
  float crossProduct(Point *p1, Point *p2, Point *p3);
  float distancePointLine(Point *pl1, Point *pl2, Point *p);
}

#endif
