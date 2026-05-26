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

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/accumulators/statistics/weighted_extended_p_square.hpp>

#include <octave/oct.h>

#include <vector>

using namespace boost::accumulators;

DEFUN_DLD (boost_accumulators_weighted_extended_p_square, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{quantiles} =} boost_accumulators_weighted_extended_p_square (@var{data}, @var{probabilities}, @var{weights})\n\
\n\
Compute multiple weighted quantile estimates using the extended P^2\n\
algorithm from Boost.Accumulators.\n\
@var{data} is a numeric matrix.  @var{probabilities} is a vector of\n\
probability values in (0,1).  @var{weights} is a vector of weights\n\
the same size as @var{data}.\n\
Returns estimated weighted quantiles corresponding to each probability.\n\
\n\
@example\n\
@group\n\
probs = [0.01, 0.1, 0.5, 0.9, 0.99];\n\
w = rand (10000, 1);\n\
q = boost_accumulators_weighted_extended_p_square (randn (10000, 1), probs, w);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_weighted_extended_p_square: DATA must be a numeric array");
  NDArray probs = args(1).xarray_value ("boost_accumulators_weighted_extended_p_square: PROBABILITIES must be a numeric array");
  NDArray weights = args(2).xarray_value ("boost_accumulators_weighted_extended_p_square: WEIGHTS must be a numeric array");

  if (weights.numel () != data.numel ())
    error ("boost_accumulators_weighted_extended_p_square: "
           "WEIGHTS must have the same number of elements as DATA");

  octave_idx_type nprobs = probs.numel ();
  if (nprobs < 1)
    error ("boost_accumulators_weighted_extended_p_square: "
           "PROBABILITIES must contain at least one probability");

  if (nprobs > 20)
    error ("boost_accumulators_weighted_extended_p_square: "
           "at most 20 probabilities are supported");

  std::vector<double> prob_vec (nprobs);
  for (octave_idx_type i = 0; i < nprobs; ++i)
    prob_vec[i] = probs(i);

  accumulator_set<double, stats<tag::weighted_extended_p_square>, double> acc (
    tag::weighted_extended_p_square::probabilities = prob_vec
  );

  const double *ptr = data.data ();
  const double *wptr = weights.data ();
  octave_idx_type n = data.numel ();

  for (octave_idx_type i = 0; i < n; ++i)
    {
      double val = ptr[i];
      double w = wptr[i];
      if (! std::isnan (val) && ! std::isnan (w))
        acc (val, weight = w);
    }

  // Extract results
  auto result = weighted_extended_p_square (acc);
  std::size_t m = result.size ();

  NDArray out (dim_vector (1, m));
  for (std::size_t i = 0; i < m; ++i)
    out(i) = result[i];

  return octave_value (out);
}
