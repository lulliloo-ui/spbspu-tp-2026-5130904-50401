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
    char expc;
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

  std::istream & operator>>(std::istream & in, DelimiterIO && dest);
  std::istream & operator>>(std::istream & in, DoubleIO && dest);
  std::istream & operator>>(std::istream & in, ULLIO && dest);
  std::istream & operator>>(std::istream & in, StringIO && dest);
  std::istream & operator>>(std::istream & in, LabelIO && dest);
  std::istream & operator>>(std::istream & in, DataStruct & dest);

  std::ostream & operator<<(std::ostream & out, const DataStruct & src);

  class IOguard
  {
  public:
    explicit IOguard(std::basic_ios< char > & s);
    ~IOguard();
  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };

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

  std::istream & operator>>(std::istream & in, DoubleIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    in >> dest.ref;
    if (!in) {
      return in;
    }
    char suffix = ' ';
    in >> suffix;
    if (suffix != 'd' && suffix != 'D') {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream & operator>>(std::istream & in, ULLIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    in >> dest.ref;
    if (!in) {
      return in;
    }
    char c1 = ' ', c2 = ' ', c3 = ' ';
    in >> c1 >> c2 >> c3;

    bool valid = ((c1 == 'u' || c1 == 'U') &&
      (c2 == 'l' || c2 == 'L') &&
      (c3 == 'l' || c3 == 'L'));

    if (!valid) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& madieva::operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
  }

  std::istream & operator>>(std::istream & in, LabelIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::string word = "";
    in >> word;
    if (in && (word != dest.exp)) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }



}




int main()
{}
