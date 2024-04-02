#pragma once

#include <zero/expected.h>
#include <zero/unique_handle.h>

namespace zero {

#if __cplusplus >= 202002L
template <typename _T, typename __deleter_t>
#else
template <typename _T, typename __deleter_t>
#endif
struct unique_handle_zero: unique_handle<expected_zero<_T>, __deleter_t> {
  template <typename... _Args>
  unique_handle_zero(_Args&&... __args) noexcept(std::is_nothrow_constructible_v<unique_handle<expected_zero<_T>, __deleter_t>, _Args...>)
    :unique_handle<expected_zero<_T>, __deleter_t>{std::forward<_Args>(__args)...} {}
};

template <typename _T, typename __deleter_t>
unique_handle_zero(_T, __deleter_t) -> unique_handle_zero<_T, __deleter_t>;


// #if __cplusplus >= 202002L
// template <typename _T, typename __deleter_t>
// requires std::default_initializable<__deleter_t> && (!std::is_pointer_v<__deleter_t>)
// #else
// template <typename _T
// , typename __deleter_t
// , typename = std::enable_if_t<std::is_default_constructible_v<__deleter_t> && (!std::is_pointer_v<__deleter_t>)>
// >
// #endif
// auto make_unique_handle_zero(auto&&... args) noexcept {
//   
// }

}

// using t = zero::unique_handle_zero<int, decltype([](int) {})>;
