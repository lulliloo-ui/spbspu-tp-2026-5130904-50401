#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <iosfwd>

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
    bool operator==(const Polygon & rhs) const;
  };

  std::istream & operator>>(std::istream & in, DelimiterIO && dest);

  std::istream & operator>>(std::istream & in, Point & dest);

  std::istream & operator>>(std::istream & in, Polygon & dest);

  std::ostream & operator<<(std::ostream & out, const Point & p);

  std::ostream & operator<<(std::ostream & out, const Polygon & dest);

}

#endif
