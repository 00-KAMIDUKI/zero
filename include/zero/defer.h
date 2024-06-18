#pragma once

#include <utility>

namespace zero {

template <typename _Func>
struct [[nodiscard("Destructor will be called instantly")]] defer {
private:
  _Func _M_f;
public:
  explicit constexpr defer(_Func __func) noexcept: _M_f(__func) {}
  defer(defer&&) = delete;
#if __cplusplus >= 202002L
  constexpr 
#endif
  ~defer() { std::forward<_Func>(_M_f)(); }
};

}
