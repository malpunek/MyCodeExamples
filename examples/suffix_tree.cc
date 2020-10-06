#include <algorithm>
#include <iostream>
#include <libalgo/suffix_tree.hh>

class X {
private:
  int value;

public:
  X(int x) : value(x){};
  X(char x) : value(x){};
  X(const X& x) : value(x.value){};
  operator int() { return value; };
  operator char() const { return value; };
};

int main() {
  // char z = (char) X('z');
  // std::cout << z << std::endl;
  std::string x("Ala");
  std::vector<char> y{'B', 'a', 's', 'i', 'a'};
  std::vector<int> z{'B', 'a', 's', 'i', 'a'};
  std::vector<X> q{X('M'), X('a'), X('l'), X('g'),
                   X('o'), X('s'), X('i'), X('a')};
  std::vector<char> p(std::begin(q), std::end(q));
  libalgo::SuffixTree buf('$', x, y, z, q);
  libalgo::SuffixTree suf('$', std::move(x), std::move(y));
  std::for_each(suf.the_string.cbegin(), suf.the_string.cend(),
                [](auto pt) { std::cout << pt << std::endl; });
  std::cout << std::endl << std::endl;
  std::for_each(buf.the_string.cbegin(), buf.the_string.cend(),
                [](auto pt) { std::cout << pt << std::endl; });
  return 0;
}