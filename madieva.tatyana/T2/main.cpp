#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <iterator>
#include <vector>
#include <iomanip>

namespace madieva {
  struct DataStruct
  {
    double key1;
    unsigned long long key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct DoubleIO
  {
    double & ref;
  };

  struct ULLIO
  {
    unsigned long long & ref;
  };

  struct StringIO
  {
    std::string & ref;
  };

  struct LabelIO
  {
    std::string exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleIO&& dest);
  std::istream& operator>>(std::istream& in, ULLIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, LabelIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);

  std::ostream& operator<<(std::ostream& out, const DataStruct& src);

  class IOguard
  {
  public:
    explicit IOguard(std::basic_ios< char >& s);
    ~IOguard();
  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };





}




int main()
{}
