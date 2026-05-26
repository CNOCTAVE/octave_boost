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
#include <boost/accumulators/statistics/peaks_over_threshold.hpp>

#include <vector>

namespace ba = boost::accumulators;

DEFUN_DLD (boost_accumulators_pot_tail_mean, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_pot_tail_mean (@var{data}, @var{threshold})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_pot_tail_mean (@var{data}, @var{threshold}, @var{weights})\n\
\n\
Estimate the (coherent) tail mean based on the Peaks Over Threshold (POT)\n\
method for both left and right tails using Boost.Accumulators.\n\
@var{threshold} is the threshold value.\n\
Optional @var{weights} is a vector of weights the same size as @var{data}.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{tail_mean_right} - right tail mean estimate\n\
@item @code{tail_mean_left} - left tail mean estimate\n\
@item @code{tail_mean_weighted_right} - weighted right tail mean (if weights given)\n\
@item @code{tail_mean_weighted_left} - weighted left tail mean (if weights given)\n\
@end itemize\n\
\n\
@example\n\
@group\n\
r = boost_accumulators_pot_tail_mean (randn (10000, 1), 2.0);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2 || args.length () > 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_pot_tail_mean: DATA must be a numeric matrix");

  double threshold = args(1).double_value ();

  bool has_weights = (args.length () >= 3);
  NDArray weights;
  if (has_weights)
    {
      weights = args(2).xarray_value ("boost_accumulators_pot_tail_mean: WEIGHTS must be a numeric vector");
      if (weights.numel () != data.numel ())
        error ("boost_accumulators_pot_tail_mean: "
               "WEIGHTS must have the same number of elements as DATA");
    }

  // Right tail POT
  ba::accumulator_set<double, ba::stats<ba::tag::peaks_over_threshold<ba::right>,
                                         ba::tag::pot_tail_mean<ba::right> > > acc_right (
    ba::tag::peaks_over_threshold<ba::right>::threshold_value = threshold
  );

  // Left tail POT
  ba::accumulator_set<double, ba::stats<ba::tag::peaks_over_threshold<ba::left>,
                                         ba::tag::pot_tail_mean<ba::left> > > acc_left (
    ba::tag::peaks_over_threshold<ba::left>::threshold_value = threshold
  );

  // Weighted versions
  ba::accumulator_set<double, ba::stats<ba::tag::weighted_peaks_over_threshold<ba::right>,
                                         ba::tag::pot_tail_mean<ba::right> >, double> acc_wright (
    ba::tag::peaks_over_threshold<ba::right>::threshold_value = threshold
  );

  ba::accumulator_set<double, ba::stats<ba::tag::weighted_peaks_over_threshold<ba::left>,
                                         ba::tag::pot_tail_mean<ba::left> >, double> acc_wleft (
    ba::tag::peaks_over_threshold<ba::left>::threshold_value = threshold
  );

  const double *ptr = data.data ();
  octave_idx_type n = data.numel ();
  bool any_data = false;

  for (octave_idx_type i = 0; i < n; ++i)
    {
      double val = ptr[i];
      if (! std::isnan (val))
        {
          any_data = true;
          acc_right (val);
          acc_left (val);
          if (has_weights)
            {
              double w = weights(i);
              if (! std::isnan (w))
                {
                  acc_wright (val, ba::weight = w);
                  acc_wleft (val, ba::weight = w);
                }
            }
        }
    }

  if (! any_data)
    error ("boost_accumulators_pot_tail_mean: DATA contains only NaN values");

  octave_scalar_map result;

  // Use find_accumulator with quantile_probability to get POT tail mean
  result.setfield ("tail_mean_right",
                   octave_value (ba::find_accumulator<ba::tag::pot_tail_mean<ba::right>>(acc_right).result(
                     (ba::accumulator = acc_right, ba::quantile_probability = 0.99))));
  result.setfield ("tail_mean_left",
                   octave_value (ba::find_accumulator<ba::tag::pot_tail_mean<ba::left>>(acc_left).result(
                     (ba::accumulator = acc_left, ba::quantile_probability = 0.01))));

  if (has_weights)
    {
      result.setfield ("tail_mean_weighted_right",
                       octave_value (ba::find_accumulator<ba::tag::pot_tail_mean<ba::right>>(acc_wright).result(
                         (ba::accumulator = acc_wright, ba::quantile_probability = 0.99))));
      result.setfield ("tail_mean_weighted_left",
                       octave_value (ba::find_accumulator<ba::tag::pot_tail_mean<ba::left>>(acc_wleft).result(
                         (ba::accumulator = acc_wleft, ba::quantile_probability = 0.01))));
    }

  return octave_value (result);
}
