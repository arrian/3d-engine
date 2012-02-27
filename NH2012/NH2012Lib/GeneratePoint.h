
#pragma once

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

    double distance(Point point)
    {
      double xDistance = x - point.x;
      double yDistance = y - point.y;
      return sqrt((xDistance * xDistance) + (yDistance * yDistance));
    }

    double distance(Point segmentStart, Point segmentEnd)
    {
      return abs((segmentEnd.x - segmentStart.x) * (segmentStart.y - y) - (segmentStart.x - x) * (segmentEnd.y - segmentStart.y)) / (sqrt(double((segmentEnd.x - segmentStart.x) * (segmentEnd.x - segmentStart.x) + (segmentEnd.y - segmentStart.y) * (segmentEnd.y - segmentStart.y))));
    }

    bool operator==(const Point &other)
    {
        return x ==  other.x && y == other.y;
    }
  };
}