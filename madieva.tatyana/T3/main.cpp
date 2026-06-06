#include "polygon.hpp"
#include "commands.hpp"
#include <string>
#include <fstream>
#include <iterator>
#include <iostream>
#include <map>
#include <functional>
#include <limits>
#include <algorithm>

int main(int argc, char * argv[])
{
  if (argc != 2) {
    return 1;
  }
  std::string file_name = argv[1];
  std::ifstream file(file_name);
  if (!file.is_open()) {
    std::cerr << "Error: cannot open file\n";
    return 1;
  }

  std::vector< madieva::Polygon > polygons;
  using iit_t = std::istream_iterator< madieva::Polygon >;
  std::copy(iit_t{file}, iit_t{}, std::back_inserter(polygons));
  polygons.erase(
    std::remove_if(polygons.begin(), polygons.end(), [](const madieva::Polygon & p) {
      return p.points.empty();
    }),
    polygons.end()
  );
  using Command = std::function< void(std::istream &, std::ostream &, const std::vector< madieva::Polygon > &) >;
  std::map < std::string, Command > commands;
  commands["AREA"] = madieva::cmd_area;
  commands["MAX"] = madieva::cmd_max;
  commands["MIN"] = madieva::cmd_min;
  commands["COUNT"] = madieva::cmd_count;
  commands["SAME"] = madieva::cmd_same;
  commands["INFRAME"] = madieva::cmd_inframe;

  std::string command_name;
  while (std::cin >> command_name) {
    auto it = commands.find(command_name);
    if (it != commands.end()) {
      it->second(std::cin, std::cout, polygons);
    } else {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
