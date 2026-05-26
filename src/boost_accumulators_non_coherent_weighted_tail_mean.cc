// Copyright (C) 2024-2026 Yu Hongbo <yuhongbo@member.fsf.org>,
//                    CNOCTAVE <cnoctave@qq.com>
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, see <http://www.gnu.org/licenses/>.

#include <octave/oct.h>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/accumulators/statistics/tail.hpp>

#include <vector>

using namespace boost::accumulators;

DEFUN_DLD (boost_accumulators_non_coherent_weighted_tail_mean, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_non_coherent_weighted_tail_mean (@var{data}, @var{weights})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_non_coherent_weighted_tail_mean (@var{data}, @var{weights}, @var{cache_size})\n\
\n\
Estimate the non-coherent weighted tail mean based on order statistics\n\
for both left and right tails using Boost.Accumulators.\n\
@var{weights} is a vector of weights the same size as @var{data}.\n\
Optional @var{cache_size} (default 10000) specifies the tail cache size.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{non_coherent_tail_mean_right} - right tail non-coherent tail mean\n\
@item @code{non_coherent_tail_mean_left} - left tail non-coherent tail mean\n\
@item @code{tail_quantile_right} - right tail quantile\n\
@item @code{tail_quantile_left} - left tail quantile\n\
@end itemize\n\
\n\
@example\n\
@group\n\
w = rand (10000, 1);\n\
r = boost_accumulators_non_coherent_weighted_tail_mean (randn (10000, 1), w);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2 || args.length () > 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_non_coherent_weighted_tail_mean: "
                                        "DATA must be a numeric matrix");

  NDArray weights = args(1).xarray_value ("boost_accumulators_non_coherent_weighted_tail_mean: "
                                           "WEIGHTS must be a numeric matrix");

  if (weights.numel () != data.numel ())
    error ("boost_accumulators_non_coherent_weighted_tail_mean: "
           "WEIGHTS must have the same number of elements as DATA");

  std::size_t cache_size = 10000;
  if (args.length () >= 3)
    {
      cache_size = args(2).xuint64_value ("boost_accumulators_non_coherent_weighted_tail_mean: "
                                           "CACHE_SIZE must be a positive integer >= 2");
      if (cache_size < 2)
        error ("boost_accumulators_non_coherent_weighted_tail_mean: "
               "CACHE_SIZE must be a positive integer >= 2");
    }

  // Right tail weighted accumulator
  accumulator_set<double, stats<tag::non_coherent_tail_mean<right>, tag::tail_quantile<right> >, double>
    acc_right (right_tail_cache_size = cache_size);

  // Left tail weighted accumulator
  accumulator_set<double, stats<tag::non_coherent_tail_mean<left>, tag::tail_quantile<left> >, double>
    acc_left (left_tail_cache_size = cache_size);

  const double *ptr = data.data ();
  const double *wptr = weights.data ();
  octave_idx_type n = data.numel ();
  bool any_data = false;

  for (octave_idx_type i = 0; i < n; ++i)
    {
      double val = ptr[i];
      double w = wptr[i];
      if (! std::isnan (val) && ! std::isnan (w))
        {
          any_data = true;
          acc_right (val, weight = w);
          acc_left (val, weight = w);
        }
    }

  if (! any_data)
    error ("boost_accumulators_non_coherent_weighted_tail_mean: "
           "DATA contains only NaN values");

  octave_scalar_map result;

  // non_coherent_tail_mean and tail_quantile need quantile_probability
  // passed explicitly alongside the accumulator set.
  // Note: the extra parentheses force the comma to be the Boost.Parameter
  // argument-pack operator rather than the C++ function-call argument separator.
  auto nctm_right = find_accumulator<tag::non_coherent_tail_mean<right>>(acc_right).result(
    (accumulator = acc_right, quantile_probability = 0.95)
  );
  auto nctm_left  = find_accumulator<tag::non_coherent_tail_mean<left>>(acc_left).result(
    (accumulator = acc_left, quantile_probability = 0.05)
  );
  auto tq_right   = find_accumulator<tag::tail_quantile<right>>(acc_right).result(
    (accumulator = acc_right, quantile_probability = 0.95)
  );
  auto tq_left    = find_accumulator<tag::tail_quantile<left>>(acc_left).result(
    (accumulator = acc_left, quantile_probability = 0.05)
  );

  result.setfield ("non_coherent_tail_mean_right", octave_value (nctm_right));
  result.setfield ("non_coherent_tail_mean_left",  octave_value (nctm_left));
  result.setfield ("tail_quantile_right",           octave_value (tq_right));
  result.setfield ("tail_quantile_left",            octave_value (tq_left));

  return octave_value (result);
}
