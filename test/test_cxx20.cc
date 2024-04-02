#include <cassert>
#include <zero/unique_handle.h>
#include <zero/unique_handle_zero.h>

int main() {
  static auto dtor_called_count = 0;
  {
    auto i = zero::make_unique_handle_zero(0, [](int) { ++dtor_called_count; });
  }
  assert(dtor_called_count == 0);

  {
    auto i = zero::make_unique_handle_zero(1, [](int) { ++dtor_called_count; });
  }
  assert(dtor_called_count == 1);

  {
    auto i = zero::make_unique_handle_zero<int>(1, [](int) { ++dtor_called_count; });
    auto j = std::move(i);
    assert(i.get() == 0);
    assert(j.get() == 1);
  }
  assert(dtor_called_count == 2);
}
