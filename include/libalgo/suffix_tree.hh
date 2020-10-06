// Stanislaw Morawski
//
// Implementation of Ukkonen's algorithm for online linear construction of
// suffix trees
//
// Assuming input is a word over alphabet (a.....z)

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace libalgo {

template <typename C,
          typename T = std::decay_t<decltype(*std::begin(std::declval<C>()))>>
struct inner_type {
  using type = T;
};

template <typename C> using inner_type_t = typename inner_type<C>::type;

template <typename C, typename...> struct first_inner_type {
  using type = typename inner_type<C>::type;
};

template <typename... Cs> class SuffixTree {

public:
  using T = typename first_inner_type<Cs...>::type;
  using T_ptr = T *;
  using T_vec = std::vector<T>;
  T_vec the_string;
  T separator;

private:
  struct node {
    std::unordered_map<T, node *> child;
    size_t left, right;
    node *fail = nullptr;
    node(int left, int right) : left(left), right(right){};
  };
  node * root, * root_fail;

  node * constructST(const T_vec &s) {
    root = new node(-1, 0);
    root_fail = new node(0, 0);
    root->fail = root_fail;

    {
      std::unordered_set<T> alphabet(s.begin(), s.end());
      for (const auto &i : alphabet)
        (root_fail)->child[i] = root;
    }

    node *active = root;
    node *edge;
    node *lastleaf = root_fail;
    size_t begin = 0, head = 0, tail = 0;
    const size_t end = s.size();

    for (size_t done = begin; done < end; ++done) {
      node *lastnode = root->fail;
      while (head <= done) {
        edge = active->child[s[tail]];
        while (edge != NULL and (edge->right) - (edge->left) <= (done - tail)) {
          active = edge;
          tail += (edge->right) - (edge->left);
          edge = active->child[s[tail]];
        }
        if (edge != NULL and s[edge->left + done - tail] == s[done]) {
          lastnode->fail = active;
          break;
        }
        node *divide = active;
        if (edge != NULL) {
          divide = new node(edge->left, edge->left + done - tail);
          active->child[s[divide->left]] = divide;
          divide->child[s[divide->right]] = edge;
          edge->left = divide->right;
        }
        lastleaf = lastleaf->fail = divide->child[s[done]] =
            new node(done, end);
        lastnode = lastnode->fail = divide;
        head++;
        active = active->fail;
      }
    }
    return root;
  }

  template <typename C> T_vec make_internal_text(const C &text) {
    T_vec result(std::begin(text), std::end(text));
    result.push_back(separator);
    return result;
  };

  void empty_func(...) { return; }

  template <typename... Css> T_vec join(const Css &... rest) {
    T_vec result;
    // TODO: this can probably be done in a more elegant manner
    empty_func(
        result.insert(result.end(), std::begin(rest), std::end(rest))...);
    return result;
  };

public:
  SuffixTree(T special_token, const Cs &... texts) : separator(special_token) {
    static_assert(std::conjunction_v<std::is_convertible<
                      inner_type_t<std::remove_reference_t<Cs>>, T>...>,
                  "All values have to be convertible to the same type");
    the_string = join(make_internal_text((texts))...);
    root = constructST(the_string);
  };

  ~SuffixTree(){
    // TODO release memory XD, or better convert to smart pointers
  };

  // TODO implement those
  // template <typename C> bool is_substring(C pattern);
  // template <typename C> size_t find_first(C pattern);
  // template <typename C> std::vector<size_t> find_all(C pattern);
};

template <> class SuffixTree<> : std::false_type {};

} // namespace libalgo