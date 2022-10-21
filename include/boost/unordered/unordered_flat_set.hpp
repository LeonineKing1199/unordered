// Copyright (C) 2022 Christian Mazakas
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNORDERED_UNORDERED_FLAT_SET_HPP_INCLUDED
#define BOOST_UNORDERED_UNORDERED_FLAT_SET_HPP_INCLUDED

#include <boost/config.hpp>
#if defined(BOOST_HAS_PRAGMA_ONCE)
#pragma once
#endif

#include <boost/unordered/detail/foa.hpp>
#include <boost/unordered/detail/type_traits.hpp>
#include <boost/unordered/unordered_flat_set_fwd.hpp>

#include <boost/core/allocator_access.hpp>
#include <boost/functional/hash.hpp>

#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <utility>

namespace boost {
  namespace unordered {
    template <class Key, class Hash, class KeyEqual, class Allocator>
    class unordered_flat_set
    {
      struct set_types
      {
        using key_type = Key;
        using init_type = Key;
        using value_type = Key;
        static Key const& extract(value_type const& key) { return key; }
        static Key&& move(value_type& x) { return std::move(x); }
      };

      using table_type = detail::foa::table<set_types, Hash, KeyEqual,
        typename boost::allocator_rebind<Allocator,
          typename set_types::value_type>::type>;

      table_type table_;

      template <class K, class H, class KE, class A, class Pred>
      typename unordered_flat_set<K, H, KE, A>::size_type friend erase_if(
        unordered_flat_set<K, H, KE, A>& set, Pred pred);

    public:
      using key_type = Key;
      using value_type = typename set_types::value_type;
      using init_type = typename set_types::init_type;
      using size_type = std::size_t;
      using difference_type = std::ptrdiff_t;
      using hasher = Hash;
      using key_equal = KeyEqual;
      using allocator_type = Allocator;
      using reference = value_type&;
      using const_reference = value_type const&;
      using iterator = typename table_type::iterator;
      using const_iterator = typename table_type::const_iterator;

      unordered_flat_set() : unordered_flat_set(0) {}

      explicit unordered_flat_set(size_type n, hasher const& h = hasher(),
        key_equal const& pred = key_equal(),
        allocator_type const& a = allocator_type())
          : table_(n, h, pred, a)
      {
      }

      unordered_flat_set(size_type n, allocator_type const& a)
          : unordered_flat_set(n, hasher(), key_equal(), a)
      {
      }

      unordered_flat_set(size_type n, hasher const& h, allocator_type const& a)
          : unordered_flat_set(n, h, key_equal(), a)
      {
      }

      explicit unordered_flat_set(allocator_type const& a)
          : unordered_flat_set(0, a)
      {
      }

      template <class Iterator>
      unordered_flat_set(Iterator first, Iterator last, size_type n = 0,
        hasher const& h = hasher(), key_equal const& pred = key_equal(),
        allocator_type const& a = allocator_type())
          : unordered_flat_set(n, h, pred, a)
      {
        this->insert(first, last);
      }

      template <class InputIt>
      unordered_flat_set(
        InputIt first, InputIt last, size_type n, allocator_type const& a)
          : unordered_flat_set(first, last, n, hasher(), key_equal(), a)
      {
      }

      template <class Iterator>
      unordered_flat_set(Iterator first, Iterator last, size_type n,
        hasher const& h, allocator_type const& a)
          : unordered_flat_set(first, last, n, h, key_equal(), a)
      {
      }

      unordered_flat_set(unordered_flat_set const& other) : table_(other.table_)
      {
      }

      unordered_flat_set(
        unordered_flat_set const& other, allocator_type const& a)
          : table_(other.table_, a)
      {
      }

      unordered_flat_set(unordered_flat_set&& other)
        BOOST_NOEXCEPT_IF(std::is_nothrow_move_constructible<hasher>::value&&
            std::is_nothrow_move_constructible<key_equal>::value&&
              std::is_nothrow_move_constructible<allocator_type>::value)
          : table_(std::move(other.table_))
      {
      }

      unordered_flat_set(unordered_flat_set&& other, allocator_type const& al)
          : table_(std::move(other.table_), al)
      {
      }

      unordered_flat_set(std::initializer_list<value_type> ilist,
        size_type n = 0, hasher const& h = hasher(),
        key_equal const& pred = key_equal(),
        allocator_type const& a = allocator_type())
          : unordered_flat_set(ilist.begin(), ilist.end(), n, h, pred, a)
      {
      }

      unordered_flat_set(std::initializer_list<value_type> init, size_type n,
        allocator_type const& a)
          : unordered_flat_set(init, n, hasher(), key_equal(), a)
      {
      }

      unordered_flat_set(std::initializer_list<value_type> init, size_type n,
        hasher const& h, allocator_type const& a)
          : unordered_flat_set(init, n, h, key_equal(), a)
      {
      }

      ~unordered_flat_set() = default;

      unordered_flat_set& operator=(unordered_flat_set const& other)
      {
        table_ = other.table_;
        return *this;
      }

      unordered_flat_set& operator=(unordered_flat_set&& other) BOOST_NOEXCEPT_IF(
        BOOST_NOEXCEPT_EXPR(std::declval<table_type&>() = std::declval<table_type&&>()))
      {
        table_ = std::move(other.table_);
        return *this;
      }

      allocator_type get_allocator() const BOOST_NOEXCEPT
      {
        return table_.get_allocator();
      }

      /// Iterators
      ///

      iterator begin() BOOST_NOEXCEPT { return table_.begin(); }
      const_iterator begin() const BOOST_NOEXCEPT { return table_.begin(); }
      const_iterator cbegin() const BOOST_NOEXCEPT { return table_.cbegin(); }

      iterator end() BOOST_NOEXCEPT { return table_.end(); }
      const_iterator end() const BOOST_NOEXCEPT { return table_.end(); }
      const_iterator cend() const BOOST_NOEXCEPT { return table_.cend(); }

      /// Capacity
      ///

      BOOST_ATTRIBUTE_NODISCARD bool empty() const BOOST_NOEXCEPT
      {
        return table_.empty();
      }

      size_type size() const BOOST_NOEXCEPT { return table_.size(); }

      size_type max_size() const BOOST_NOEXCEPT { return table_.max_size(); }

      /// Modifiers
      ///

      void clear() BOOST_NOEXCEPT { table_.clear(); }

      std::pair<iterator, bool> insert(value_type const& value)
      {
        return table_.insert(value);
      }

      std::pair<iterator, bool> insert(value_type&& value)
      {
        return table_.insert(std::move(value));
      }

      iterator insert(const_iterator, value_type const& value)
      {
        return table_.insert(value).first;
      }

      iterator insert(const_iterator, value_type&& value)
      {
        return table_.insert(std::move(value)).first;
      }

      template <class InputIterator>
      void insert(InputIterator first, InputIterator last)
      {
        for (auto pos = first; pos != last; ++pos) {
          table_.emplace(*pos);
        }
      }

      void insert(std::initializer_list<value_type> ilist)
      {
        this->insert(ilist.begin(), ilist.end());
      }

      template <class... Args> std::pair<iterator, bool> emplace(Args&&... args)
      {
        return table_.emplace(std::forward<Args>(args)...);
      }

      template <class... Args>
      iterator emplace_hint(const_iterator, Args&&... args)
      {
        return this->emplace(std::forward<Args>(args)...).first;
      }

      void erase(const_iterator pos) { return table_.erase(pos); }
      iterator erase(const_iterator first, const_iterator last)
      {
        while (first != last) {
          this->erase(first++);
        }
        return iterator{detail::foa::const_iterator_cast_tag{}, last};
      }

      size_type erase(key_type const& key) { return table_.erase(key); }

      template <class K>
      typename std::enable_if<
        detail::transparent_non_iterable<K, unordered_flat_set>::value,
        size_type>::type
      erase(K const& key)
      {
        return table_.erase(key);
      }

      void swap(unordered_flat_set& rhs) BOOST_NOEXCEPT_IF(
        BOOST_NOEXCEPT_EXPR(std::declval<table_type&>().swap(std::declval<table_type&>())))
      {
        table_.swap(rhs.table_);
      }

      template <class H2, class P2>
      void merge(unordered_flat_set<key_type, H2, P2, allocator_type>& source)
      {
        table_.merge(source.table_);
      }

      template <class H2, class P2>
      void merge(unordered_flat_set<key_type, H2, P2, allocator_type>&& source)
      {
        table_.merge(std::move(source.table_));
      }

      /// Lookup
      ///

      size_type count(key_type const& key) const
      {
        auto pos = table_.find(key);
        return pos != table_.end() ? 1 : 0;
      }

      template <class K>
      typename std::enable_if<
        detail::are_transparent<K, hasher, key_equal>::value, size_type>::type
      count(K const& key) const
      {
        auto pos = table_.find(key);
        return pos != table_.end() ? 1 : 0;
      }

      BOOST_FORCEINLINE iterator find(key_type const& key)
      {
        return table_.find(key);
      }

      BOOST_FORCEINLINE const_iterator find(key_type const& key) const
      {
        return table_.find(key);
      }

      template <class K>
      BOOST_FORCEINLINE typename std::enable_if<
        boost::unordered::detail::are_transparent<K, hasher, key_equal>::value,
        iterator>::type
      find(K const& key)
      {
        return table_.find(key);
      }

      template <class K>
      BOOST_FORCEINLINE typename std::enable_if<
        boost::unordered::detail::are_transparent<K, hasher, key_equal>::value,
        const_iterator>::type
      find(K const& key) const
      {
        return table_.find(key);
      }

      bool contains(key_type const& key) const
      {
        return this->find(key) != this->end();
      }

      template <class K>
      typename std::enable_if<
        boost::unordered::detail::are_transparent<K, hasher, key_equal>::value,
        bool>::type
      contains(K const& key) const
      {
        return this->find(key) != this->end();
      }

      std::pair<iterator, iterator> equal_range(key_type const& key)
      {
        auto pos = table_.find(key);
        if (pos == table_.end()) {
          return {pos, pos};
        }

        auto next = pos;
        ++next;
        return {pos, next};
      }

      std::pair<const_iterator, const_iterator> equal_range(
        key_type const& key) const
      {
        auto pos = table_.find(key);
        if (pos == table_.end()) {
          return {pos, pos};
        }

        auto next = pos;
        ++next;
        return {pos, next};
      }

      template <class K>
      typename std::enable_if<
        detail::are_transparent<K, hasher, key_equal>::value,
        std::pair<iterator, iterator> >::type
      equal_range(K const& key)
      {
        auto pos = table_.find(key);
        if (pos == table_.end()) {
          return {pos, pos};
        }

        auto next = pos;
        ++next;
        return {pos, next};
      }

      template <class K>
      typename std::enable_if<
        detail::are_transparent<K, hasher, key_equal>::value,
        std::pair<const_iterator, const_iterator> >::type
      equal_range(K const& key) const
      {
        auto pos = table_.find(key);
        if (pos == table_.end()) {
          return {pos, pos};
        }

        auto next = pos;
        ++next;
        return {pos, next};
      }

      /// Hash Policy
      ///

      size_type bucket_count() const BOOST_NOEXCEPT { return table_.capacity(); }

      float load_factor() const BOOST_NOEXCEPT { return table_.load_factor(); }

      float max_load_factor() const BOOST_NOEXCEPT
      {
        return table_.max_load_factor();
      }

      void max_load_factor(float) {}

      void rehash(size_type n) { table_.rehash(n); }

      void reserve(size_type n) { table_.reserve(n); }

      /// Observers
      ///

      hasher hash_function() const { return table_.hash_function(); }

      key_equal key_eq() const { return table_.key_eq(); }
    };

    template <class Key, class Hash, class KeyEqual, class Allocator>
    bool operator==(
      unordered_flat_set<Key, Hash, KeyEqual, Allocator> const& lhs,
      unordered_flat_set<Key, Hash, KeyEqual, Allocator> const& rhs)
    {
      if (&lhs == &rhs) {
        return true;
      }

      return (lhs.size() == rhs.size()) && ([&] {
        for (auto const& key : lhs) {
          auto pos = rhs.find(key);
          if ((pos == rhs.end()) || (key != *pos)) {
            return false;
          }
        }
        return true;
      })();
    }

    template <class Key, class Hash, class KeyEqual, class Allocator>
    bool operator!=(
      unordered_flat_set<Key, Hash, KeyEqual, Allocator> const& lhs,
      unordered_flat_set<Key, Hash, KeyEqual, Allocator> const& rhs)
    {
      return !(lhs == rhs);
    }

    template <class Key, class Hash, class KeyEqual, class Allocator>
    void swap(unordered_flat_set<Key, Hash, KeyEqual, Allocator>& lhs,
      unordered_flat_set<Key, Hash, KeyEqual, Allocator>& rhs)
      BOOST_NOEXCEPT_IF(BOOST_NOEXCEPT_EXPR(lhs.swap(rhs)))
    {
      lhs.swap(rhs);
    }

    template <class Key, class Hash, class KeyEqual, class Allocator,
      class Pred>
    typename unordered_flat_set<Key, Hash, KeyEqual, Allocator>::size_type
    erase_if(unordered_flat_set<Key, Hash, KeyEqual, Allocator>& set, Pred pred)
    {
      return erase_if(set.table_, pred);
    }
  } // namespace unordered
} // namespace boost

#endif
