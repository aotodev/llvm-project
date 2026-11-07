//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: std-at-least-c++26

// <hive>

// libc++'s hard and default limits: a block holds at most 4096 elements, and by default at most 128 KB of them.

#include <cstddef>
#include <hive>
#include <limits>

struct Big {
  char data[1000];
};

static_assert(std::hive<char>::block_capacity_hard_limits().min == 1);
static_assert(std::hive<char>::block_capacity_hard_limits().max == 4096);
static_assert(std::hive<int>::block_capacity_hard_limits().max == 4096);
static_assert(std::hive<Big>::block_capacity_hard_limits().max == 4096);

// SIZE_MAX is a multiple of 17, so 17 elements fill size_t exactly and leave no room for the words.
static_assert(std::numeric_limits<std::size_t>::max() % 17 == 0);
struct Huge {
  char data[std::numeric_limits<std::size_t>::max() / 17];
};

static_assert(std::hive<Huge>::block_capacity_hard_limits().max == 16);
static_assert(std::hive<Huge>::block_capacity_default_limits().min == 16);
static_assert(std::hive<Huge>::block_capacity_default_limits().max == 16);

struct alignas(64) Line {
  char data[64];
};

static_assert(std::hive<char>::block_capacity_default_limits().min == 64);
static_assert(std::hive<char>::block_capacity_default_limits().max == 4096);
static_assert(std::hive<long long>::block_capacity_default_limits().max == 4096);
static_assert(std::hive<Line>::block_capacity_default_limits().max == 2048);
static_assert(std::hive<Big>::block_capacity_default_limits().min == 64);
static_assert(std::hive<Big>::block_capacity_default_limits().max == 131);
