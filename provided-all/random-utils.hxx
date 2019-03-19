// Copyright (c) Paul Preney 2019. All Rights Reserved.
// For use by students in the University of Windsor's 2019W COMP3400 course.

#ifndef random_utils_hxx_
#define random_utils_hxx_

//===========================================================================

#include <random>
#include <vector>
#include <algorithm>
#include <type_traits>

//===========================================================================

//
// template <typename MersenneEngine>
// make_randomly_seeded_mersenne_twister_random_engine()
// function
//
// This function returns a properly-seeded randomly seeded Mersenne Twister
// (std::mt19937 by default) random number engine.
//
// NOTE: This function is only defined if MersenneEngine has a state_size
//       integral member. Such is used to generate sufficient random numbers
//       to seed the random number engine returned.
//

template <typename MersenneEngine = std::mt19937>
inline auto make_randomly_seeded_mersenne_twister_random_engine() ->
  typename std::enable_if<
    MersenneEngine::state_size != 0, MersenneEngine
  >::type
{
  using namespace std;

  // Use std::random_device for random data...
  random_device rd;
  vector<typename MersenneEngine::result_type> data;

  // Populate random_data with enough random data for the random engine's
  // state_size...
  data.reserve(MersenneEngine::state_size);
  generate_n(
    back_inserter(data),
    MersenneEngine::state_size,
    [&rd](){ return rd(); }
  );

  // Fill seed_seq with seed value from data
  seed_seq seeds(begin(data), end(data));
  return MersenneEngine(seeds);
}

//===========================================================================

//
// make_randomly_seeded_mt19937_engine()
// function
//
// This function returns a properly randomly seeded std::mt19937 engine.
//
inline std::mt19937 make_randomly_seeded_mt19937_engine()
{
  return make_randomly_seeded_mersenne_twister_random_engine<std::mt19937>();
}

//
// make_randomly_seeded_mt19937_64_engine()
// function
//
// This function returns a properly randomly seeded std::mt19937_64 engine.
//
inline std::mt19937_64 make_randomly_seeded_mt19937_64_engine()
{
  return 
    make_randomly_seeded_mersenne_twister_random_engine<std::mt19937_64>()
  ;
}

//===========================================================================

#endif // #ifndef random_utils_hxx_
