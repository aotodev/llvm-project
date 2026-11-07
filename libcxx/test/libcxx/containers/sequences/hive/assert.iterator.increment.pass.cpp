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

// Incrementing an iterator that is end(), value-initialized, or on a slot with no element.

#include <hive>
#include <memory>

#include "check_assertion.h"

int main(int, char**) {
  using Storage  = std::__hive_storage<int, std::allocator<int>>;
  using Iterator = Storage::iterator;

  Storage empty{std::allocator<int>()};
  TEST_LIBCPP_ASSERT_FAILURE(++empty.__end(), "hive: incrementing a non-dereferenceable iterator");
  TEST_LIBCPP_ASSERT_FAILURE(Iterator()++, "hive: incrementing a non-dereferenceable iterator");

  Storage s(std::hive_limits(3, 3), std::allocator<int>());
  auto [block, index] = s.__emplace(1);
  TEST_LIBCPP_ASSERT_FAILURE(++s.__end(), "hive: incrementing a non-dereferenceable iterator");
  TEST_LIBCPP_ASSERT_FAILURE(s.__end()++, "hive: incrementing a non-dereferenceable iterator");
  TEST_LIBCPP_ASSERT_FAILURE(++Iterator(block, 2), "hive: incrementing a non-dereferenceable iterator");

  return 0;
}
