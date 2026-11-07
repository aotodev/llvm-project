//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: std-at-least-c++26

// <hive>

// ~hive() is not constexpr: a hive can be constant-initialized, but cannot be a constexpr variable or be destroyed
// during constant evaluation.

#include <hive>

constinit std::hive<int> a;
constinit std::hive<int> b(std::hive_limits(64, 128));

constexpr std::hive<int> c; // expected-error {{constexpr variable cannot have non-literal type}}

constexpr bool destroyed_in_constant_evaluation() {
  std::hive<int> h;
  return h.block_capacity_limits().min == 64;
}
static_assert(destroyed_in_constant_evaluation()); // expected-error {{not an integral constant expression}}
