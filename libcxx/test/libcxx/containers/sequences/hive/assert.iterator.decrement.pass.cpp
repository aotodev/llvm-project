//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: std-at-least-c++26
// REQUIRES: can-test-hardening-assertions-fast

// <hive>

// Decrementing a value-initialized iterator, or begin(): the first slot of the first block.

#include <hive>
#include <memory>

#include "check_assertion.h"

int main(int, char**) {
  using Storage  = std::__hive_storage<int, std::allocator<int>>;
  using Iterator = Storage::iterator;

  Storage empty{std::allocator<int>()};
  TEST_LIBCPP_ASSERT_FAILURE(--empty.__end(), "hive: decrementing a non-decrementable iterator");
  TEST_LIBCPP_ASSERT_FAILURE(Iterator()--, "hive: decrementing a non-decrementable iterator");

  Storage s(std::hive_limits(3, 3), std::allocator<int>());
  s.__emplace(1);
  TEST_LIBCPP_ASSERT_FAILURE(--s.__begin(), "hive: decrementing a non-decrementable iterator");
  TEST_LIBCPP_ASSERT_FAILURE(s.__begin()--, "hive: decrementing a non-decrementable iterator");

  // Two blocks: the first slot of the first block is still begin().
  for (int n = 0; n != 3; ++n) {
    s.__emplace(n);
  }
  TEST_LIBCPP_ASSERT_FAILURE(--s.__begin(), "hive: decrementing a non-decrementable iterator");

  return 0;
}
