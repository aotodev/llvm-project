//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: std-at-least-c++26

// <hive>

// libc++'s hard limits: a block holds at most 4096 elements.

#include <hive>

struct Big {
  char data[1000];
};

static_assert(std::hive<char>::block_capacity_hard_limits().min == 1);
static_assert(std::hive<char>::block_capacity_hard_limits().max == 4096);
static_assert(std::hive<int>::block_capacity_hard_limits().max == 4096);
static_assert(std::hive<Big>::block_capacity_hard_limits().max == 4096);
