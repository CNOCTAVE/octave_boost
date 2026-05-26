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
#include <boost/accumulators/statistics/weighted_peaks_over_threshold.hpp>

#include <vector>

namespace ba = boost::accumulators;

DEFUN_DLD (boost_accumulators_peaks_over_threshold_and_variants, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_peaks_over_threshold_and_variants (@var{data}, @var{threshold})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_peaks_over_threshold_and_variants (@var{data}, @var{threshold}, @var{weights})\n\
\n\
Compute Peaks Over Threshold (POT) method for quantile and tail mean\n\
estimation using Boost.Accumulators.\n\
@var{threshold} is the threshold value above which peaks are considered.\n\
Optional @var{weights} is a vector of weights the same size as @var{data}.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{pot_quantile} - POT quantile estimate (right tail)\n\
@item @code{pot_tail_mean} - POT tail mean estimate\n\
@item @code{weighted_pot_quantile} - weighted POT quantile (if weights given)\n\
@item @code{weighted_pot_tail_mean} - weighted POT tail mean (if weights given)\n\
@end itemize\n\
\n\
@example\n\
@group\n\
r = boost_accumulators_peaks_over_threshold_and_variants (randn (1000, 1), 2.0);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2 || args.length () > 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_peaks_over_threshold_and_variants: "
                                        "DATA must be a numeric matrix");

  double threshold = args(1).double_value ();

  bool has_weights = (args.length () >= 3);
  NDArray weights;
  if (has_weights)
    {
      weights = args(2).xarray_value ("boost_accumulators_peaks_over_threshold_and_variants: "
                                       "WEIGHTS must be a numeric vector");
      if (weights.numel () != data.numel ())
        error ("boost_accumulators_peaks_over_threshold_and_variants: "
               "WEIGHTS must have the same number of elements as DATA");
    }

  // POT accumulator using threshold_value (right tail)
  ba::accumulator_set<double, ba::stats<ba::tag::peaks_over_threshold<ba::right>,
                                         ba::tag::pot_quantile<ba::right>,
                                         ba::tag::pot_tail_mean<ba::right> > > acc_pot (
    ba::tag::peaks_over_threshold<ba::right>::threshold_value = threshold
  );

  // Weighted POT accumulator (if weights provided)
  ba::accumulator_set<double, ba::stats<ba::tag::weighted_peaks_over_threshold<ba::right>,
                                         ba::tag::pot_quantile<ba::right>,
                                         ba::tag::pot_tail_mean<ba::right> >, double> acc_wpot (
    ba::tag::peaks_over_threshold<ba::right>::threshold_value = threshold
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
          acc_pot (val);
          if (has_weights)
            {
              double w = weights(i);
              if (! std::isnan (w))
                acc_wpot (val, ba::weight = w);
            }
        }
    }

  if (! any_data)
    error ("boost_accumulators_peaks_over_threshold_and_variants: "
           "DATA contains only NaN values");

  octave_scalar_map result;

  // Extract POT results using find_accumulator with quantile_probability
  result.setfield ("pot_quantile",
                   octave_value (ba::find_accumulator<ba::tag::pot_quantile<ba::right>>(acc_pot).result((ba::accumulator = acc_pot, ba::quantile_probability = 0.99))));
  result.setfield ("pot_tail_mean",
                   octave_value (ba::find_accumulator<ba::tag::pot_tail_mean<ba::right>>(acc_pot).result((ba::accumulator = acc_pot, ba::quantile_probability = 0.99))));

  if (has_weights)
    {
      result.setfield ("weighted_pot_quantile",
                       octave_value (ba::find_accumulator<ba::tag::pot_quantile<ba::right>>(acc_wpot).result((ba::accumulator = acc_wpot, ba::quantile_probability = 0.99))));
      result.setfield ("weighted_pot_tail_mean",
                       octave_value (ba::find_accumulator<ba::tag::pot_tail_mean<ba::right>>(acc_wpot).result((ba::accumulator = acc_wpot, ba::quantile_probability = 0.99))));
    }

  return octave_value (result);
}
