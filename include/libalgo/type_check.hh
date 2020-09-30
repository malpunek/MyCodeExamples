#ifndef LIBALGO_TYPE_CHECK
#define LIBALGO_TYPE_CHECK

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

} // namespace _type_check

#endif // LIBALGO_TYPE_CHECK