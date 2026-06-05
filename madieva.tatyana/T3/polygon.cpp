#include "polygon.hpp"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>

namespace madieva
{
  std::istream & operator>>(std::istream & in, DelimiterIO && dest)
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

  std::istream & operator>>(std::istream & in, Point & dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    in >> DelimiterIO{'('} >> dest.x >> DelimiterIO{';'} >> dest.y >> DelimiterIO{')'};
    return in;
  }

  std::istream & operator>>(std::istream & in, Polygon & dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::string line;
    std::getline(in, line);
    if (line.empty()) {
      return in;
    }
    std::istringstream iss(line);
    size_t vertexCount = 0;
    iss >> vertexCount;
    if (!iss) {
      return in;
    }
    dest.points.clear();
    dest.points.resize(vertexCount);
    std::generate(dest.points.begin(), dest.points.end(), [&iss]()
    {
      Point p;
      iss >> p;
      return p;
    });
    if (!iss) {
      dest.points.clear();
    }
    return in;
  }

  std::ostream & operator<<(std::ostream & out, const Point & p)
  {
    out << "(" << p.x << ";" << p.y << ")";
    return out;
  }

  std::ostream & operator<<(std::ostream & out, const Polygon & dest)
  {
    out << dest.points.size() << " ";
    std::copy(dest.points.begin(), dest.points.end(), 
      std::ostream_iterator< Point >(out, " "));
    return out;
  }

}
