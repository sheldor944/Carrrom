#include "headers/maths.h"
#include "headers/libHeaders.h"

double distanceSq(double x1, double y1, double x2, double y2)
{
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

double dist(double x1, double y1, double x2, double y2)
{
    return sqrt((double) (x2 - x1) * (double) (x2 - x1) + (double) (y2 - y1) * (double) (y2 - y1));
}