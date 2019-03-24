// Copyright (c) Paul Preney 2019. All Rights Reserved.
// For use by students in the University of Windsor's 2019W COMP3400 course.

#ifndef monte_carlo_utils_hxx_
#define monte_carlo_utils_hxx_

//===========================================================================

template <typename Tally, typename Controller, typename Op>
inline Tally monte_carlo(Controller&& c, Op op)
{
  Tally tally;
  while (c(tally(op())))
    ;
  return tally;
}

template <typename Controller, typename Tally, typename Op>
inline Tally const& monte_carlo_tally(Controller&& c, Tally&& tally, Op op)
{
  while (c(tally(op())))
    ;
  return tally;
}

//===========================================================================

#endif // #ifndef monte_carlo_utils_hxx_
