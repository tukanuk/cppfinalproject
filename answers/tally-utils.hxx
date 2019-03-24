// Copyright (c) Paul Preney 2019. All Rights Reserved.
// For use by students in the University of Windsor's 2019W COMP3400 course.

#ifndef tally_utils_hxx_
#define tally_utils_hxx_

//===========================================================================

#include <atomic>
#include <limits>
#include <thread>
#include <cstddef>
#include <unordered_map>

//===========================================================================

struct tally_nothing
{
  template <typename T>
  constexpr tally_nothing const& operator ()(T const&) const noexcept
  {
    return *this;
  }
};

//===========================================================================

class tally_count
{
public:
  using size_type = std::size_t;

private:
  size_type n_;

public:
  constexpr tally_count() noexcept :
    n_{}
  {
  }

  constexpr tally_count(tally_count const& t) noexcept = default;
  constexpr tally_count(tally_count&& t) noexcept = default;
  constexpr tally_count& operator =(tally_count const& t) noexcept = default;
  constexpr tally_count& operator =(tally_count&& t) noexcept = default;

  constexpr size_type const& count() const noexcept
  {
    return n_;
  }

  template <typename T>
  constexpr tally_count& operator ()(T const&) noexcept
  {
    ++n_;
    return *this;
  }
};

//===========================================================================

template <typename T>
class tally_mean_variance
{
public:
  using value_type = T;
  using size_type = std::size_t;

private:
  size_type n_;
  value_type mean_;
  value_type variance_;
  
public:
  constexpr tally_mean_variance() :
    n_{},
    mean_{},
    variance_{}
  {
  }
  constexpr tally_mean_variance(tally_mean_variance const& t) = default;
  constexpr tally_mean_variance(tally_mean_variance&& t) = default;
  constexpr tally_mean_variance& operator =(tally_mean_variance const& t) = default;
  constexpr tally_mean_variance& operator =(tally_mean_variance&& t) = default;

  constexpr size_type const& count() const
  {
    return n_;
  }

  constexpr value_type const& mean() const
  {
    return mean_;
  }

  constexpr value_type variance() const
  {
    if (1 < n_)
      return variance_;

    if constexpr(std::numeric_limits<T>::has_quiet_NaN)
      return std::numeric_limits<T>::quiet_NaN();
    else if constexpr(std::numeric_limits<T>::has_signaling_NaN)
      return std::numeric_limits<T>::signaling_NaN();
    else if constexpr(std::numeric_limits<T>::is_bounded)
      // max() is technically incorrect but there is no NaN...
      return std::numeric_limits<T>::max();
    else
      // zero is technically incorrect but there is no NaN or max()...
      return T(0);
  }

  // Using Knuth's recurrence relation from The Art of Computer Programming
  // 3rd. ed., Vol. 2, Seminumerical Algorithms, Section 4.2.2.
  constexpr tally_mean_variance& operator ()(T const& t)
  {
    ++n_;
    value_type const delta = t - mean_;
    mean_ += delta / n_;
    variance_ += delta * (t - mean_);
    return *this;
  }
};

//===========================================================================

template <typename Tally>
class tally_within_thread_id
{
public:
  using tally_type = Tally;
  using tally_map_type = std::unordered_map<std::thread::id, tally_type>;

private:
  tally_map_type tally_map_;

public:
  tally_within_thread_id() = default;
  tally_within_thread_id(tally_within_thread_id const& t) = default;
  tally_within_thread_id(tally_within_thread_id&& t) = default;
  tally_within_thread_id& operator =(tally_within_thread_id const& t) = default;
  tally_within_thread_id& operator =(tally_within_thread_id&& t) = default;

  constexpr tally_map_type const& get() const noexcept
  {
    return tally_map_;
  }
  
  template <typename T>
  tally_within_thread_id& operator ()(T&& t)
  {
    tally_map_[std::this_thread::get_id()](std::move(t));
    return *this;
  }

  template <typename T>
  tally_within_thread_id& operator ()(T const& t)
  {
    tally_map_[std::this_thread::get_id()](t);
    return *this;
  }
};

//===========================================================================

#endif // #ifndef tally_utils_hxx_
