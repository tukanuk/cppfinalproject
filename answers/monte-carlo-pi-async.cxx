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
  stop_when_stopped_controller swsc;

  auto future_result = async(launch::async, [&swsc, &bm]() {
    bm.start();
    auto result = monte_carlo<tally_predicate>(swsc, []()->bool{
      static thread_local auto xre = make_randomly_seeded_mt19937_64_engine();
      static thread_local auto yre = make_randomly_seeded_mt19937_64_engine();
      static thread_local uniform_real_distribution<REAL> ud(REAL(0), REAL(1));
      return (sqrt(pow(ud(xre), 2) + pow(ud(yre), 2)) <= 1);
    });
    bm.stop();
    return result;
  });

  cout << "Sleeping... ";
  cout.flush();
  this_thread::sleep_for(chrono::seconds(2));

  cout << "stopping... ";
  swsc.stop();
  cout.flush();

  auto result = future_result.get();
  cout << "Done.\n";
  cout.flush();

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
