
#pragma once

#include <stdlib.h>
#include <iostream>
#include <math.h>

namespace Generator
{
  struct Point
  {
    int x;
    int y;

    Point(int x = 0, int y = 0)
    {
      this->x = x;
      this->y = y;
    }

    double distance(Point other)
    {
      double xDistance = x - other.x;
      double yDistance = y - other.y;
      return sqrt((xDistance * xDistance) + (yDistance * yDistance));
    }

    double distance(Point segmentStart, Point segmentEnd)
    {
      return abs((segmentEnd.x - segmentStart.x) * (segmentStart.y - y) - (segmentStart.x - x) * (segmentEnd.y - segmentStart.y)) / (sqrt(double((segmentEnd.x - segmentStart.x) * (segmentEnd.x - segmentStart.x) + (segmentEnd.y - segmentStart.y) * (segmentEnd.y - segmentStart.y))));
    }

    int distanceTaxicab(Point other)
    {
      return (int) (abs(x - other.x) + abs(y - other.y));
    }

    bool operator==(const Point &other)
    {
        return x ==  other.x && y == other.y;
    }

    Point operator+(const Point &other)
    {
        return Point(x + other.x, y + other.y);
    }
  };
}