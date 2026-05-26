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

namespace ba = boost::accumulators;

DEFUN_DLD (boost_accumulators_median_and_variants, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_median_and_variants (@var{data})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_median_and_variants (@var{data}, @var{cache_size}, @var{num_bins})\n\
\n\
Compute the median of @var{data} using Boost.Accumulators with\n\
three different algorithms (P^2 quantile, density, and\n\
P^2 cumulative distribution).  Returns a struct with fields:\n\
@itemize\n\
@item @code{median_p_square} - median using P^2 quantile algorithm\n\
@item @code{median_density} - median using density estimation\n\
@item @code{median_cdist} - median using P^2 cumulative distribution\n\
@end itemize\n\
\n\
Optional parameters for density method:\n\
@var{cache_size} (default 10000) and @var{num_bins} (default 1000).\n\
\n\
@example\n\
@group\n\
r = boost_accumulators_median_and_variants (randn (1000, 1));\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1 || args.length () > 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_median_and_variants: DATA must be a numeric matrix");

  std::size_t cache_size = 10000;
  std::size_t num_bins = 1000;
  std::size_t num_cells = 100;

  if (args.length () >= 2)
    {
      cache_size = args(1).xuint64_value ("boost_accumulators_median_and_variants: "
                                           "CACHE_SIZE must be a positive integer");
    }

  if (args.length () >= 3)
    {
      num_bins = args(2).xuint64_value ("boost_accumulators_median_and_variants: "
                                         "NUM_BINS must be a positive integer");
    }

  // Three median variants
  ba::accumulator_set<double, ba::stats<ba::tag::median(ba::with_p_square_quantile)> > acc_psq;

  ba::accumulator_set<double, ba::stats<ba::tag::median(ba::with_density)> > acc_dens (
    ba::density_cache_size = cache_size,
    ba::density_num_bins = num_bins
  );

  ba::accumulator_set<double, ba::stats<ba::tag::median(ba::with_p_square_cumulative_distribution)> > acc_cdist (
    ba::p_square_cumulative_distribution_num_cells = num_cells
  );

  const double *ptr = data.data ();
  octave_idx_type n = data.numel ();

  for (octave_idx_type i = 0; i < n; ++i)
    {
      double val = ptr[i];
      if (! std::isnan (val))
        {
          acc_psq (val);
          acc_dens (val);
          acc_cdist (val);
        }
    }

  octave_scalar_map result;
  result.setfield ("median_p_square", octave_value (ba::median (acc_psq)));
  result.setfield ("median_density", octave_value (ba::median (acc_dens)));
  result.setfield ("median_cdist", octave_value (ba::median (acc_cdist)));

  return octave_value (result);
}
