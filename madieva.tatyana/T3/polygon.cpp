#include "polygon.hpp"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <limits>
#include <string>
#include <functional>

namespace madieva
{
  bool compare(const Point & a, const Point & b, int dx, int dy)
  {
    return a.x + dx == b.x && a.y + dy == b.y;
  }

  bool Polygon::operator==(const Polygon & rhs) const
  {
    if (points.size() != rhs.points.size()) {
      return false;
    }
    if (points.empty()) {
      return true;
    }

    const int dx = rhs.points.front().x - points.front().x;
    const int dy = rhs.points.front().y - points.front().y;

    using namespace std::placeholders;
    auto comparator = std::bind(compare, _1, _2, dx, dy);

    return std::equal(points.begin(), points.end(), rhs.points.begin(), comparator);
  }

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

  Point readPoint(std::istream & in)
  {
    char next_char = in.peek();
    if (next_char == '\n') {
      in.setstate(std::ios::failbit);
    }
    Point p;
    in >> p;
    return p;
  }

  std::istream & operator>>(std::istream & in, Polygon & dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    size_t vertexCount = 0;
    in >> vertexCount;

    if (!in || vertexCount < 3) {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      dest.points.clear();
      return in;
    }
    dest.points.clear();

    using namespace std::placeholders;
    auto reader = std::bind(readPoint, std::ref(in));
    std::generate_n(std::back_inserter(dest.points), vertexCount, reader);
    if (!in) {
      dest.points.clear();
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      return in;
    }
    char next_char = in.peek();
    if (next_char != '\n' && next_char != EOF) {
      dest.points.clear();
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
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
