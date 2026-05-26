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

#include <vector>

namespace ba = boost::accumulators;

DEFUN_DLD (boost_accumulators_extended_p_square, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{quantiles} =} boost_accumulators_extended_p_square (@var{data}, @var{probabilities})\n\
\n\
Compute multiple quantile estimates using the extended P^2 algorithm\n\
from Boost.Accumulators.\n\
@var{data} is a numeric matrix.  @var{probabilities} is a vector of\n\
probability values in (0,1).  Returns estimated quantiles corresponding\n\
to each probability.\n\
\n\
@example\n\
@group\n\
probs = [0.25, 0.5, 0.75];\n\
q = boost_accumulators_extended_p_square (randn (1000, 1), probs);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 2)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_extended_p_square: DATA must be a numeric matrix");
  NDArray probs = args(1).xarray_value ("boost_accumulators_extended_p_square: PROBABILITIES must be a numeric vector");

  octave_idx_type nprobs = probs.numel ();
  if (nprobs < 1)
    error ("boost_accumulators_extended_p_square: PROBABILITIES must contain at least one probability");

  // Build probabilities array for Boost
  boost::array<double, 20> prob_arr;  // support up to 20 probabilities
  if (nprobs > 20)
    error ("boost_accumulators_extended_p_square: at most 20 probabilities are supported");

  for (octave_idx_type i = 0; i < nprobs; ++i)
    prob_arr[i] = probs(i);

  // Truncated array: copy to match nprobs
  // We create a vector and use it for probabilities
  std::vector<double> prob_vec (nprobs);
  for (octave_idx_type i = 0; i < nprobs; ++i)
    prob_vec[i] = probs(i);

  ba::accumulator_set<double, ba::stats<ba::tag::extended_p_square> > acc (
    ba::tag::extended_p_square::probabilities = prob_vec
  );

  const double *ptr = data.data ();
  octave_idx_type n = data.numel ();

  for (octave_idx_type i = 0; i < n; ++i)
    {
      double val = ptr[i];
      if (! std::isnan (val))
        acc (val);
    }

  // Extract results
  auto result = ba::extended_p_square (acc);
  std::size_t m = result.size ();

  NDArray out (dim_vector (1, m));
  for (std::size_t i = 0; i < m; ++i)
    out(i) = result[i];

  return octave_value (out);
}
