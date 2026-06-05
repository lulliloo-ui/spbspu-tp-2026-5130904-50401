#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>

namespace madieva {

  struct DelimiterIO
  {
    char expc;
  };

  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream & madieva::operator>>(std::istream & in, DelimiterIO && dest);

  std::istream& operator>>(std::istream & in, Point & dest);

  std::istream& operator>>(std::istream & in, Polygon & dest);


}

#endif
