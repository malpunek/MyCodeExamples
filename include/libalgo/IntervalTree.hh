/*
Interval Tree
Created by Stanislaw Morawski

This implementation of interval tree lets you perform
standard operations over a range as big as sizeof(long long)

Interface:

The tree works on half open intervals - [begin, end).

The template parameters are as follows:
libalgo::IntervalTree<
  IND_T: Index Type - he type to index positions in the tree. Most commonly it
    would be either int or long lo,
  VAL_T: Value Type (default = Index Type) - The type of values to hold in the
    tree,
>

IntervalTree<..>(IND_T end) constructs a tree operating on [{}, end)
(including {} and excluding n) (Note: for builtin numeric types {} = 0)

IntervalTree(IND_T begin, IND_T end) constructs a tree operating on [begin, end)

add(VAL_T value, IND_T begin, IND_T end) adds value to [begin, end)
query(IND_T begin, IND_T end) returns maximum value in [begin, end)
size() returns the number of nodes in the tree

Complexity:
n - the number of distinct possible values in [begin, end)
m - the number of performed operations

Constructors work in constant time
add() and query() work in O(log(n)) time
The memory used is O(m * log(n))

size() works in O(m * log(n)) as having it faster would require storing
additional value and I assumed it would only be used for debugging purposses

*/

#ifndef LIBALGO_INTERVAL_TREE
#define LIBALGO_INTERVAL_TREE

#include <cassert>
#include <cstddef>
#include <limits>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

namespace libalgo {

template <
    typename IND_T, typename VAL_T = IND_T,
    // TODO Relax assumptions on the types
    VAL_T (*GET_VAL_LOWEST)() = std::numeric_limits<VAL_T>::lowest,
    typename std::enable_if<std::is_arithmetic<IND_T>::value, int>::type = 0,
    typename std::enable_if<std::is_arithmetic<VAL_T>::value, int>::type = 0>
class IntervalTree {

private:
  // TODO Perhaps this should be done differently
  inline static const VAL_T kLowestVal = GET_VAL_LOWEST();

  VAL_T value = {};
  VAL_T max_subtree_value = {};

  IND_T begin, end;
  IntervalTree *ls, *rs, *parent;

  inline bool less_equal(const IND_T &lhs, const IND_T &rhs) {
    return (lhs < rhs or lhs == rhs);
  }

  inline IND_T length() const { return end - begin; }

  inline IND_T new_mid() const { return begin + length() / 2; }

  inline bool divisible() const {
    return begin != new_mid() and end != new_mid();
  }

  void divide() {
    // TODO delete assert
    assert(divisible());
    if (ls != nullptr)
      return;
    ls = new IntervalTree(begin, new_mid(), this);
    rs = new IntervalTree(new_mid(), end, this);
    return;
  }

  void maximalRangesFitting(IND_T search_begin, IND_T search_end,
                            std::vector<std::pair<IntervalTree *, VAL_T>> &V,
                            VAL_T value_inherited) {
    if (less_equal(end, search_begin) or less_equal(search_end, begin))
      return;
    bool fits = less_equal(search_begin, begin) and less_equal(end, search_end);
    if (fits or !divisible())
      V.push_back(std::make_pair(this, value_inherited));
    else {
      divide();
      ls->maximalRangesFitting(search_begin, search_end, V,
                               value_inherited + value);
      rs->maximalRangesFitting(search_begin, search_end, V,
                               value_inherited + value);
    }
    return;
  }

  void update() {
    max_subtree_value = {};
    if (ls != nullptr)
      max_subtree_value = ls->max_subtree_value + ls->value;
    if (rs != nullptr)
      max_subtree_value =
          std::max(max_subtree_value, rs->max_subtree_value + rs->value);
    if (parent != nullptr)
      parent->update();
  }

public:
  IntervalTree(IND_T begin, IND_T end, IntervalTree *par = nullptr)
      : begin(begin), end(end), ls(nullptr), rs(nullptr), parent(par){};

  IntervalTree(IND_T size) : IntervalTree({}, size, nullptr){};

  void add(VAL_T val, IND_T begin_add, IND_T end_add) {
    assert(parent == nullptr);

    std::vector<std::pair<IntervalTree *, VAL_T>> V;
    this->maximalRangesFitting(begin_add, end_add, V, {});

    for (auto &i : V)
      i.first->value = i.first->value + val;

    for (auto &i : V)
      if (i.first->begin == begin_add or i.first->end == end_add)
        i.first->update();
    return;
  }

  VAL_T query(IND_T begin_query, IND_T end_query) {
    assert(parent == nullptr);

    std::vector<std::pair<IntervalTree *, VAL_T>> V;
    this->maximalRangesFitting(begin_query, end_query, V, 0);

    VAL_T result = kLowestVal;
    for (auto &i : V)
      result = std::max(result,
                        i.first->max_subtree_value + i.first->value + i.second);
    return result;
  }

  size_t size() const {
    return ls != nullptr ? ls->size() + rs->size() + 1 : 1;
  }
};

} // namespace libalgo

#endif