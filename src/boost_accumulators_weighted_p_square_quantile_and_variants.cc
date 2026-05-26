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
#include <boost/accumulators/statistics/weighted_p_square_quantile.hpp>

namespace ba = boost::accumulators;

DEFUN_DLD (boost_accumulators_weighted_p_square_quantile_and_variants, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{q} =} boost_accumulators_weighted_p_square_quantile_and_variants (@var{data}, @var{weights}, @var{prob})\n\
\n\
Estimate a single weighted quantile of @var{data} using the P^2 algorithm.\n\
@var{weights} is a vector of weights the same size as @var{data}.\n\
@var{prob} is the probability value (0,1) for the desired quantile.\n\
Returns the estimated quantile value.\n\
\n\
@example\n\
@group\n\
w = rand (100000, 1);\n\
q = boost_accumulators_weighted_p_square_quantile_and_variants (rand (100000, 1), w, 0.5);\n\
@result{} q ~ 0.5\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_weighted_p_square_quantile_and_variants: "
                                        "DATA must be a numeric matrix");

  NDArray weights = args(1).xarray_value ("boost_accumulators_weighted_p_square_quantile_and_variants: "
                                           "WEIGHTS must be a numeric matrix");

  if (weights.numel () != data.numel ())
    error ("boost_accumulators_weighted_p_square_quantile_and_variants: "
           "WEIGHTS must have the same number of elements as DATA");

  double prob = args(2).double_value ();
  if (prob <= 0.0 || prob >= 1.0)
    error ("boost_accumulators_weighted_p_square_quantile_and_variants: "
           "PROB must be a scalar in (0,1)");

  ba::accumulator_set<double, ba::stats<ba::tag::weighted_p_square_quantile>, double>
    acc (ba::quantile_probability = prob);

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
    error ("boost_accumulators_weighted_p_square_quantile_and_variants: "
           "DATA contains only NaN values");

  double result = ba::weighted_p_square_quantile (acc);
  return octave_value (result);
}
