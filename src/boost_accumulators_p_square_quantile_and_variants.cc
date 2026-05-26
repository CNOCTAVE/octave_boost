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

#include <vector>

using namespace boost::accumulators;

DEFUN_DLD (boost_accumulators_p_square_quantile_and_variants, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{q} =} boost_accumulators_p_square_quantile_and_variants (@var{data}, @var{prob})\n\
@deftypefnx {Loadable Function} {@var{q} =} boost_accumulators_p_square_quantile_and_variants (@var{data}, @var{prob}, @var{weights})\n\
\n\
Compute single quantile estimates using the P^2 algorithm\n\
from Boost.Accumulators.  @var{prob} is a scalar probability in (0,1).\n\
Optional @var{weights} is a vector of weights the same size as @var{data}.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{quantile} - P^2 quantile estimate\n\
@item @code{weighted_quantile} - weighted P^2 quantile (if weights given)\n\
@end itemize\n\
\n\
@example\n\
@group\n\
r = boost_accumulators_p_square_quantile_and_variants (randn (100, 1), 0.5);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2 || args.length () > 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_p_square_quantile_and_variants: "
                                        "DATA must be a numeric matrix");

  double prob = args(1).double_value ();
  if (prob <= 0.0 || prob >= 1.0)
    error ("boost_accumulators_p_square_quantile_and_variants: "
           "PROB must be in the range (0, 1)");

  bool has_weights = (args.length () >= 3);
  NDArray weights;
  if (has_weights)
    {
      weights = args(2).xarray_value ("boost_accumulators_p_square_quantile_and_variants: "
                                       "WEIGHTS must be a numeric matrix");
      if (weights.numel () != data.numel ())
        error ("boost_accumulators_p_square_quantile_and_variants: "
               "WEIGHTS must have the same number of elements as DATA");
    }

  const double *ptr = data.data ();
  octave_idx_type n = data.numel ();

  octave_scalar_map result;

  // P^2 quantile accumulator
  accumulator_set<double, stats<tag::p_square_quantile> > acc(
    quantile_probability = prob
  );

  // Weighted P^2 quantile (if weights provided)
  accumulator_set<double, stats<tag::weighted_p_square_quantile>, double> acc_w(
    quantile_probability = prob
  );

  for (octave_idx_type i = 0; i < n; ++i)
    {
      double val = ptr[i];
      if (! std::isnan (val))
        {
          acc (val);
          if (has_weights)
            {
              double w = weights(i);
              if (! std::isnan (w))
                acc_w (val, weight = w);
            }
        }
    }

  result.setfield ("quantile", octave_value (p_square_quantile (acc)));

  if (has_weights)
    result.setfield ("weighted_quantile", octave_value (p_square_quantile (acc_w)));

  return octave_value (result);
}
