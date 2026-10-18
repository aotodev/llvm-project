//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: std-at-least-c++26
// REQUIRES: can-test-hardening-assertions-extensive

// <hive>

// Constructing a hive with limits outside the hard limits, or with min > max.

#include <hive>

#include "check_assertion.h"

int main(int, char**) {
  using H   = std::hive<int>;
  auto hard = H::block_capacity_hard_limits();

  TEST_LIBCPP_ASSERT_FAILURE(H(std::hive_limits(0, hard.max)), "hive: block limits are not within the hard limits");
  TEST_LIBCPP_ASSERT_FAILURE(
      H(std::hive_limits(hard.min, hard.max + 1)), "hive: block limits are not within the hard limits");
  TEST_LIBCPP_ASSERT_FAILURE(H(std::hive_limits(8, 4)), "hive: block limits are not within the hard limits");
  TEST_LIBCPP_ASSERT_FAILURE(
      H(std::hive_limits(8, 4), std::allocator<int>()), "hive: block limits are not within the hard limits");

  return 0;
}
