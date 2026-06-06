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

  void even_odd_num_filter(std::istream & in, std::ostream & out,
    const std::vector< Polygon > & polygons, std::string param,
    std::vector< Polygon > & filtered);

  void cmd_area(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

  void cmd_max(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

  void cmd_min(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

  void cmd_count(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

  void cmd_same(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

  struct Frame {
    int x_min = std::numeric_limits< int >::max();
    int x_max = std::numeric_limits< int >::min();
    int y_min = std::numeric_limits< int >::max();
    int y_max = std::numeric_limits< int >::min();
  };

  Frame getFrame(const Polygon & p);

  void cmd_inframe(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

}

#endif
