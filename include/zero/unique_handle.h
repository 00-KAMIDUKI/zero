#pragma once

#include <initializer_list>
#include <tuple>

#include <zero/expected.h>
#if __cplusplus >= 202002L
#include <zero/concepts.h>
#endif

namespace zero {

#if __cplusplus >= 202002L
template <zero::concepts::optional __optional_t
, zero::concepts::deleter_for<__optional_t> __deleter_t
>
#else
template <typename __optional_t, typename __deleter_t>
#endif
struct [[nodiscard("Destructor will be called instantly")]] unique_handle {
  using value_type = typename __optional_t::value_type;

  constexpr unique_handle() noexcept = default;

  constexpr unique_handle(__optional_t const& __value)
  noexcept(std::is_nothrow_constructible_v<__deleter_t>)
    :_M_tuple{__value, __deleter_t{}} {}

  constexpr unique_handle(__optional_t&& __value)
  noexcept(std::is_nothrow_constructible_v<__deleter_t>)
    :_M_tuple{std::move(__value), __deleter_t{}} {}

  template <typename _Deleter>
  constexpr unique_handle(__optional_t const& __value, _Deleter&& __deleter) noexcept
    :_M_tuple{__value, std::forward<_Deleter>(__deleter)} {}
    
  template <typename _Deleter>
  constexpr unique_handle(__optional_t&& __value, _Deleter&& __deleter) noexcept
    :_M_tuple{std::move(__value), std::forward<_Deleter>(__deleter)} {}

  template <typename... _Args
  , typename = std::enable_if_t<std::is_constructible_v<value_type, _Args...>>
  > constexpr unique_handle(std::in_place_t, _Args&&... __args)
  noexcept(std::is_nothrow_constructible_v<value_type, _Args...>)
    :_M_tuple{__optional_t{std::in_place, std::forward<_Args>(__args)...}, __deleter_t{}} {}

  template <typename _U
  , typename... _Args
  , typename = std::enable_if_t<std::is_constructible_v<value_type, std::initializer_list<_U>&, _Args...>>
  > constexpr unique_handle(std::in_place_t, std::initializer_list<_U> __il, _Args&&... __args)
  noexcept(std::is_nothrow_constructible_v<value_type, std::initializer_list<_U>&, _Args...>)
    :_M_tuple{__optional_t{std::in_place, __il, std::forward<_Args>(__args)...}, __deleter_t{}} {}

  constexpr unique_handle(unique_handle&& __other) noexcept
    :_M_tuple{std::move(__other._M_tuple)}
  { __other._M_value_wrapper().reset(); }

  constexpr auto operator=(unique_handle&& __other) noexcept -> unique_handle& {
    if (this != std::addressof(__other)) {
      this->_M_tuple = std::move(__other._M_tuple);
      __other._M_value_wrapper().reset();
    }
    return *this;
  }

#if __cplusplus >= 202002L
  constexpr 
#endif
  ~unique_handle() {
    if (_M_value_wrapper().has_value()) {
      _M_deleter()(*_M_value_wrapper());
    }
    _M_value_wrapper().reset();
  }

  constexpr auto get() const noexcept
    -> value_type const& { return *_M_value_wrapper(); }

  constexpr auto has_value() const noexcept
    -> bool { return _M_value_wrapper().has_value(); }

  explicit constexpr operator bool() const noexcept { return has_value(); }

  constexpr void leak() noexcept { _M_value_wrapper().reset(); }

  constexpr auto operator==(unique_handle const& __other) const noexcept
    -> bool { return _M_value_wrapper() == __other._M_value_wrapper(); }

private:
  std::tuple<__optional_t, __deleter_t> _M_tuple;

  constexpr auto& _M_value_wrapper() noexcept { return std::get<0>(_M_tuple); }
  constexpr auto const& _M_value_wrapper() const noexcept { return std::get<0>(_M_tuple); }
  constexpr auto& _M_deleter() noexcept { return std::get<1>(_M_tuple); }
  constexpr auto const& _M_deleter() const noexcept { return std::get<1>(_M_tuple); }
};

}

// #include <zero/expected.h>
// inline auto f() {
//   auto i = zero::unique_handle<zero::expected_zero<int>, void(*)(int)>{};
//
//   // template <typename _T, typename __deleter_t>
//   using unique_handle_zero = zero::unique_handle<zero::expected_zero<int>, void(*)(int)>;
// }
