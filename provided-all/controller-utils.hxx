// Copyright (c) Paul Preney 2019. All Rights Reserved.
// For use by students in the University of Windsor's 2019W COMP3400 course.

#ifndef controller_utils_hxx_
#define controller_utils_hxx_

//===========================================================================

#include <mutex>
#include <atomic>
#include <utility>
#include "benchmark-utils.hxx"

//===========================================================================
//
// A "controller" as defined in this file must have a function call
// operator overload that:
//
//   1) accepts a "tally" object of some kind, and,
//   2) returns a bool.
//
// If the function call operator returns true, then processing
// should continue. If it returns false, then processing should terminate.
//
// What a given controller does with the tally object is defined by that
// controller.
//
//===========================================================================

struct never_stop_controller
{
  // ignore all tally objects and return true...
  template <typename Tally>
  constexpr bool operator ()(Tally const&) const noexcept
  {
    return true;
  }
};

//===========================================================================

class stop_after_count_controller
{
private:
  std::size_t const n_;

public:
  constexpr stop_after_count_controller() noexcept : 
    n_{}
  { 
  }

  constexpr stop_after_count_controller(std::size_t const& n) noexcept :
    n_{n}
  {
  }

  constexpr stop_after_count_controller(
    stop_after_count_controller const&
  ) noexcept = default;
  
  constexpr std::size_t count() const noexcept
  { 
    return n_; 
  }
 
  // return false only when at least n_ tallies were done...
  template <typename Tally>
  constexpr bool operator ()(Tally const& tally) const 
    noexcept(noexcept(tally.count() < n_))
  {
    return tally.count() < n_;
  }
};

//===========================================================================

class stop_when_stopped_controller
{
private:
  std::atomic<bool> stop_;

public:
  stop_when_stopped_controller() noexcept : 
    stop_{true}
  { 
  }

  //
  // atomic<T>s cannot be copy-constructed but one can manually copy-load the
  // value to be constructed. In this case, the lifetime of the object hasn't
  // started and this instance does not depend on c's state --so copy
  // construction is appropriate.
  //
  stop_when_stopped_controller(
    stop_when_stopped_controller const& c
  ) noexcept :
    stop_{c.stop_.load(std::memory_order_relaxed)}
  {
  }
  
  //
  // The following copy-assignment is permitted since there is only one
  // atomic member variable in this class to be updated, i.e., no
  // synchronization or ordering is required with any other variables and 
  // this instance is not otherwise dependent on any other instances.
  //
  stop_when_stopped_controller& operator =(
    stop_when_stopped_controller const& c
  ) noexcept
  {
    stop_.store(
      c.stop_.load(std::memory_order_relaxed), 
      std::memory_order_relaxed
    );
    return *this;
  }

  void reset() noexcept
  {
    stop_.store(true, std::memory_order_relaxed);
  }
  
  void stop() noexcept
  {
    stop_.store(false, std::memory_order_relaxed);
  }

  // return false only when stop_ is false...
  template <typename Tally>
  bool operator ()(Tally const&) const noexcept
  {
    return stop_.load(std::memory_order_relaxed);
  }
};

//===========================================================================

class stop_after_count_or_when_stopped_controller
{
private:
  std::size_t const n_;
  std::atomic<bool> stop_;

public:
  stop_after_count_or_when_stopped_controller() noexcept : 
    n_{},
    stop_{true}
  { 
  }

  stop_after_count_or_when_stopped_controller(std::size_t n) noexcept : 
    n_{n},
    stop_{true}
  { 
  }

  //
  // atomic<T>s cannot be copy-constructed but one can manually copy-load the
  // value to be constructed. In this case, the lifetime of the object hasn't
  // started and this instance does not depend on c's state --so copy
  // construction is appropriate.
  //
  stop_after_count_or_when_stopped_controller(
    stop_after_count_or_when_stopped_controller const& c
  ) noexcept :
    n_{c.n_},
    stop_{c.stop_.load(std::memory_order_relaxed)}
  {
  }

  //
  // The following copy-assignment is deleted since (i) it would modify n_
  // requiring synchronization on n_ and (ii) atomics cannot be 
  // copy-assigned. Item (ii) could be worked-around by using .store() to
  // "copy" the value --but to do that properly would require using a "mutex"
  // to guard accesses to both n_ and stop_ during modification. This
  // imposes significant overhead --so copy-assignment is proscribed.
  //
  stop_after_count_or_when_stopped_controller& operator =(
    stop_after_count_or_when_stopped_controller const&
  ) = delete;

  constexpr std::size_t count() const noexcept
  { 
    return n_; 
  }
  
  void reset() noexcept
  {
    stop_.store(true, std::memory_order_relaxed);
  }
  
  void stop() noexcept
  {
    stop_.store(false, std::memory_order_relaxed);
  }

  // return false only when stop_ is false, or, 
  // when at least n_ tallies were done...
  template <typename Tally>
  bool operator ()(Tally const& tally) const 
    noexcept(noexcept(tally.count() < n_))
  {
    return stop_.load(std::memory_order_relaxed) && (tally.count() < n_);
  }
};

//===========================================================================

template <typename ClockType>
class stop_after_duration_controller
{
public:
  using clock_type = ClockType;
  using duration_type = typename clock_type::duration;

private:
  duration_type d_;
  benchmark<clock_type> bm_;
  std::mutex mutex_;

public:
  stop_after_duration_controller() = delete;
  stop_after_duration_controller(duration_type const& d) :
    d_{d},
    bm_{benchmark<clock_type>::start_clock}
  {
  }
  stop_after_duration_controller(stop_after_duration_controller const&) 
    = delete;
  stop_after_duration_controller(stop_after_duration_controller&&) = delete;
  stop_after_duration_controller& operator =(
    stop_after_duration_controller const&) = delete;
  stop_after_duration_controller& operator =(
    stop_after_duration_controller&&) = delete;

  void stop()
  {
    std::lock_guard<std::mutex> guard(mutex_);
    bm_.stop();
  }

  // return false after d_ time has elapsed...
  template <typename Tally>
  bool operator ()(Tally const&)
  {
    std::lock_guard<std::mutex> guard(mutex_);
    return bm_.duration() < d_;
  }
};

//===========================================================================

#endif // #ifndef controller_hxx_
