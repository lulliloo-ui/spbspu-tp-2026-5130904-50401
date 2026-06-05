#include "polygon.hpp"
#include <iostream>
#include <algorithm>

namespace madieva
{
  std::istream & madieva::operator>>(std::istream & in, DelimiterIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = ' ';
    in >> c;
    if (in && (c != dest.expc)) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream & in, Point & dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
     return in;
    }
    in >> DelimiterIO{'('} >> dest.x >> DelimiterIO{';'} >> dest.y >> DelimiterIO{')'};
    return in;
  }

  std::istream& operator>>(std::istream & in, Polygon & dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
     return in;
    }
    size_t vertexCount = 0;
    in >> vertexCount;
    if (!in) {
      return in;
    }
    std::generate(dest.points.begin(), dest.points.end(), [&in]()
    {
      Point p;
      in >> p;
      return p;
    });
    if (!in) {
      dest.points.clear();
    }
    return in;
  }

}
