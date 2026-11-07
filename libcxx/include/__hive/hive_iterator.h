// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___HIVE_HIVE_ITERATOR_H
#define _LIBCPP___HIVE_HIVE_ITERATOR_H

#include <__assert>
#include <__config>
#include <__cstddef/size_t.h>
#include <__hive/hive_bitmap.h>
#include <__hive/hive_block.h>
#include <__iterator/iterator_traits.h>
#include <__memory/pointer_traits.h>
#include <__type_traits/conditional.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

#if _LIBCPP_STD_VER >= 26

_LIBCPP_BEGIN_NAMESPACE_STD

// A block and a slot index: 16 bytes with raw pointers, trivially copyable. end() is {the last block, its capacity},
// or {nullptr, 0} for an empty hive, which is also the value-initialized iterator.
template <class _Tp, class _VoidPtr, bool _Const>
struct _LIBCPP_WARN_UNUSED __hive_iterator {
private:
  using __block_type _LIBCPP_NODEBUG =
      __hive_block<_Tp, _VoidPtr>; // not __block: a keyword in Objective-C++ and under -fblocks
  using __block_pointer _LIBCPP_NODEBUG = typename __block_type::__block_pointer;
  using __bitmap _LIBCPP_NODEBUG        = __hive_bitmap<__block_type>;

public:
  using iterator_category = bidirectional_iterator_tag;
  using value_type        = _Tp;
  using difference_type   = typename pointer_traits<_VoidPtr>::difference_type;
  using reference         = __conditional_t<_Const, const _Tp&, _Tp&>;
  using pointer           = __rebind_pointer_t<_VoidPtr, __conditional_t<_Const, const _Tp, _Tp>>;

  _LIBCPP_HIDE_FROM_ABI constexpr __hive_iterator() noexcept = default;

  _LIBCPP_HIDE_FROM_ABI constexpr __hive_iterator(__block_pointer __b, size_t __i) noexcept
      : __block_(__b), __index_(__i) {}

  template <bool _OtherConst>
    requires(_Const && !_OtherConst)
  _LIBCPP_HIDE_FROM_ABI constexpr __hive_iterator(const __hive_iterator<_Tp, _VoidPtr, _OtherConst>& __other) noexcept
      : __block_(__other.__block_), __index_(__other.__index_) {}

  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr reference operator*() const noexcept {
    _LIBCPP_ASSERT_VALID_ELEMENT_ACCESS(__dereferenceable(), "hive: dereferencing a non-dereferenceable iterator");
    return *(__block_->__slots_ + __index_);
  }

  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr pointer operator->() const noexcept {
    return pointer_traits<pointer>::pointer_to(**this);
  }

  _LIBCPP_HIDE_FROM_ABI constexpr __hive_iterator& operator++() noexcept {
    _LIBCPP_ASSERT_VALID_ELEMENT_ACCESS(__dereferenceable(), "hive: incrementing a non-dereferenceable iterator");

    // The bit at the capacity is 0, so the end of a block fails this test with no capacity compare.
    const size_t __j = __index_ + 1;
    if (__bitmap::__is_live(*__block_, __j)) [[likely]] {
      __index_ = __j;
      return *this;
    }

    __index_ = __bitmap::__next_live(*__block_, __j);
    if (__index_ == __block_->__capacity_ && __block_->__next_ != nullptr) {
      __block_ = __block_->__next_;
      __index_ = __bitmap::__first_live(*__block_);
    }
    return *this;
  }

  _LIBCPP_HIDE_FROM_ABI constexpr __hive_iterator operator++(int) noexcept {
    __hive_iterator __tmp = *this;
    ++*this;
    return __tmp;
  }

  _LIBCPP_HIDE_FROM_ABI constexpr __hive_iterator& operator--() noexcept {
    _LIBCPP_ASSERT_VALID_ELEMENT_ACCESS(__block_ != nullptr, "hive: decrementing a non-decrementable iterator");

    // From end() the index is the capacity, whose bit is 0; from slot 0 only the previous block remains.
    if (__index_ != 0) {
      const size_t __i = __index_ - 1;
      if (__bitmap::__is_live(*__block_, __i)) [[likely]] {
        __index_ = __i;
        return *this;
      }

      __index_ = __bitmap::__prev_live(*__block_, __i);
      if (__index_ != __block_->__capacity_) {
        return *this;
      }
    }

    _LIBCPP_ASSERT_VALID_ELEMENT_ACCESS(
        __block_->__prev_ != nullptr, "hive: decrementing a non-decrementable iterator");
    __block_ = __block_->__prev_;
    __index_ = __bitmap::__last_live(*__block_);
    return *this;
  }

  _LIBCPP_HIDE_FROM_ABI constexpr __hive_iterator operator--(int) noexcept {
    __hive_iterator __tmp = *this;
    --*this;
    return __tmp;
  }

  _LIBCPP_HIDE_FROM_ABI friend constexpr bool
  operator==(const __hive_iterator& __x, const __hive_iterator& __y) noexcept {
    return __x.__block_ == __y.__block_ && __x.__index_ == __y.__index_;
  }

private:
  template <class, class, bool>
  friend struct __hive_iterator;

  // The index and the bit; cannot check that the block belongs to a hive.
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr bool __dereferenceable() const noexcept {
    return __block_ != nullptr && __index_ < __block_->__capacity_ && __bitmap::__is_live(*__block_, __index_);
  }

  __block_pointer __block_ = nullptr;
  size_t __index_          = 0;
};

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP_STD_VER >= 26

_LIBCPP_POP_MACROS

#endif // _LIBCPP___HIVE_HIVE_ITERATOR_H
