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

#include <octave/oct.h>

namespace ba = boost::accumulators;

DEFUN_DLD (boost_accumulators_sum_and_variants, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_sum_and_variants (@var{data})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_sum_and_variants (@var{data}, @var{weights})\n\
\n\
Compute the sum and its variants (sum, sum_of_weights, sum_of_variates)\n\
using Boost.Accumulators.\n\
If @var{weights} is provided, weighted statistics are computed.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{sum} - (weighted) sum of elements\n\
@item @code{sum_of_weights} - sum of weights (if weights provided)\n\
@item @code{sum_kahan} - Kahan-compensated sum (if no weights)\n\
@item @code{count} - number of non-NaN elements\n\
@end itemize\n\
\n\
@example\n\
@group\n\
r = boost_accumulators_sum_and_variants ([1, 2, 3]);\n\
@result{} r.sum = 6, r.count = 3\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1 || args.length () > 2)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_sum_and_variants: DATA must be a numeric matrix");

  bool has_weights = (args.length () >= 2);
  NDArray weights;
  if (has_weights)
    {
      weights = args(1).xarray_value ("boost_accumulators_sum_and_variants: WEIGHTS must be a numeric matrix");
      if (weights.numel () != data.numel ())
        error ("boost_accumulators_sum_and_variants: "
               "WEIGHTS must have the same number of elements as DATA");
    }

  const double *ptr = data.data ();
  octave_idx_type n = data.numel ();

  octave_scalar_map result;

  if (has_weights)
    {
      // Weighted sum
      ba::accumulator_set<double, ba::stats<ba::tag::sum, ba::tag::count, ba::tag::sum_of_weights,
                                    ba::tag::sum_of_variates<double, ba::tag::covariate1> >,
                      double> acc;

      const double *wptr = weights.data ();

      for (octave_idx_type i = 0; i < n; ++i)
        {
          double val = ptr[i];
          double w = wptr[i];
          if (! std::isnan (val) && ! std::isnan (w))
            acc (val, ba::weight = w, ba::covariate1 = val);
        }

      result.setfield ("sum", octave_value (ba::sum (acc)));
      result.setfield ("sum_of_weights", octave_value (ba::sum_of_weights (acc)));
      result.setfield ("sum_of_variates", octave_value (ba::sum_of_variates (acc)));
      result.setfield ("count", octave_value (static_cast<double> (ba::count (acc))));
    }
  else
    {
      // Unweighted sum with Kahan summation
      ba::accumulator_set<double, ba::stats<ba::tag::sum, ba::tag::sum_kahan> > acc;
      ba::accumulator_set<double, ba::features<ba::tag::count> > acc_cnt;

      for (octave_idx_type i = 0; i < n; ++i)
        {
          double val = ptr[i];
          if (! std::isnan (val))
            {
              acc (val);
              acc_cnt (val);
            }
        }

      result.setfield ("sum", octave_value (ba::sum (acc)));
      result.setfield ("sum_kahan", octave_value (ba::sum_kahan (acc)));
      result.setfield ("count", octave_value (static_cast<double> (ba::count (acc_cnt))));
    }

  return octave_value (result);
}
