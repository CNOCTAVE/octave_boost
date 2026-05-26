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
#include <boost/accumulators/statistics/weighted_kurtosis.hpp>

namespace ba = boost::accumulators;

DEFUN_DLD (boost_accumulators_weighted_kurtosis, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{k} =} boost_accumulators_weighted_kurtosis (@var{data}, @var{weights})\n\
\n\
Compute the weighted kurtosis of @var{data} using Boost.Accumulators.\n\
@var{weights} is a vector of weights the same size as @var{data}.\n\
The weighted kurtosis measures the tail weight of the probability\n\
distribution, with the normal distribution having zero kurtosis.\n\
\n\
@example\n\
@group\n\
k = boost_accumulators_weighted_kurtosis ([2, 7, 4, 9, 3], [4, 1, 3, 1, 2]);\n\
@result{} k = 0.58137\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 2)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_weighted_kurtosis: DATA must be a numeric matrix");

  NDArray weights = args(1).xarray_value ("boost_accumulators_weighted_kurtosis: WEIGHTS must be a numeric matrix");

  if (weights.numel () != data.numel ())
    error ("boost_accumulators_weighted_kurtosis: "
           "WEIGHTS must have the same number of elements as DATA");

  ba::accumulator_set<double, ba::stats<ba::tag::weighted_kurtosis>, double> acc;

  const double *ptr = data.data ();
  const double *wptr = weights.data ();
  octave_idx_type n = data.numel ();

  for (octave_idx_type i = 0; i < n; ++i)
    {
      double val = ptr[i];
      double w = wptr[i];
      if (! std::isnan (val) && ! std::isnan (w))
        acc (val, ba::weight = w);
    }

  double result = ba::weighted_kurtosis (acc);
  return octave_value (result);
}
