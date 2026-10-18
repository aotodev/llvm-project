//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: std-at-least-c++26

// <hive>

// constexpr explicit hive(hive_limits block_limits);
// constexpr hive(hive_limits block_limits, const Allocator&);

#include <cassert>
#include <hive>
#include <type_traits>

#include "min_allocator.h"
#include "test_allocator.h"
#include "test_macros.h"

template <class H>
void check(const H& h, std::hive_limits expected) {
  assert(h.empty());
  assert(h.size() == 0);
  assert(h.capacity() == 0);
  auto l = h.block_capacity_limits();
  assert(l.min == expected.min);
  assert(l.max == expected.max);
}

template <class T, class Alloc>
void test() {
  using H     = std::hive<T, Alloc>;
  auto hard   = H::block_capacity_hard_limits();
  auto narrow = std::hive_limits(hard.min, hard.min);
  auto wide   = std::hive_limits(hard.min, hard.max);
  auto mid    = std::hive_limits(hard.min + (hard.max - hard.min) / 2, hard.max);

  for (auto l : {narrow, wide, mid}) {
    {
      H h(l);
      check(h, l);
    }
    {
      H h(l, Alloc());
      check(h, l);
      assert(h.get_allocator() == Alloc());
    }
  }
}

int main(int, char**) {
  test<int, std::allocator<int>>();
  test<char, std::allocator<char>>();
  test<int, min_allocator<int>>();
  test<int, test_allocator<int>>();

  {
    using H = std::hive<int, test_allocator<int>>;
    auto l  = H::block_capacity_default_limits();
    H h(l, test_allocator<int>(7));
    check(h, l);
    assert(h.get_allocator().get_data() == 7);
  }

  static_assert(!std::is_convertible_v<std::hive_limits, std::hive<int>>);
  static_assert(std::is_constructible_v<std::hive<int>, std::hive_limits, const std::allocator<int>&>);

  return 0;
}
