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
#include <boost/accumulators/statistics/weighted_skewness.hpp>

using namespace boost::accumulators;

DEFUN_DLD (boost_accumulators_weighted_skewness, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{s} =} boost_accumulators_weighted_skewness (@var{data}, @var{weights})\n\
\n\
Compute the weighted skewness of @var{data} using Boost.Accumulators.\n\
The skewness of a sample distribution is defined as the ratio of the 3rd\n\
central moment and the 3/2-th power of the 2nd central moment (variance).\n\
@var{weights} is a vector of weights the same size as @var{data}.\n\
\n\
@example\n\
@group\n\
s = boost_accumulators_weighted_skewness ([2, 7, 4, 9, 3], [4, 1, 3, 1, 2]);\n\
@result{} s = 1.3071\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 2)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_weighted_skewness: DATA must be a numeric matrix");

  NDArray weights = args(1).xarray_value ("boost_accumulators_weighted_skewness: WEIGHTS must be a numeric matrix");

  if (weights.numel () != data.numel ())
    error ("boost_accumulators_weighted_skewness: "
           "WEIGHTS must have the same number of elements as DATA");

  accumulator_set<double, stats<tag::weighted_skewness>, double> acc;

  const double *ptr = data.data ();
  const double *wptr = weights.data ();
  octave_idx_type n = data.numel ();
  bool any_data = false;

  for (octave_idx_type i = 0; i < n; ++i)
    {
      double val = ptr[i];
      double w = wptr[i];
      if (! std::isnan (ptr[i]) && ! std::isnan (wptr[i]))
        {
          any_data = true;
          acc (val, weight = w);
        }
    }

  if (! any_data)
    error ("boost_accumulators_weighted_skewness: DATA contains only NaN values");

  double result = weighted_skewness (acc);
  return octave_value (result);
}

