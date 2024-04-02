#pragma once

#if __cplusplus >= 202002L
#include <concepts>

namespace zero::concepts {

template <typename _T>
concept optional = requires(_T __v) {
  { __v.has_value() } -> std::convertible_to<bool>;
  { __v.reset() };
  { __v.value() } -> std::convertible_to<typename _T::value_type>;
};

template <typename _T, typename _U>
concept deleter_for = optional<_U> && std::invocable<_T, typename _U::value_type&>;

}
#endif
