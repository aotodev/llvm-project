//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: std-at-least-c++26

// <hive>

// constexpr hive_limits block_capacity_limits() const noexcept;
// static constexpr hive_limits block_capacity_default_limits() noexcept;
// static constexpr hive_limits block_capacity_hard_limits() noexcept;
// static constexpr bool is_within_hard_limits(hive_limits) noexcept;

#include <cassert>
#include <cstddef>
#include <hive>
#include <memory>
#include <type_traits>
#include <utility>

#include "min_allocator.h"
#include "test_macros.h"

template <class H>
constexpr bool test() {
  ASSERT_NOEXCEPT(H::block_capacity_default_limits());
  ASSERT_NOEXCEPT(H::block_capacity_hard_limits());
  ASSERT_NOEXCEPT(H::is_within_hard_limits(std::hive_limits(1, 1)));
  ASSERT_NOEXCEPT(std::declval<const H&>().block_capacity_limits());
  ASSERT_SAME_TYPE(decltype(H::block_capacity_default_limits()), std::hive_limits);
  ASSERT_SAME_TYPE(decltype(H::block_capacity_hard_limits()), std::hive_limits);
  ASSERT_SAME_TYPE(decltype(H::is_within_hard_limits(std::hive_limits(1, 1))), bool);
  ASSERT_SAME_TYPE(decltype(std::declval<const H&>().block_capacity_limits()), std::hive_limits);

  constexpr std::hive_limits hard = H::block_capacity_hard_limits();
  constexpr std::hive_limits def  = H::block_capacity_default_limits();

  // The hard limits bound the defaults.
  assert(hard.min <= hard.max);
  assert(def.min <= def.max);
  assert(hard.min <= def.min);
  assert(def.max <= hard.max);
  assert(H::is_within_hard_limits(hard));
  assert(H::is_within_hard_limits(def));

  // [hive.overview]: the maximum hard limit is no larger than allocator_traits<Allocator>::max_size().
  using A = typename H::allocator_type;
  assert(hard.max <= std::allocator_traits<A>::max_size(A()));

  // Outside the hard limits, or min > max.
  assert(!H::is_within_hard_limits(std::hive_limits(hard.max, hard.min)) || hard.min == hard.max);
  assert(!H::is_within_hard_limits(std::hive_limits(hard.min, hard.max + 1)));
  if (hard.min > 0)
    assert(!H::is_within_hard_limits(std::hive_limits(hard.min - 1, hard.max)));
  if (hard.min < hard.max)
    assert(!H::is_within_hard_limits(std::hive_limits(hard.min + 1, hard.min)));

  return true;
}

struct Big {
  char data[1000];
};

int main(int, char**) {
  test<std::hive<int>>();
  test<std::hive<char>>();
  test<std::hive<Big>>();
  test<std::hive<int, min_allocator<int>>>();
  static_assert(test<std::hive<int>>());
  static_assert(test<std::hive<char>>());
  static_assert(test<std::hive<Big>>());

  return 0;
}
