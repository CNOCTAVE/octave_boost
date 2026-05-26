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
#include <boost/accumulators/statistics/weighted_mean.hpp>

#include <octave/oct.h>

namespace ba = boost::accumulators;

DEFUN_DLD (boost_accumulators_weighted_mean_and_variants, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_weighted_mean_and_variants (@var{data}, @var{weights})\n\
\n\
Compute the weighted mean and its variants of @var{data} using\n\
Boost.Accumulators.\n\
@var{weights} is a vector of weights the same size as @var{data}.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{weighted_mean} - weighted mean of samples\n\
@item @code{weighted_mean_of_variates} - weighted mean of variates\n\
@item @code{sum_of_weights} - sum of weights\n\
@item @code{count} - number of non-NaN elements\n\
@end itemize\n\
\n\
@example\n\
@group\n\
r = boost_accumulators_weighted_mean_and_variants ([10, 6, 4, 6], [2, 3, 4, 5]);\n\
@result{} r.weighted_mean = 6.0\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 2)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_weighted_mean_and_variants: "
                                        "DATA must be a numeric matrix");

  NDArray weights = args(1).xarray_value ("boost_accumulators_weighted_mean_and_variants: "
                                           "WEIGHTS must be a numeric matrix");

  if (weights.numel () != data.numel ())
    error ("boost_accumulators_weighted_mean_and_variants: "
           "WEIGHTS must have the same number of elements as DATA");

  ba::accumulator_set<
      double
    , ba::stats<
          ba::tag::weighted_mean
        , ba::tag::weighted_mean_of_variates<double, ba::tag::covariate1>
        , ba::tag::sum_of_weights
        , ba::tag::count
      >
    , double
  > acc;

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
          acc (val, ba::weight = w, ba::covariate1 = val);
        }
    }

  if (! any_data)
    error ("boost_accumulators_weighted_mean_and_variants: "
           "DATA contains only NaN values");

  octave_scalar_map result;

  result.setfield ("weighted_mean", octave_value (ba::weighted_mean (acc)));
  result.setfield ("weighted_mean_of_variates",
                   octave_value (ba::weighted_mean_of_variates<double, ba::tag::covariate1> (acc)));
  result.setfield ("sum_of_weights", octave_value (ba::sum_of_weights (acc)));
  result.setfield ("count", octave_value (static_cast<double> (ba::count (acc))));

  return octave_value (result);
}
