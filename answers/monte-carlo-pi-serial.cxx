// Brejvinder
#include <thread>
#include <future>
#include <iomanip>
#include <iostream>

#include "tally-utils.hxx"
#include "random-utils.hxx"
#include "benchmark-utils.hxx"
#include "controller-utils.hxx"
#include "monte-carlo-utils.hxx"

#include "project-tally-task.hxx"

using REAL = long double;

int main() {
  using namespace std;

  benchmark<chrono::high_resolution_clock> bm;
  stop_after_count_controller swsc(50'000'0000);

  cout << "Running... "; cout.flush();
  bm.start(); // start benchmarking

  auto result = monte_carlo<tally_predicate>(swsc, []()->bool{
    static auto xre = make_randomly_seeded_mt19937_64_engine();
    static auto yre = make_randomly_seeded_mt19937_64_engine();
    static uniform_real_distribution<REAL> ud(REAL(0), REAL(1));
    return (sqrt(pow(ud(xre), 2) + pow(ud(yre), 2)) <= 1);
  });

  bm.stop(); // stop benchmarking
  cout << "Done.\n";

  auto const default_cout_width = cout.width();
  auto const default_cout_precision = cout.precision();

  cout
      << "The Monte Carlo method was executed serially "
      << result.count() << " times.\n"
      << "Estimate of pi = "
      << setw(numeric_limits<REAL>::max_digits10)
      << setprecision(numeric_limits<REAL>::max_digits10)
      << REAL(result.true_count()) / REAL(result.count()) * REAL(4)
      << "\nTotal elapsed time = "
      << setw(default_cout_width)
      << setprecision(default_cout_precision)
      << duration_convert(bm.duration()).count()
      << " seconds."
      << endl;
  ;
}

