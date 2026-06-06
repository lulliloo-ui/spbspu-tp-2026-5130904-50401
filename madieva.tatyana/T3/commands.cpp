#include "commands.hpp"
#include "polygon.hpp"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <functional>
#include <limits>
#include <cmath>
#include <iterator>

namespace madieva
{
  double getCrossProduct(const Point & a, const Point & b)
  {
    return static_cast<double>(a.x * b.y - b.x * a.y);
  }

  double getArea(const Polygon & polygon)
  {
    if (polygon.points.size() < 3) {
      return 0.0;
    }

    std::vector< double > crossProducts(polygon.points.size());
    using namespace std::placeholders;
    auto crossProductFunc = std::bind(getCrossProduct, _1, _2);
    std::transform(
      polygon.points.begin(),
      polygon.points.end() - 1,
      polygon.points.begin() + 1,
      crossProducts.begin(),
      crossProductFunc);

    crossProducts.back() = static_cast< double >(
      polygon.points.back().x * polygon.points.front().y -
      polygon.points.front().x * polygon.points.back().y
    );

    double sum = std::accumulate(crossProducts.begin(), crossProducts.end(), 0.0);

    return std::abs(sum) / 2.0;
  }

  bool isEven(const Polygon & p)
  {
    return p.points.size() % 2 == 0;
  }

  bool isOdd(const Polygon & p)
  {
    return p.points.size() % 2 != 0;
  }

  bool hasVertexCount(const Polygon & p, size_t vertex)
  {
    return p.points.size() == vertex;
  }

  void cmdArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
  {
    std::string param;
    if (!(in >> param)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::vector< Polygon > filtered;
    using namespace std::placeholders;
    if (param == "MEAN") {
      if (polygons.empty()) {
        out << "<INVALID COMMAND>\n";
        return;
      }
      filtered = polygons;
    } else if (param == "EVEN") {
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered),
      isEven);
    } else if (param == "ODD") {
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered),
      isOdd);
    } else {
      try {
        size_t vertex = std::stoul(param);
        if (vertex < 3) {
          out << "<INVALID COMMAND>\n";
          return;
        }
        auto hasVertex = std::bind(hasVertexCount, _1, vertex);
        std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered),
          hasVertex);
      } catch (...) {
        out << "<INVALID COMMAND>\n";
        return;
      }
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

  bool isAreaLess(const Polygon & a, const Polygon & b)
  {
    return getArea(a) < getArea(b);
  }

  bool isVertexLess(const Polygon & a, const Polygon & b)
  {
    return a.points.size() < b.points.size();
  }

  void cmdMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
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
        isAreaLess);
      out << std::fixed << std::setprecision(1) << getArea(*it) << "\n";
    } else if (param == "VERTEXES") {
      auto it = std::max_element(polygons.begin(), polygons.end(),
        isVertexLess);
      out << it->points.size() << "\n";
    } else {
      out << "<INVALID COMMAND>\n";
    }
  }

  void cmdMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
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
        isAreaLess);
      out << std::fixed << std::setprecision(1) << getArea(*it) << "\n";
    } else if (param == "VERTEXES") {
      auto it = std::min_element(polygons.begin(), polygons.end(),
        isVertexLess);
      out << it->points.size() << "\n";
    } else {
      out << "<INVALID COMMAND>\n";
    }
  }

  void cmdCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
  {
    std::string param;
    if (!(in >> param)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::vector< Polygon > filtered;
    using namespace std::placeholders;
    if (param == "EVEN") {
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered),
      isEven);
    } else if (param == "ODD") {
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered),
      isOdd);
    } else {
      try {
        size_t vertex = std::stoul(param);
        if (vertex < 3) {
          out << "<INVALID COMMAND>\n";
          return;
        }
        auto hasVertex = std::bind(hasVertexCount, _1, vertex);
        std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered),
          hasVertex);
      } catch (...) {
        out << "<INVALID COMMAND>\n";
        return;
      }
    }
    out << filtered.size() << "\n";
  }

  void cmdSame(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
  {
    Polygon target;
    if (!(in >> target)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (target.points.size() < 3 ) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    auto same = std::bind(
      std::equal_to<Polygon>(),
      std::placeholders::_1,
      target
    );
    size_t count = std::count_if(polygons.begin(), polygons.end(), same);
    out << count << "\n";
  }

  bool isXLess(const Point & a, const Point & b)
  {
    return a.x < b.x;
  }

  bool isYLess(const Point & a, const Point & b)
  {
    return a.y < b.y;
  }

  Frame getFrame(const Polygon & p)
  {
    Frame f;
    using namespace std::placeholders;
    auto xLess = std::bind(isXLess, _1, _2);
    auto yLess = std::bind(isYLess, _1, _2);
    auto result_x = std::minmax_element(p.points.begin(), p.points.end(), xLess);
    auto result_y = std::minmax_element(p.points.begin(), p.points.end(), yLess);

    f.x_min = result_x.first->x;
    f.x_max = result_x.second->x;
    f.y_min = result_y.first->y;
    f.y_max = result_y.second->y;
    return f;
  }

  bool isXMinLess(const Frame & a, const Frame & b)
  {
    return a.x_min < b.x_min;
  }

  bool isXMaxLess(const Frame & a, const Frame & b)
  {
    return a.x_max < b.x_max;
  }

  bool isYMinLess(const Frame & a, const Frame & b)
  {
    return a.y_min < b.y_min;
  }

  bool isYMaxLess(const Frame & a, const Frame & b)
  {
    return a.y_max < b.y_max;
  }

  void cmdInframe(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
  {
    Polygon target;
    if (!(in >> target)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (target.points.size() < 3 ) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (polygons.empty()) {
      out << "<FALSE>\n";
      return;
    }
    std::vector< Frame > f;
    f.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(f), getFrame);
    auto min_x = std::min_element(f.begin(), f.end(), isXMinLess);
    auto max_x = std::max_element(f.begin(), f.end(), isXMaxLess);
    auto min_y = std::min_element(f.begin(), f.end(), isYMinLess);
    auto max_y = std::max_element(f.begin(), f.end(), isYMaxLess);
    const Frame global_f {
      min_x->x_min,
      max_x->x_max,
      min_y->y_min,
      max_y->y_max
    };
    const Frame target_f = getFrame(target);
    const bool is_inside = (target_f.x_min >= global_f.x_min) &&
      (target_f.x_max <= global_f.x_max) &&
      (target_f.y_min >= global_f.y_min) &&
      (target_f.y_max <= global_f.y_max);
    if (is_inside) {
      out << "<TRUE>\n";
    } else {
      out << "<FALSE>\n";
    }
  }
}
