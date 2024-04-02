#pragma once

#include <zero/expected.h>
#include <zero/unique_handle.h>
#include <zero/concepts.h>

namespace zero {

#if __cplusplus >= 202002L
template <typename _T, concepts::deleter_for<_T> __deleter_t>
#else
template <typename _T, typename __deleter_t>
#endif
using unique_handle_zero = unique_handle<expected_zero<_T>, __deleter_t>;


// #if __cplusplus >= 202002L
// template <std::default_initializable _T, concepts::deleter_for<_T> __deleter_t>
// #else
// template <typename _T, typename __deleter_t
// , typename = std::enable_if_t<std::is_invocable_v<__deleter_t, _T&>>
// #endif
// >
// constexpr auto make_unique_handle_zero(_T const& __value, __deleter_t&& __deleter) noexcept {
//   return unique_handle_zero<_T, __deleter_t>{expected_zero<_T>{__value}, std::forward<__deleter_t>(__deleter)};
// }
//
// #if __cplusplus >= 202002L
// template <std::default_initializable _T, concepts::deleter_for<_T> __deleter_t>
// #else
// template <typename _T, typename __deleter_t
// , typename = std::enable_if_t<std::is_invocable_v<__deleter_t, _T&>>
// #endif
// >
// constexpr auto make_unique_handle_zero(_T&& __value, __deleter_t&& __deleter) noexcept {
//   return unique_handle_zero<_T, __deleter_t>{expected_zero<_T>{std::move(__value)}, std::forward<__deleter_t>(__deleter)};
// }

// #if __cplusplus >= 202002L
// template <typename _T, concepts::deleter_for<_T> __deleter_t>
// constexpr auto make_unique_handle_zero(auto&&... __args)
// noexcept(std::is_nothrow_constructible_v<_T, decltype(__args)...>)
// #else
// template <typename _T
// , typename __deleter_t
// , typename... _Args
// // , typename = std::enable_if_t<std::is_constructible_v<_T, _Args...>>
// > constexpr auto make_unique_handle_zero(_Args&&... __args)
// noexcept(std::is_nothrow_constructible_v<_T, _Args...>)
// // #endif
// { return unique_handle_zero<_T, __deleter_t>{expected_zero<_T>{std::forward<decltype(__args)>(__args)...}}; }

}

