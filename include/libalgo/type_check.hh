#ifndef LIBALGO_TYPE_CHECK
#define LIBALGO_TYPE_CHECK

#include <iterator>
#include <type_traits>

namespace _type_check {

template <typename O, typename T> class HasOperator {

  template <typename Op, typename TT>
  static auto test(int)
      -> decltype(std::declval<Op>()(std::declval<TT>(), std::declval<TT>()),
                  std::true_type());

  template <typename, typename> static auto test(...) -> std::false_type;

public:
  using type = decltype(test<O, T>(0));
};

template <typename C,
          typename T = std::decay_t<decltype(*std::begin(std::declval<C>()))>>
struct inner_type {
  using type = T;
};
template <typename C> using inner_type_t = typename inner_type<C>::type;

template <typename C, typename...> struct first_inner_type {
  using type = typename inner_type<C>::type;
};

} // namespace _type_check

#endif // LIBALGO_TYPE_CHECK