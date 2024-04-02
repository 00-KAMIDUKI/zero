#pragma once
#include <tuple>
#include <initializer_list>
#include <exception>
#if __cplusplus >= 202002L
#include <concepts>
#endif

namespace zero {

struct bad_expected_access: std::exception {
  const char* what() const noexcept override
  { return "bad_optional_access"; }
};

namespace __details {

template <auto... __zero>
struct __default_has_value {
  template <typename _T>
  constexpr auto operator()(_T const& __value) const noexcept
    -> bool { return ((__value != __zero) && ...); }
};

template <auto... __zero>
struct __default_reset {
  template <typename _T>
  constexpr void operator()(_T& __value) const noexcept
  { __value = std::get<0>(std::tuple{__zero...}); }
};

template <auto... __zero>
struct __default_construct {
  constexpr auto operator()() const noexcept
  { return std::get<0>(std::tuple{__zero...}); }
};

}

template <typename _T
, typename __has_value_t
, typename __reset_t
, typename __construct_t = void
> struct expected {

  using value_type = _T;

  static_assert(!std::is_same_v<std::remove_cv_t<_T>, std::in_place_t>);
  static_assert(std::is_object_v<_T> && !std::is_array_v<_T>);

  constexpr expected() noexcept
#if __cplusplus >= 202002L
  requires std::is_invocable_r_v<value_type, __construct_t>
#endif
    :_M_tuple{__construct_t{}(), __has_value_t{}, __reset_t{}} {}

  constexpr expected(
    value_type const& __value,
    __has_value_t const& __has_value = __has_value_t{},
    __reset_t const& __reset = __reset_t{}
  ) noexcept: _M_tuple{__value, __has_value, __reset} {}
    
  constexpr expected(
    value_type&& __value,
    __has_value_t const& __has_value = __has_value_t{},
    __reset_t const& __reset = __reset_t{}
  ) noexcept: _M_tuple{std::move(__value), __has_value, __reset} {}

  template <typename... _Args
  , typename = std::enable_if_t<std::is_constructible_v<value_type, _Args...>>
  > constexpr expected(
    std::in_place_t,
    _Args&&... __args
  ) noexcept(std::is_nothrow_constructible_v<value_type, _Args...>)
    : _M_tuple{value_type{}, __has_value_t{}, __reset_t{}} {}

  template <typename _U
  , typename... _Args
  , typename = std::enable_if_t<std::is_constructible_v<value_type, std::initializer_list<_U>&, _Args...>>
  > constexpr expected(
    std::in_place_t,
    std::initializer_list<_U> __il,
    _Args&&... __args
  ) noexcept(std::is_nothrow_constructible_v<value_type, std::initializer_list<_U>&, _Args...>)
    : _M_tuple{value_type{__il, std::forward<_Args>(__args)...}, __has_value_t{}, __reset_t{}} {}

  template <typename... _Args
  , typename = std::enable_if_t<std::is_constructible_v<value_type, _Args...>>
  > constexpr auto& emplace(_Args&&... __args)
  noexcept(std::is_nothrow_constructible_v<value_type, _Args...>)
  { return std::get<0>(_M_tuple) = value_type{std::forward<_Args>(__args)...}, *this; }

  template <typename _U
  , typename... _Args
  , typename = std::enable_if_t<std::is_constructible_v<value_type, std::initializer_list<_U>&, _Args...>>
  > constexpr auto& emplace(std::initializer_list<_U> __il, _Args&&... __args)
  noexcept(std::is_nothrow_constructible_v<value_type, std::initializer_list<_U>&, _Args...>)
  { return std::get<0>(_M_tuple) = value_type{__il, std::forward<_Args>(__args)...}, *this; }

  constexpr auto has_value() const noexcept -> bool { 
    if constexpr (std::is_member_function_pointer_v<decltype(std::get<1>(_M_tuple))>) {
      return decltype(std::get<0>(_M_tuple).*std::get<1>(_M_tuple))();
    } else {
      return std::get<1>(_M_tuple)(std::get<0>(_M_tuple));
    }
  }

  constexpr explicit operator bool() const noexcept
    { return has_value(); }

  constexpr void reset() noexcept {
    if constexpr (std::is_member_function_pointer_v<decltype(std::get<2>(_M_tuple))>) {
      std::get<0>(_M_tuple).*std::get<2>(_M_tuple)();
    } else {
      std::get<2>(_M_tuple)(std::get<0>(_M_tuple));
    }
  }

  constexpr auto operator*() const& noexcept
    -> value_type const& { return std::get<0>(_M_tuple); }

  constexpr auto operator*() & noexcept
    -> value_type& { return std::get<0>(_M_tuple); }

  constexpr auto operator*() const&& noexcept
    -> value_type const&& { return std::move(std::get<0>(_M_tuple)); }

  constexpr auto operator*() && noexcept
    -> value_type&& { return std::move(std::get<0>(_M_tuple)); }

  constexpr auto operator->() const noexcept
    -> value_type const* { return std::addressof(std::get<0>(_M_tuple)); }

  constexpr auto operator->() noexcept
    -> value_type* { return std::addressof(std::get<0>(_M_tuple)); }

  constexpr auto value() const&
    -> value_type const& { if (has_value()) return std::get<0>(_M_tuple); throw bad_expected_access{}; }

  constexpr auto value() &
    -> value_type& { if (has_value()) return std::get<0>(_M_tuple); throw bad_expected_access{}; }

  constexpr auto value() const&&
    -> value_type const&& { if (has_value()) return std::move(std::get<0>(_M_tuple)); throw bad_expected_access{}; }

  constexpr auto value() &&
    -> value_type&& { if (has_value()) return std::move(std::get<0>(_M_tuple)); throw bad_expected_access{}; }

  constexpr auto error() const&
    -> value_type const& { return **this; }

  constexpr auto error() &
    -> value_type& { return **this; }

  constexpr auto error() const&&
    -> value_type const&& { return std::move(**this); }

  constexpr auto error() &&
    -> value_type&& { return std::move(**this); }

private:
  std::tuple<value_type, __has_value_t, __reset_t> _M_tuple;
};

template <auto... __zeros>
using expected_zeros = expected<std::common_type_t<decltype(__zeros)...>
, __details::__default_has_value<__zeros...>
, __details::__default_reset<__zeros...>
, __details::__default_construct<__zeros...>
>;

#if __cplusplus >= 202002L
template <std::default_initializable _T>
#else
template <typename _T>
#endif
using expected_zero = expected<_T
, __details::__default_has_value<_T{}>
, __details::__default_reset<_T{}>
, __details::__default_construct<_T{}>
>;

template <typename __has_value_t, typename __reset_t, typename _T>
constexpr auto make_expected(_T&& __value) noexcept
{ return expected<std::decay_t<_T>, __has_value_t, __reset_t>{std::forward<_T>(__value)}; }

template <typename __has_value_t
, typename __reset_t
, typename... _Args
, typename _T
, typename = std::enable_if_t<std::is_constructible_v<_T, _Args...>>
> constexpr auto make_expected(_Args&&... __args)
noexcept(std::is_nothrow_constructible_v<_T, _Args...>)
{ return expected<_T, __has_value_t, __reset_t>{std::in_place, _T{std::forward<_Args>(__args)...}}; }

template <typename __has_value_t
, typename __reset_t
, typename _T
, typename _U
, typename... _Args
, typename = std::enable_if_t<std::is_constructible_v<_T, std::initializer_list<_U>&, _Args...>>
> constexpr auto make_expected(std::initializer_list<_U> __il, _Args&&... __args)
noexcept(std::is_nothrow_constructible_v<_T, std::initializer_list<_U>&, _Args...>)
{ return expected<_T, __has_value_t, __reset_t>{std::in_place, __il, std::forward<_Args>(__args)...}; }

}
//
// inline auto f1() {
//   struct has_value {
//     auto operator()(int x) const noexcept -> bool { return x != 0; }
//   };
//
//   struct reset {
//     auto operator()(int& x) const noexcept { x = 0; }
//   };
//
//   auto x = zero::make_expected<has_value, reset>(1);
//   static_assert(sizeof x == sizeof(int));
//   x.emplace();
//
//   static constexpr int i = 0;
//   constexpr auto& ref = i;
//   auto y = zero::expected_zeros<ref>();
//   static_assert(sizeof y == sizeof(int));
//   y.reset();
//
//   constexpr auto z = zero::expected_zero<signed char>{};
//   static_assert(sizeof z == sizeof(signed char));
//   static_assert(!z.has_value());
// }
//
// inline auto f2() {
//   struct foo {
//     int x;
//     auto has_value1() const noexcept -> bool { return x != 0; }
//     auto reset1() noexcept { x = 0; }
//   };
//
//   auto x = zero::expected{foo{1}, &foo::has_value1, &foo::reset1};
// }
//
// inline auto f3() {
//   zero::expected_zero<int> x{};
// }
