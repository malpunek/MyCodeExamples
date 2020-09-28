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

// TODO: to template
class node {

private:
  // TODO: https://en.cppreference.com/w/cpp/types/numeric_limits
  static const int INFINITY = (1 << 30);

  int value;
  int max_subtree_value;
  int l, r;
  node *ls, *rs, *parent;

  inline int length();
  void divide();
  void maximalRangesFitting(int p, int q, vector<pair<node *, int>> &V,
                            int value_inherited);
  void update();

public:
  node(int l, int r, node *par);
  node(int l, int r);
  node(int size);

  void add(int val, int p, int q);
  int query(int p, int q);
};
