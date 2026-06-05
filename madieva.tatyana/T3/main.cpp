#include "polygon.hpp"
#include <string>
#include <fstream>
#include <iterator>
#include <iostream>

int main(int argc, char * argv[])
{
  if (argc != 2) {
    return 1;
  }
  std::string file_name = argv[1];
  std::ifstream file(file_name);
  if (!file.is_open()) {
    return 1;
  }

  std::vector< madieva::Polygon > polygons;
  using iit_t = std::istream_iterator< madieva::Polygon >;
  std::copy(iit_t{file}, iit_t{}, std::back_inserter(polygons));
  return 0;

}
