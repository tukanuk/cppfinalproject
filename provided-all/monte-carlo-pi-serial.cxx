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

int main()
{
  using namespace std;

  //
  // Method to estimate pi (3.151592...) using Monte Carlo...
  //
  // Given a circle of radius 0.5 enclosed in a unit square:
  //   * area_of_circle = pi * radius**2
  //     i.e., radius == 0.5
  //   * area_of_square = 1
  // Thus, area_of_square / area_of_circle == pi / 4.
  //
  // Generate uniformly distributed random numbers within
  // the square, i.e., [0,1]. Pi is calculated as the number
  // of points in the circle by the total number of points
  // the resulting ratio should approximate pi / 4. Thus,
  // a Monte Carlo estimate of pi would be 4 times pi / 4.
  //

  #error "TODO: Declare a variable called bm of type benchmark<chrono::high_resolution_clock>"
  #error "TODO: Declare a variable called swsc of type stop_after_count_controller."
  // NOTE: Also pass 50'000'000 (50 million) to swsc's constructor.

  cout << "Running... "; cout.flush();
  bm.start(); // start benchmarking

  // run the Monte Carlo simulation...
  #error "TODO: Follow the directions below to code/run the Monte Carlo simulation."
  //
  // NOTE 1: The instructions below involve a SINGLE function call to a
  //         function called monte_carlo() defined in monte-carlo-utils.hxx.
  //
  // NOTE 2: The monte_carlo() function requires one to pass in the first
  //         template parameter (which must be a suitable "tally" type).
  //         This template parameter will be the type "tally_predicate"
  //         which you must write the code for in project-tally-task.hxx.
  //
  //         The file project-tally-task.hxx provides all instructions in
  //         #error and // comments to complete that partially-provided code.
  //
  //         The purpose of the tally_predicate class is:
  //           * track the number of times something is tallied
  //           * track the number of times the tally value is true
  //           * track the number of times the tally value is false
  //         A tally occurs when the type's function call operator overload
  //         is called. (For examples of other tallying objects see
  //         tally-utils.hxx.)
  //
  //         You can see how the tally type is used with a controller
  //         type in monte-carlo-utils.hxx. Notice:
  //           * everything loops until the controller's
  //             function call operator overload returns false
  //           * the controller's function call operator overload
  //             has one argument that receives the return
  //             value from the tally type's function call operator
  //             overload
  //           * typically the tally type's function call operator
  //             overload returns itself
  //           * each iteration of the loop executes the function
  //             passed to monte_carlo<T>()'s second argument
  //             once
  //         Thus, one can deterministically control when the loop
  //         iteration stops via the definition of the controller's
  //         function call operator overload, i.e., it must return
  //         false to terminate the loop.
  //
  // INSTRUCTIONS:
  //   1) Declare an auto variable called result and initialize it to
  //      the return value of the monte_carlo<tally_predicate>()
  //      call.
  //   2) The first argument to the monte_carlo<tally_predicate>()
  //      needs to be the controller, i.e., swsc.
  //   3) The second argument will be a lambda function defined
  //      in order of appearance as follows:
  //        * The lambda captures no variables.
  //        * The lambda is passed no arguments.
  //        * The lambda returns a bool (but this can be omitted).
  //        * Within the lambda, define 2 STATIC variables
  //          representing random number engines as follows:
  //            - xre = make_randomly_seeded_mt19937_64_engine();
  //            - yre = make_randomly_seeded_mt19937_64_engine();
  //          where xre is for the random x-coordinates, yre is for
  //          the random y-coordinates.
  //            NOTE 1: The code in random-utils.hxx has the correct way
  //                    to properly and randomly seed a Mersenne Twister.
  //                    It is provided so you can see how to do this.
  //            NOTE 2: The x- and y- values are independent (orthogonal)
  //                    and need to be computed using independent
  //                    (orthogonal) random number engines.
  //        * Within the lambda, define 1 STATIC variable of type
  //          uniform_real_distribution<REAL> called "ud" and pass
  //          these two valuse to its constructor: REAL(0) and
  //          REAL(1).
  //            NOTE 1: You could hard-code 0.0 and 1.0. Using REAL()
  //                    essentially converts 0 and 1 to the type REAL.
  //            NOTE 2: REAL is a type alias already defined in this file.
  //        * Within the lambda, generate two random values: an x
  //          value and a y value and store these in two variables. To
  //          generate the random x value, write ud(xre), and, use
  //          ud(yre) to generate the random y value.
  //            NOTE: ud's type has a function call operator overload
  //                  per C++'s definition of <random>. See the
  //                  Josuttis text discussion of random numbers.
  //        * Per the code in monte-carlo-utils.hxx, this lambda
  //          needs to return a bool. Generate a point in the unit squire and
  //          return whether or not it is inside the circle.
#error "TODO: Write auto result = monte_carlo<tally_predicate>, etc. here"

  bm.stop(); // stop benchmarking
  cout << "Done.\n";

  // the next two lines allow one to reset width and precision to their
  // defaults...
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