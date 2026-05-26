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

DEFUN_DLD (boost_accumulators_density, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{density} =} boost_accumulators_density (@var{data})\n\
@deftypefnx {Loadable Function} {@var{density} =} boost_accumulators_density (@var{data}, @var{cache_size}, @var{num_bins})\n\
\n\
Estimate the probability density of @var{data} using Boost.Accumulators.\n\
Returns an Nx2 matrix where each row is [value, density].\n\
Optional @var{cache_size} (default 10000) and @var{num_bins} (default 100)\n\
control the density estimation.\n\
\n\
@example\n\
@group\n\
d = boost_accumulators_density (randn (1000, 1));\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1 || args.length () > 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_density: DATA must be a numeric matrix");

  std::size_t cache_size = 10000;
  std::size_t num_bins = 100;

  if (args.length () >= 2)
    {
      cache_size = args(1).xuint64_value ("boost_accumulators_density: CACHE_SIZE must be a positive integer");
    }

  if (args.length () >= 3)
    {
      num_bins = args(2).xuint64_value ("boost_accumulators_density: NUM_BINS must be a positive integer");
    }

  ba::accumulator_set<double, ba::stats<ba::tag::density> > acc (
    ba::tag::density::cache_size = cache_size,
    ba::tag::density::num_bins = num_bins
  );

  const double *ptr = data.data ();
  octave_idx_type n = data.numel ();

  for (octave_idx_type i = 0; i < n; ++i)
    {
      double val = ptr[i];
      if (! std::isnan (val))
        acc (val);
    }

  // Extract density: iterator_range of pairs (value, density)
  auto result = ba::density (acc);
  std::size_t num_points = result.size ();

  NDArray out (dim_vector (num_points, 2));

  for (std::size_t i = 0; i < num_points; ++i)
    {
      out(i, 0) = result[i].first;   // value
      out(i, 1) = result[i].second;  // density
    }

  return octave_value (out);
}
