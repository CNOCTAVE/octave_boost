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
#include <boost/accumulators/statistics/weighted_p_square_cumulative_distribution.hpp>

#include <vector>

namespace ba = boost::accumulators;

DEFUN_DLD (boost_accumulators_weighted_p_square_cumulative_distribution, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{hist} =} boost_accumulators_weighted_p_square_cumulative_distribution (@var{data}, @var{weights})\n\
@deftypefnx {Loadable Function} {@var{hist} =} boost_accumulators_weighted_p_square_cumulative_distribution (@var{data}, @var{weights}, @var{num_cells})\n\
\n\
Estimate the cumulative distribution function of @var{data} using the P^2\n\
algorithm for weighted samples.\n\
@var{weights} is a vector of weights the same size as @var{data}.\n\
Optional @var{num_cells} (default 100) specifies the number of bins.\n\
Returns an Nx2 matrix where each row is [value, cumulative_probability].\n\
\n\
@example\n\
@group\n\
w = rand (10000, 1);\n\
hist = boost_accumulators_weighted_p_square_cumulative_distribution (randn (10000, 1), w);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2 || args.length () > 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_weighted_p_square_cumulative_distribution: "
                                        "DATA must be a numeric matrix");

  NDArray weights = args(1).xarray_value ("boost_accumulators_weighted_p_square_cumulative_distribution: "
                                           "WEIGHTS must be a numeric matrix");

  if (weights.numel () != data.numel ())
    error ("boost_accumulators_weighted_p_square_cumulative_distribution: "
           "WEIGHTS must have the same number of elements as DATA");

  std::size_t num_cells = 100;
  if (args.length () >= 3)
    {
      num_cells = args(2).uint64_value ();
      if (num_cells < 2)
        error ("boost_accumulators_weighted_p_square_cumulative_distribution: "
               "NUM_CELLS must be a positive integer >= 2");
    }

  ba::accumulator_set<double, ba::stats<ba::tag::weighted_p_square_cumulative_distribution>, double>
    acc (ba::tag::weighted_p_square_cumulative_distribution::num_cells = num_cells);

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
          acc (val, ba::weight = w);
        }
    }

  if (! any_data)
    error ("boost_accumulators_weighted_p_square_cumulative_distribution: "
           "DATA contains only NaN values");

  // Extract the histogram (use auto to avoid iterator_range dependency)
  auto hist = ba::weighted_p_square_cumulative_distribution (acc);
  std::size_t sz = hist.size ();

  NDArray result (dim_vector (sz, 2));
  for (std::size_t i = 0; i < sz; ++i)
    {
      result(i, 0) = hist[i].first;   // value
      result(i, 1) = hist[i].second;  // cumulative probability
    }

  return octave_value (result);
}
