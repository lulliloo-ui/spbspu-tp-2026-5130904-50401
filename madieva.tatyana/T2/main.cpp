#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <iomanip>
#include <limits>
#include <algorithm>

namespace madieva {
  struct DataStruct
  {
    double key1;
    unsigned long long key2;
    std::string key3;
    bool operator<(const DataStruct& rhs) const;
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

}
bool madieva::DataStruct::operator<(const DataStruct& rhs) const
{
  if (key1 != rhs.key1) {
    return key1 < rhs.key1;
  }
  if (key2 != rhs.key2) {
    return key2 < rhs.key2;
  }
  return key3.length() < rhs.key3.length();
}

std::istream & madieva::operator>>(std::istream & in, DelimiterIO && dest)
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

std::istream & madieva::operator>>(std::istream & in, DoubleIO && dest)
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

std::istream & madieva::operator>>(std::istream & in, ULLIO && dest)
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

std::istream & madieva::operator>>(std::istream & in, StringIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}

std::istream & madieva::operator>>(std::istream & in, LabelIO && dest)
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

std::istream & madieva::operator>>(std::istream & in, DataStruct & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }

  double k1 = 0;
  unsigned long long k2 = 0.0;
  std::string k3 = "";
  std::string key;
  bool have1 = false, have2 = false, have3 = false;

  in >> DelimiterIO{'('};
  in >> DelimiterIO{':'};
  for (int i = 0; i < 3 && in; ++i) {
    in >> key;
    if (key == "key1") {
      in >> DoubleIO{k1};
      if (in) have1 = true;
    } else if (key == "key2") {
      in >> ULLIO{k2};
      if (in) have2 = true;
    } else if (key == "key3") {
      in >> StringIO{k3};
      if (in) have3 = true;
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
  if (!in) {
    in.clear();
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return in >> dest;
  }
  return in;
}

std::ostream & madieva::operator<<(std::ostream  & out, const DataStruct & src)
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

madieva::IOguard::IOguard(std::basic_ios< char > & s) :
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

int main()
{
  using madieva::DataStruct;
  using T = DataStruct;
  std::vector< T > data;
  using iit_t = std::istream_iterator< T >;
  std::copy(iit_t{std::cin}, iit_t{}, std::back_inserter(data));
  std::sort(data.begin(), data.end());
  using oit_t = std::ostream_iterator< DataStruct >;
  std::copy(std::begin(data), std::end(data), oit_t{std::cout, "\n"});

}
