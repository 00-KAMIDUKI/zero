#pragma once

#include <zero/expected.h>
#include <zero/unique_handle.h>

namespace zero {

#if __cplusplus >= 202002L
template <std::default_initializable _T, typename __deleter_t>
#else
template <typename _T, typename __deleter_t>
#endif
using unique_handle_zero = unique_handle<expected_zero<_T>, __deleter_t>;


template <typename _T, typename __deleter_t
#if __cplusplus < 202002L
, typename = std::enable_if_t<std::is_invocable_v<__deleter_t, _T&>>
#endif
>
auto make_unique_handle_zero(_T const& __value, __deleter_t&& __deleter) noexcept {
  return unique_handle_zero<_T, __deleter_t>{expected_zero<_T>{__value}, std::forward<__deleter_t>(__deleter)};
}

}

