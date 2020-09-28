// Created by Stanislaw Morawski
// Interval Tree
//
// This implementation of interval tree lets you perform
// standard operations over a range as big as sizeof(integer)
//
// You could easily transform it to an interval tree operating over range as big
// as sizeof(long long)
//
// Amount of memory and time consumed by the tree is pessimisticaly O(m*logn)
// where n is the size of the tree and m is the number of operations performed
//
// Interface:
//
// node(int n) constructs a tree operating on range [1, n] (including 1 and n)
// node(int p, int q) constructs a tree operating on range [p, q] (including p
// and q) Works in constant time.
//
// add(value, p, q) adds value to range[p, q] (including p and q)
// query(p, q) returns maximum value in range[p, q] (including p and q)
//
// Both add() and query() work in O(log(n)) time
//
//
#include <vector>
using namespace std;

class node {

private:
  static const int INFINITY = (1 << 30);

  int value;
  int max_subtree_value;
  int l, r;
  node *ls, *rs, *parent;

  inline int length() { return r - l; }

  void divide() {
    if (ls != NULL)
      return;
    ls = new node(l, l + length() / 2, this);
    rs = new node(l + length() / 2 + 1, r, this);
    return;
  }

  void maximalRangesFitting(int p, int q, vector<pair<node *, int>> &V,
                            int value_inherited) {
    if (p > r or q < l)
      return;
    else if (p <= l and q >= r)
      V.push_back(make_pair(this, value_inherited));
    else {
      divide();
      ls->maximalRangesFitting(p, q, V, value_inherited + value);
      rs->maximalRangesFitting(p, q, V, value_inherited + value);
    }
    return;
  }

  void update() {
    max_subtree_value = 0;
    if (ls != NULL)
      max_subtree_value = ls->max_subtree_value + ls->value;
    if (rs != NULL)
      max_subtree_value =
          max(max_subtree_value, rs->max_subtree_value + rs->value);
    if (parent != NULL)
      parent->update();
  }

public:
  node(int l, int r, node *par)
      : value(0), max_subtree_value(0), l(l), r(r), ls(NULL), rs(NULL) {
    parent = par;
  };

  node(int l, int r) : node(l, r, NULL){};

  node(int size) : node(1, size, NULL){};

  void add(int val, int p, int q) {
    vector<pair<node *, int>> V;
    this->maximalRangesFitting(p, q, V, 0);
    for (auto i : V)
      i.first->value += val;
    for (auto i : V)
      if (i.first->l == p or i.first->r == q)
        i.first->update();
    return;
  }

  int query(int p, int q) {
    vector<pair<node *, int>> V;
    this->maximalRangesFitting(p, q, V, 0);
    int result = -INFINITY;
    for (auto i : V)
      result =
          max(result, i.first->max_subtree_value + i.first->value + i.second);
    return result;
  }
};
