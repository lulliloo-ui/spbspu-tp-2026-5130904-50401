#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "polygon.hpp"

namespace madieva
{
  double getArea(const Polygon & polygon);

  void cmd_area(std::istream & in, std::ostream & out, const std::vector< Polygon > polygons);

}

#endif
