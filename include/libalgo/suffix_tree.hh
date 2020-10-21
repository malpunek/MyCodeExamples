// Stanislaw Morawski
//
// Implementation of Ukkonen's algorithm for online linear construction of
// suffix trees
//
// Assuming input is a word over alphabet (a.....z)

#ifndef LIBALGO_SUFFIX_TREE
#define LIBALGO_SUFFIX_TREE

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <string>
#include <type_traits>
// TODO move to robin_hood hashstructures
// github.com/martinus/robin-hood-hashing
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "libalgo/type_check.hh"

namespace libalgo {

template <typename C> class SuffixTree {

public:
  using T = _type_check::inner_type_t<C>;
  using T_vec = std::vector<T>;
  T_vec the_string;

private:
  struct node {
    size_t left, right, full_left;
    // empty leaf is needed for implicit leaves
    node *fail = nullptr, *empty_leaf = nullptr;
    std::unordered_map<T, node *> children;

    node *child(const T &symbol) {
      auto it = children.find(symbol);
      return it == children.end() ? nullptr : it->second;
    }
    node(size_t left, size_t right, size_t full_left = 0)
        : left(left), right(right), full_left(full_left){};
    ~node() {
      for_each(children.begin(), children.end(),
               [](const auto &i) { delete i.second; });
    };
  };

  node *root;

  node *constructST(const T_vec &s) {
    /*
    This is a McCreight and Ukkonen inspired algorithm for constructing suffix tree in
    O(n). It was presented to me (without the code) on TCS departament of Jagiellonian
    University (https://www.tcs.uj.edu.pl/en_GB/).

    It's beauty lies both in it's compactness (39 lines-of-code total) as well as in the
    fact that it's very easy to see/proove that it runs in linear time. There are three
    loops. In each pass of any of the loops one of {`done`, `active_begin`,
    `active_end`} is incremented by at least one. But at the end they are all smaller or
    equal to the length of the text. Since the rest of the operations are performed in
    constant time the algorithm is indeed linear in time (and space).
    */

    // We leverage Ukkonen's idea of auxilary state = `root->fail`. We can pass from
    // `root->fail` to the `root` by any valid word of length one. Hence `root` (which
    // is a representation of an empty suffix) is a node over (-1, 0).
    root = new node(-1, 0);
    root->fail = new node(0, 0);
    {
      std::unordered_set<T> alphabet(s.begin(), s.end());
      for (const auto &i : alphabet)
        (root->fail)->children[i] = root;
    }

    // `active` state (first non-leaf on the boudary path) and the active `edge` [refer
    // to https://www.cs.helsinki.fi/u/ukkonen/SuffixT1withFigs.pdf for definitions]
    node *active = root, *edge;

    // `lastleaf` and `lastnode` below are used to setup suffix links (`->fail`s) of
    // newly created leaves/nodes. We can avoid if-s in the code because we're never
    // going to use `root->fail->fail` to do something meaningful so we can manipulate
    // it freely
    node *lastleaf = root->fail;

    // pointers to the text:
    // * `active_begin` = how many leaves (open edges) have we inserted so far = the
    //    amount of times we performed `active = active->fail` = the first character
    //    when following the path from `root` to `active`
    // * `active_end` = were active ends in the string we are building the tree from
    // * `done` = we have a tree for `s[:done-1]` we try to add `s[done]` to it
    size_t active_begin = 0, active_end = 0, done = 0;
    const size_t end = s.size();

    // Convenience function
    auto active_offset = [&done, &active_end] { return done - active_end; };

    // We build the tree iteratively
    for (; done <= end; ++done) {

      // Again: we're not using `root->fail->fail` so we can set it to avoid ifs.
      node *lastnode = root->fail;

      // If thats not true we have more than `done` leaves in the tree so the tree for
      // s[:done] is ready.
      while (active_begin <= done) {

        // Find the new `active` node and active `edge`
        edge = active->child(s[active_end]);
        while (edge != nullptr and (edge->right) - (edge->left) <= active_offset()) {
          active = edge;
          active_end += (edge->right) - (edge->left);
          edge = active->child(s[active_end]);
        }

        // Check if `s[done]` is already in the tree in the right place below `active`.
        // Obviously if it's here then it's also there below `active->fail` so we're
        // done constructing the tree up to done
        if (done != end and edge != nullptr and
            s[edge->left + active_offset()] == s[done]) {
          lastnode->fail = active;
          break;
        }

        // `s[done]` is not there so we have to add it in the right place
        node *split = active;
        if (edge != nullptr) {
          // Bad luck we have to insert `s[done]` in the middle of `edge`. So let's
          // split the `edge`!
          split = new node(edge->left, edge->left + active_offset(), active->full_left);
          active->children[s[split->left]] = split;
          split->children[s[split->right]] = edge;
          edge->left = split->right;
        }
        // Ok now we can finally add `s[done]`. Oh and let's set the suffix link of the
        // previously created leaf to the new one. How do we now the new one is the
        // right one, you ask? Well since last time when we added a leaf we only
        // followed one suffix link and (maybe) moved down the tree, so the label of the
        // new leaf has to be the label of the last leaf without the first letter. So
        // indeed it's the correct one. Oh and let's remember the leaf so we can add
        // it's suffix link in the future
        lastleaf = lastleaf->fail = new node(done, end, active_begin);
        if (done == end)
          split->empty_leaf = lastleaf;
        else
          split->children[s[done]] = lastleaf;

        // Similar story with suffix links here!
        // But wait a minute, you say, we reset the `lastnode` every time we increment
        // `done`. How do we set the suffix link for the last node we create before
        // incrementing `done`?
        // It's easy - there are two ways of exiting this loop:
        // (1) - we found `s[done]` in the tree before the whole splitting business. But
        //    there, just before the `break` we set the suffix link so we're covered.
        // (2) - `active_begin` is getting bigger than done. But that means `active_end`
        //    is also as big. And that means `edge` is a nullptr. So in the last pass we
        //    don't create any new nodes and set the `lastnode->fail` correctly.
        lastnode = lastnode->fail = split;

        // Ok now we can say that `[active_begin, end]` is in the tree, so let's move on
        // to the next suffix.
        active_begin++;
        active = active->fail;
      }
    }
    lastleaf->fail = root;
    return root;
  };

  template <typename C2>
  std::optional<std::pair<node *, size_t>> find_node(const C2 &query) {
    node *active = root;
    size_t tree_ptr = active->right;
    for (auto it = std::begin(query); it != std::end(query); it++, tree_ptr++) {
      if (tree_ptr == active->right) {
        active = active->child(*it);
        if (active == nullptr)
          return {};
        tree_ptr = active->left;
      }
      if (the_string[tree_ptr] != *it)
        return {};
    }
    return std::make_pair(active, tree_ptr - active->left);
  }

public:
  SuffixTree(const C &text)
      : the_string(std::begin(text), std::end(text)), root(constructST(the_string)){};
  ~SuffixTree() {
    (root->fail->children).clear();
    delete root->fail;
    delete root;
  };

  template <typename C2> bool find(const C2 &query) { return (bool)find_node(query); }

  bool has_all(node *x = nullptr) {
    if (x == nullptr)
      x = root;
    if (x->fail == nullptr)
      return false;
    if (x->empty_leaf != nullptr)
      if (!has_all(x->empty_leaf))
        return false;
    for (auto it = x->children.begin(); it != x->children.end(); it++) {
      auto res = has_all(it->second);
      if (res == false)
        return res;
    }
    return true;
  }

  std::vector<T_vec> all_suffixes() {
    auto lowest =
        find_node(the_string).value_or(std::pair<node *, size_t>(nullptr, 0)).first;
    std::vector<T_vec> result;
    while (lowest != nullptr and lowest->left != -1) {
      result.emplace_back(the_string.begin() + lowest->full_left, the_string.end());
      lowest = lowest->fail;
    }
    return result;
  }

  template <typename...> friend class GeneralisedSuffixTree;
};
} // namespace libalgo

#endif // LIBALGO_SUFFIX_TREE
