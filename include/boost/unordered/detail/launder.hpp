#ifndef BOOST_UNORDERED_DETAIL_LAUNDER_HPP_
#define BOOST_UNORDERED_DETAIL_LAUNDER_HPP_

#include <new>

#include <boost/config.hpp>

namespace boost {
  namespace unordered {
    namespace detail {

#if defined(_GLIBCXX_HAVE_BUILTIN_LAUNDER)
      template <class T> T* launder(T* p) BOOST_NOEXCEPT
      {
        return __builtin_launder(p);
      }
#else
      template <class T> T* launder(T* p) BOOST_NOEXCEPT { return p; }
#endif
    } // namespace detail
  }   // namespace unordered
} // namespace boost

#endif
