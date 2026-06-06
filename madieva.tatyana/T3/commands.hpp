#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "polygon.hpp"
#include <iosfwd>
#include <limits>
#include <string>
#include <vector>

namespace madieva
{
  double getArea(const Polygon & polygon);

  void cmdArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

  void cmdMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

  void cmdMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

  void cmdCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

  void cmdSame(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

  struct Frame {
    int x_min = std::numeric_limits< int >::max();
    int x_max = std::numeric_limits< int >::min();
    int y_min = std::numeric_limits< int >::max();
    int y_max = std::numeric_limits< int >::min();
  };

  Frame getFrame(const Polygon & p);

  void cmdInframe(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

}

#endif
