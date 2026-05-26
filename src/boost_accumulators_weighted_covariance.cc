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
#include <boost/accumulators/statistics/weighted_covariance.hpp>

#include <octave/oct.h>

namespace ba = boost::accumulators;

DEFUN_DLD (boost_accumulators_weighted_covariance, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{c} =} boost_accumulators_weighted_covariance (@var{data})\n\
@deftypefnx {Loadable Function} {@var{c} =} boost_accumulators_weighted_covariance (@var{data}, @var{weights})\n\
\n\
Compute the weighted covariance between two variables using\n\
Boost.Accumulators.\n\
@var{data} must be an Nx2 matrix, where the first column is variable X\n\
and the second column is variable Y.\n\
Optional @var{weights} is a vector of weights the same length as @var{data}.\n\
Returns the weighted covariance.\n\
\n\
@example\n\
@group\n\
c = boost_accumulators_weighted_covariance ([1, 2; 1, 4; 2, 3; 6, 1]);\n\
@result{} c = -1.75\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1 || args.length () > 2)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_weighted_covariance: DATA must be a numeric matrix");

  if (data.columns () < 2)
    error ("boost_accumulators_weighted_covariance: DATA must have at least 2 columns");

  bool has_weights = (args.length () >= 2);
  NDArray weights;
  if (has_weights)
    {
      weights = args(1).xarray_value ("boost_accumulators_weighted_covariance: WEIGHTS must be a numeric matrix");
      if (weights.numel () != data.rows ())
        error ("boost_accumulators_weighted_covariance: "
               "WEIGHTS must have the same number of elements as DATA rows");
    }

  ba::accumulator_set<double, ba::stats<ba::tag::weighted_covariance<double, ba::tag::covariate1> >, double> acc;

  octave_idx_type n = data.rows ();
  bool any_data = false;

  for (octave_idx_type i = 0; i < n; ++i)
    {
      double x = data(i, 0);
      double y = data(i, 1);
      if (! std::isnan (x) && ! std::isnan (y))
        {
          any_data = true;
          if (has_weights)
            {
              double w = weights(i);
              if (! std::isnan (w))
                acc (x, ba::weight = w, ba::covariate1 = y);
            }
          else
            acc (x, ba::weight = 1.0, ba::covariate1 = y);
        }
    }

  if (! any_data)
    error ("boost_accumulators_weighted_covariance: DATA contains only NaN values");

  double result = ba::weighted_covariance (acc);
  return octave_value (result);
}
