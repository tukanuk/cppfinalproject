// Copyright (c) Paul Preney 2019. All Rights Reserved.
// For use by students in the University of Windsor's 2019W COMP3400 course.

#ifndef benchmark_utils_hxx_
#define benchmark_utils_hxx_

//===========================================================================

#include <chrono>
#include <ostream>
#include <optional>
#include <utility>

//===========================================================================

template <typename ClockType>
class benchmark
{
public:
  using clock_type = ClockType;
  using rep_type = typename clock_type::rep;
  using period_type = typename clock_type::period;
  using time_point_type = typename clock_type::time_point;
  using duration_type = typename clock_type::duration;
  
  // define constant to indicate "start"ing...
  struct start_clock_type final { };
  static constexpr auto const start_clock = start_clock_type{};

private:
  std::optional<time_point_type> start_time_;
  std::optional<time_point_type> stop_time_;

public:
  benchmark() = default;
  benchmark(benchmark const&) = default;
  benchmark(benchmark&&) = default;
  benchmark& operator =(benchmark const&) = default;
  benchmark& operator =(benchmark&&) = default;

  benchmark(start_clock_type const&) :
    start_time_(clock_type::now())
  {
  }

  void reset()
  {
    stop_time_.reset();
    start_time_.reset();
  }

  void start()
  {
    stop_time_.reset();
    start_time_ = clock_type::now();
  }

  void stop()
  {
    stop_time_ = clock_type::now();
    if (!start_time_)
      start_time_ = stop_time_;
  }

  static constexpr bool is_steady()
  {
    return clock_type::is_steady();
  }

  static constexpr period_type resolution()
  {
    return period_type{};
  }

  duration_type duration()
  {
    // if timing wasn't started, then start it and return zero...
    if (!start_time_)
    {
      start();
      return *start_time_ - *start_time_;
    }

    // if timing stopped, then return the duration...
    if (stop_time_)
      return *stop_time_ - *start_time_;

    // otherwise return the elapsed time so far...
    return clock_type::now() - *start_time_;
  }
};

//===========================================================================

//
// template <typename Rep, typename Period>
// duration_convert(d)
// function template
//
// This function will convert a std::chrono::duration d into a
// std::chrono::duration<Rep,Period>. If the new units are floating point,
// implicit conversions are allowed to ensure the result
// isn't truncated to the source integral type's integer value. This
// implementation defaults to a floating-point representation in seconds.
//
// NOTES: 
//   * Rep should be an integer or floating-point type.
//   * Period is likely to be one of these (suffixed with ::period):
//       std::chrono:{nano,micro,milli}seconds
//       std::chrono::{minutes,hours,days,weeks,months,years}
//
template <
  typename Rep = float, 
  typename Period = std::chrono::seconds::period,
  typename Duration
>
inline auto duration_convert(Duration&& d) 
{
  //
  // Use std::chrono::treat_as_floating_point to know when to allow implicit
  // conversions to floating-point to take place.
  //
  using StdChronoTimeDuration = std::chrono::duration<Rep,Period>;
  if constexpr(std::chrono::treat_as_floating_point_v<StdChronoTimeDuration>)
    return StdChronoTimeDuration(d); // allow implicit conversions
  else
    // no implicit conversions...
    return 
      std::chrono::duration_cast<StdChronoTimeDuration>(
        std::forward<Duration>(d)
      )
    ;
}

//===========================================================================

#endif // #ifndef benchmark_utils_hxx_
