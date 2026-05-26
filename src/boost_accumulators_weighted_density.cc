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
#include <boost/accumulators/statistics/weighted_density.hpp>

#include <octave/oct.h>

#include <vector>

using namespace boost::accumulators;

DEFUN_DLD (boost_accumulators_weighted_density, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{density} =} boost_accumulators_weighted_density (@var{data}, @var{weights})\n\
@deftypefnx {Loadable Function} {@var{density} =} boost_accumulators_weighted_density (@var{data}, @var{weights}, @var{cache_size}, @var{num_bins})\n\
\n\
Estimate the weighted probability density of @var{data} using\n\
Boost.Accumulators.  @var{weights} is a vector of weights the same\n\
size as @var{data}.  Returns an Nx2 matrix where each row is\n\
[value, weighted_density].\n\
Optional @var{cache_size} (default 10000) and @var{num_bins} (default 100)\n\
control the density estimation.\n\
\n\
@example\n\
@group\n\
w = rand (1000, 1);\n\
d = boost_accumulators_weighted_density (randn (1000, 1), w);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2 || args.length () > 4)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_weighted_density: DATA must be a numeric matrix");

  NDArray weights = args(1).xarray_value ("boost_accumulators_weighted_density: WEIGHTS must be a numeric matrix");

  if (weights.numel () != data.numel ())
    error ("boost_accumulators_weighted_density: "
           "WEIGHTS must have the same number of elements as DATA");

  std::size_t cache_size = 10000;
  std::size_t num_bins = 100;

  if (args.length () >= 3)
    {
      cache_size = args(2).xuint64_value ("boost_accumulators_weighted_density: CACHE_SIZE must be a positive integer");
    }

  if (args.length () >= 4)
    {
      num_bins = args(3).xuint64_value ("boost_accumulators_weighted_density: NUM_BINS must be a positive integer");
    }

  accumulator_set<double, stats<tag::weighted_density>, double> acc (
    tag::weighted_density::cache_size = cache_size,
    tag::weighted_density::num_bins = num_bins
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

  // Extract weighted density: iterator_range of pairs (value, density)
  auto result = weighted_density (acc);
  std::size_t num_points = result.size ();

  NDArray out (dim_vector (num_points, 2));

  for (std::size_t i = 0; i < num_points; ++i)
    {
      out(i, 0) = result[i].first;   // value
      out(i, 1) = result[i].second;  // weighted density
    }

  return octave_value (out);
}
