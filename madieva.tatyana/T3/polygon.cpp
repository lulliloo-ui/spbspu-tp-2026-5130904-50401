#include "polygon.hpp"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <limits>
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
    size_t vertexCount = 0;
    in >> vertexCount;
    if (!in) {
      in.clear();
      in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      dest.points.clear(); 
      return in;
    }

    dest.points.clear();
    std::generate_n(std::back_inserter(dest.points), vertexCount, [&in]() {
      Point p;
      in >> p;
      return p;
    });
    if (!in) {
      dest.points.clear();
      in.clear();
      in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Пропускаем остаток бракованной строки
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
