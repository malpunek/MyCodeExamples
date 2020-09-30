#include <cstddef>
#include <cstdio>
#include <vector>

#include "libalgo/set.hh"

int main() {
  int queries;
  scanf(" %d", &queries);
  libalgo::SplaySet<int> S;
  for (int i = 0; i < queries; i++) {
    int type, x;
    scanf(" %d %d", &type, &x);
    if (type == 5) {
      if (S.find(x))
        printf("%d in set\n", x);
      else
        printf("%d not in set\n", x);
    } else if (type > 0)
      S.insert(x);
    else if (type < 0)
      S.erase(x);
    else {
      int val;
      scanf(" %d", &val);
      S.shift(x, val);
    }
    std::vector<int> VVV = S.sortedValues();
    for (size_t i = 0; i < VVV.size(); i++)
      printf("%d ", VVV[i]);
    printf("\n");
  }
  return 0;
}