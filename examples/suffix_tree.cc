#include <algorithm>
#include <iostream>
#include <libalgo/suffix_tree.hh>
#include <string>

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
  int n;
  std::cin >> n;
  while (n--) {
    std::string s;
    std::cin >> s;
    libalgo::SuffixTree st(s);
    size_t queries;
    std::cin >> queries;
    std::string y = "TAK\n", n = "NIE\n";
    while (queries--) {
      std::string query;
      std::cin >> query;
      std::cout << (st.find(query) ? y : n);
    }
  }
  return 0;
}