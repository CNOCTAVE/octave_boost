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
#include <boost/accumulators/statistics/weighted_median.hpp>
#include <boost/accumulators/statistics/density.hpp>
#include <boost/accumulators/statistics/p_square_cumulative_distribution.hpp>

namespace ba = boost::accumulators;

DEFUN_DLD (boost_accumulators_weighted_median_and_variants, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_weighted_median_and_variants (@var{data}, @var{weights})\n\
\n\
Compute the weighted median of @var{data} using three different algorithms\n\
from Boost.Accumulators: P^2 quantile, density, and cumulative distribution.\n\
@var{weights} is a vector of weights the same size as @var{data}.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{weighted_median_p_square} - weighted median via P^2 algorithm\n\
@item @code{weighted_median_density} - weighted median via density estimation\n\
@item @code{weighted_median_cdist} - weighted median via cumulative distribution\n\
@end itemize\n\
\n\
@example\n\
@group\n\
w = rand (1000, 1);\n\
r = boost_accumulators_weighted_median_and_variants (randn (1000, 1), w);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 2)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_weighted_median_and_variants: "
                                        "DATA must be a numeric matrix");

  NDArray weights = args(1).xarray_value ("boost_accumulators_weighted_median_and_variants: "
                                           "WEIGHTS must be a numeric matrix");

  if (weights.numel () != data.numel ())
    error ("boost_accumulators_weighted_median_and_variants: "
           "WEIGHTS must have the same number of elements as DATA");

  // P^2 weighted median
  ba::accumulator_set<double, ba::stats<ba::tag::weighted_median(ba::with_p_square_quantile)>, double>
    acc_psquare;

  // Density weighted median
  ba::accumulator_set<double, ba::stats<ba::tag::weighted_median(ba::with_density)>, double>
    acc_dens (ba::density_cache_size = 1000,
              ba::density_num_bins = 100);

  // Cumulative distribution weighted median
  ba::accumulator_set<double, ba::stats<ba::tag::weighted_median(ba::with_p_square_cumulative_distribution)>, double>
    acc_cdist (ba::p_square_cumulative_distribution_num_cells = 100);

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
          acc_psquare (val, ba::weight = w);
          acc_dens (val, ba::weight = w);
          acc_cdist (val, ba::weight = w);
        }
    }

  if (! any_data)
    error ("boost_accumulators_weighted_median_and_variants: "
           "DATA contains only NaN values");

  octave_scalar_map result;

  result.setfield ("weighted_median_p_square",
                   octave_value (ba::weighted_median (acc_psquare)));
  result.setfield ("weighted_median_density",
                   octave_value (ba::weighted_median (acc_dens)));
  result.setfield ("weighted_median_cdist",
                   octave_value (ba::weighted_median (acc_cdist)));

  return octave_value (result);
}
