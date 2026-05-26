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

DEFUN_DLD (boost_accumulators_mean_and_variants, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_mean_and_variants (@var{data})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_mean_and_variants (@var{data}, @var{weights})\n\
\n\
Compute the mean and its variants (mean, count, sum, mean of weights,\n\
mean of variates) of @var{data} using Boost.Accumulators.\n\
If @var{weights} is provided, weighted statistics are computed.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{mean} - arithmetic mean\n\
@item @code{count} - number of elements\n\
@item @code{sum} - sum of elements\n\
@item @code{mean_of_weights} - mean of weights (if weights provided)\n\
@end itemize\n\
\n\
@example\n\
@group\n\
r = boost_accumulators_mean_and_variants ([1, 0, 2]);\n\
@result{} r.mean = 1.0, r.count = 3, r.sum = 3\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1 || args.length () > 2)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_mean_and_variants: DATA must be a numeric matrix");

  bool has_weights = (args.length () >= 2);
  NDArray weights;
  if (has_weights)
    {
      weights = args(1).xarray_value ("boost_accumulators_mean_and_variants: WEIGHTS must be a numeric matrix");
      if (weights.numel () != data.numel ())
        error ("boost_accumulators_mean_and_variants: "
               "WEIGHTS must have the same number of elements as DATA");
    }

  // Build result struct
  octave_scalar_map result;

  if (has_weights)
    {
      // Weighted mean
      ba::accumulator_set<double, ba::stats<ba::tag::mean, ba::tag::sum, ba::tag::mean_of_weights>, double> acc;

      const double *dptr = data.data ();
      const double *wptr = weights.data ();
      octave_idx_type n = data.numel ();

      for (octave_idx_type i = 0; i < n; ++i)
        {
          double val = dptr[i];
          double w = wptr[i];
          if (! std::isnan (val) && ! std::isnan (w))
            acc (val, ba::weight = w);
        }

      result.setfield ("mean", octave_value (ba::mean (acc)));
      result.setfield ("count", octave_value (static_cast<double> (ba::count (acc))));
      result.setfield ("sum", octave_value (ba::sum (acc)));
      result.setfield ("mean_of_weights", octave_value (ba::mean_of_weights (acc)));
    }
  else
    {
      // Unweighted mean
      ba::accumulator_set<double, ba::stats<ba::tag::mean, ba::tag::sum> > acc;

      const double *ptr = data.data ();
      octave_idx_type n = data.numel ();

      for (octave_idx_type i = 0; i < n; ++i)
        {
          double val = ptr[i];
          if (! std::isnan (val))
            acc (val);
        }

      result.setfield ("mean", octave_value (ba::mean (acc)));
      result.setfield ("count", octave_value (static_cast<double> (ba::count (acc))));
      result.setfield ("sum", octave_value (ba::sum (acc)));
    }

  return octave_value (result);
}
