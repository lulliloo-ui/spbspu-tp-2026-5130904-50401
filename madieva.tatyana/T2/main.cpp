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

  std::istream & operator>>(std::istream & in, DataStruct & dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }

    double k1 = 0;
    unsigned long long k2 = 0;
    std::string k3 = "";
    std::string key;

    bool have1 = false, have2 = false, have3 = false;

    in >> DelimiterIO{'('};
    for (int i = 0; i < 3; ++i) {
      in >> DelimiterIO{':'};
      in >> key;
      in >> DelimiterIO{' '};
      if (key == "key1") {
        in >> DoubleIO{k1};
        have1 = true;
      } else if (key == "key2") {
        in >> ULLIO{k2};
        have2 = true;
      } else if (key == "key3") {
        in >> StringIO{k3};
        have3 = true;
      } else {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> DelimiterIO{':'};
    }
    in >> DelimiterIO{')'};
    if (have1 && have2 && have3) {
      dest.key1 = k1;
      dest.key2 = k2;
      dest.key3 = k3;
    } else {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream& madieva::operator<<(std::ostream& out, const DataStruct& src)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) {
        return out;
    }

    IOguard guard(out);
    
    out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1 << "d";
    out << ":key2 " << src.key2 << "ull";
    out << ":key3 \"" << src.key3 << "\":)";
    return out;
  }


  madieva::IOguard::IOguard(std::basic_ios<char>& s) :
    s_(s),
    width_(s.width()),
    precision_(s.precision()),
    fmt_(s.flags()),
    fill_(s.fill())
{}

  madieva::IOguard::~IOguard()
  {
    s_.width(width_);
    s_.precision(precision_);
    s_.flags(fmt_);
    s_.fill(fill_);
  }

}




int main()
{
  using namespace madieva;
    std::string test = "(:key2 42ull:key1 3.14d:key3 \"hello\":)";
    std::istringstream iss(test);
    DataStruct ds;
    iss >> ds;
    if (iss) {
        std::cout << ds << std::endl;
    } else {
        std::cout << "Ошибка чтения" << std::endl;
    }


}
