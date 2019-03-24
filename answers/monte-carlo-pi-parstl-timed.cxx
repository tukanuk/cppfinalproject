// Brejvinder
#include <thread>
#include <future>
#include <iomanip>
#include <iostream>

#include <pstl/execution>
#include <pstl/algorithm>
#include <pstl/numeric>
#include <pstl/memory>

#include "tally-utils.hxx"
#include "random-utils.hxx"
#include "benchmark-utils.hxx"
#include "controller-utils.hxx"
#include "monte-carlo-utils.hxx"

#include "project-tally-task.hxx"

using REAL = long double;

int main() {
  using namespace std;

  auto const ninvocations = 8;
  cout << "Running " << ninvocations << " invocations... ";
  cout.flush();

  benchmark<chrono::high_resolution_clock> bm;
  bm.start();

  stop_after_duration_controller<chrono::high_resolution_clock> swsc(2s);

  vector<size_t> indices(ninvocations);
  iota(begin(indices), end(indices), 0);

  auto total = transform_reduce(pstl::execution::par_unseq,
                                begin(indices),
                                end(indices),
                                tally_predicate(),
  [](tally_predicate const & tally1, tally_predicate const & tally2) {return (tally1 + tally2);},
  [&swsc](size_t const&) {
    return monte_carlo<tally_predicate>(swsc, []()->bool{
      static thread_local auto xre = make_randomly_seeded_mt19937_64_engine();
      static thread_local auto yre = make_randomly_seeded_mt19937_64_engine();
      static thread_local uniform_real_distribution<REAL> ud(REAL(0), REAL(1));
      return (sqrt(pow(ud(xre), 2) + pow(ud(yre), 2)) <= 1);
    });
  }
                               );
  bm.stop();

  auto const default_cout_width = cout.width();
  auto const default_cout_precision = cout.precision();

  cout
      << "The Monte Carlo method was executed using " << ninvocations
      << " invocations with a total of " << total.count() << " samples.\n"
      << "The overall estimate of pi is "
      << setw(numeric_limits<REAL>::max_digits10)
      << setprecision(numeric_limits<REAL>::max_digits10)
      << REAL(total.true_count()) / REAL(total.count()) * REAL(4)
      << "\nTotal elapsed time = "
      << setw(default_cout_width)
      << setprecision(default_cout_precision)
      << duration_convert(bm.duration()).count()
      << " seconds."
      << endl;

  return 0;
}
