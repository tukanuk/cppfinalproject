// Copyright (c) Paul Preney 2019. All Rights Reserved.
// For use by students in the University of Windsor's 2019W COMP3400 course.
// Brejvinder
#ifndef project_tally_task_hxx_
#define project_tally_task_hxx_

//===========================================================================

#include <cstddef>

//===========================================================================

class tally_predicate {
 public:
  using size_type = std::size_t;

 private:
  size_type n_;
  size_type true_;

 public:
  tally_predicate() : n_() , true_() {}

  tally_predicate(tally_predicate const& t) = default;
  tally_predicate(tally_predicate&& t) = default;
  tally_predicate& operator =(tally_predicate const& t) = default;
  tally_predicate& operator =(tally_predicate&& t) = default;

  tally_predicate& operator +=(tally_predicate const& rhs) {
    n_ += rhs.n_;
    true_ += rhs.true_;
    return *this;
  }

  tally_predicate operator +(tally_predicate const& rhs) const {
    tally_predicate tmp(*this);
    tmp += rhs;
    return tmp;
  }

  size_type const& count() const {
    return n_;
  }

  size_type const& true_count() const {
    return true_;
  }

  size_type false_count() const {
    return n_ - true_;
  }

  template <typename T>
  tally_predicate& operator ()(T const& t) {
    ++n_;
    true_ += bool(t);
    return *this;
  }
};

//===========================================================================

#endif // #ifndef project_tally_task_hxx_
