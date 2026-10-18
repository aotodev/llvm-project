//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: std-at-least-c++26

// <hive>

// constexpr hive() noexcept(noexcept(Allocator()));
// constexpr explicit hive(const Allocator&) noexcept;

#include <cassert>
#include <hive>
#include <type_traits>

#include "min_allocator.h"
#include "test_allocator.h"
#include "test_macros.h"

template <class H>
void check_empty(const H& h) {
  assert(h.empty());
  assert(h.size() == 0);
  assert(h.capacity() == 0);
  auto l = h.block_capacity_limits();
  auto d = H::block_capacity_default_limits();
  assert(l.min == d.min);
  assert(l.max == d.max);
}

template <class T, class Alloc>
void test() {
  using H = std::hive<T, Alloc>;
  {
    H h;
    check_empty(h);
    assert(h.get_allocator() == Alloc());
  }
  {
    H h = {};
    check_empty(h);
  }
}

int main(int, char**) {
  test<int, std::allocator<int>>();
  test<double, std::allocator<double>>();
  test<int, min_allocator<int>>();
  test<int, test_allocator<int>>();

  {
    using H = std::hive<int, test_allocator<int>>;
    test_allocator_statistics stats;
    H h(test_allocator<int>(42, &stats));
    check_empty(h);
    assert(h.get_allocator().get_data() == 42);
    assert(stats.alloc_count == 0);
  }

  static_assert(std::is_nothrow_default_constructible_v<std::hive<int>>);
  static_assert(std::is_nothrow_constructible_v<std::hive<int>, const std::allocator<int>&>);
  static_assert(!std::is_convertible_v<const std::allocator<int>&, std::hive<int>>);

  return 0;
}
