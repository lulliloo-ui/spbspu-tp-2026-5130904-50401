#include "polygon.hpp"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>

namespace madieva
{
  double getArea(const Polygon & polygon)
  {
    if (polygon.points.size() < 3) {
      return 0.0;
    }

    std::vector< double > crossProducts(polygon.points.size());
    std::transform(
      polygon.points.begin(),
      polygon.points.end() - 1,
      polygon.points.begin() + 1,
      crossProducts.begin(),
      [](const Point & a, const Point & b) {
        return static_cast< double >(a.x * b.y - b.x * a.y);
      }
    );

    crossProducts.back() = static_cast< double >(
      polygon.points.back().x * polygon.points.front().y -
      polygon.points.front().x * polygon.points.back().y
    );

    double sum = std::accumulate(crossProducts.begin(), crossProducts.end(), 0.0);

    return std::abs(sum) / 2.0;
  }

  void even_odd_num_filter(std::istream & in, std::ostream & out,
    const std::vector< Polygon > & polygons,std::string param,
    std::vector<Polygon> & filtered)
  {
    if (param == "EVEN") {
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered),
      [](const Polygon & p) {
        return p.points.size() % 2 == 0;
      });
    } else if (param == "ODD") {
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered),
      [](const Polygon & p) {
        return p.points.size() % 2 != 0;
      });
    } else {
      try {
        size_t vertex = std::stoul(param);
        std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered),
          [vertex](const Polygon & p) {
            return p.points.size() == vertex;
          });
      } catch (...) {
        out << "<INVALID COMMAND>\n";
        return;
      }
    }
  }

  void cmd_area(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
  {
    std::string param;
    if (!(in >> param)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::vector<Polygon> filtered;
    if (param == "MEAN") {
      if (polygons.empty()) {
        out << "<INVALID COMMAND>\n";
        return;
      }
      filtered = polygons;
    } else {
      even_odd_num_filter(in, out, polygons, param, filtered);
    }
    std::vector< double > areas;
    areas.reserve(filtered.size());
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), getArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0, std::plus< double >());

    if (param == "MEAN") {
      out << std::fixed << std::setprecision(1) << (sum / filtered.size()) << "\n";
    } 
    else {
      out << std::fixed << std::setprecision(1) << sum << "\n";
    }
  }

  void cmd_max(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
  {
    std::string param;
    if (!(in >> param)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (polygons.empty()) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (param == "AREA") {
      auto it = std::max_element(polygons.begin(), polygons.end(),
        [](const Polygon & a, const Polygon & b) {
          return getArea(a) < getArea(b);
        });
      out << std::fixed << std::setprecision(1) << getArea(*it) << "\n";
    } else if (param == "VERTEXES") {
      auto it = std::max_element(polygons.begin(), polygons.end(),
        [](const Polygon & a, const Polygon & b) {
          return a.points.size() < b.points.size();
        });
      out << it->points.size() << "\n";
    } else {
      out << "<INVALID COMMAND>\n";
    }
  }

  void cmd_min(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
  {
    std::string param;
    if (!(in >> param)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (polygons.empty()) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (param == "AREA") {
      auto it = std::min_element(polygons.begin(), polygons.end(),
        [](const Polygon & a, const Polygon & b) {
          return getArea(a) < getArea(b);
        });
      out << std::fixed << std::setprecision(1) << getArea(*it) << "\n";
    } else if (param == "VERTEXES") {
      auto it = std::min_element(polygons.begin(), polygons.end(),
        [](const Polygon & a, const Polygon & b) {
          return a.points.size() < b.points.size();
        });
      out << it->points.size() << "\n";
    } else {
      out << "<INVALID COMMAND>\n";
    }
  }

  void cmd_count(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
  {
    std::string param;
    if (!(in >> param)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::vector<Polygon> filtered;
    even_odd_num_filter(in, out, polygons, param, filtered);
    out << filtered.size();
  }

  void cmd_same(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
  {

  }

}
