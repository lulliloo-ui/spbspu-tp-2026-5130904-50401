#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "polygon.hpp"

namespace madieva
{
  double getArea(const Polygon & polygon);

 void even_odd_num_filter(std::istream & in, std::ostream & out,
    const std::vector< Polygon > & polygons,std::string param,
    std::vector<Polygon> & filtered);

  void cmd_area(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

  void cmd_max(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

  void cmd_min(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

  void cmd_count(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);

}

#endif
