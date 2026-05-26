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

DEFUN_DLD (boost_accumulators_weighted_peaks_over_threshold_and_variants, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_weighted_peaks_over_threshold_and_variants (@var{data}, @var{threshold}, @var{weights})\n\
\n\
Compute the weighted Peaks Over Threshold (POT) method for weighted\n\
quantile and weighted tail mean estimation.\n\
@var{threshold} is the threshold value above which peaks are considered.\n\
@var{weights} is a vector of weights the same size as @var{data}.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{pot_quantile} - POT quantile estimate (right tail)\n\
@item @code{pot_tail_mean} - POT tail mean estimate\n\
@end itemize\n\
\n\
@example\n\
@group\n\
w = rand (1000, 1);\n\
r = boost_accumulators_weighted_peaks_over_threshold_and_variants (randn (1000, 1), 2.0, w);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_weighted_peaks_over_threshold_and_variants: "
           "DATA must be a numeric matrix");

  double threshold = args(1).xdouble_value ("boost_accumulators_weighted_peaks_over_threshold_and_variants: "
           "THRESHOLD must be a numeric scalar");

  NDArray weights = args(2).xarray_value ("boost_accumulators_weighted_peaks_over_threshold_and_variants: "
           "WEIGHTS must be a numeric matrix");

  if (weights.numel () != data.numel ())
    error ("boost_accumulators_weighted_peaks_over_threshold_and_variants: "
           "WEIGHTS must have the same number of elements as DATA");

  // Weighted POT accumulator
  ba::accumulator_set<double, ba::stats<ba::tag::weighted_peaks_over_threshold<ba::right>,
                                         ba::tag::pot_quantile<ba::right>,
                                         ba::tag::pot_tail_mean<ba::right> >, double> acc (
    ba::tag::peaks_over_threshold<ba::right>::threshold_value = threshold
  );

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
    error ("boost_accumulators_weighted_peaks_over_threshold_and_variants: "
           "DATA contains only NaN values");

  octave_scalar_map result;

  result.setfield ("pot_quantile", octave_value (ba::find_accumulator<ba::tag::pot_quantile<ba::right>>(acc).result((ba::accumulator = acc, ba::quantile_probability = 0.99))));
  result.setfield ("pot_tail_mean", octave_value (ba::find_accumulator<ba::tag::pot_tail_mean<ba::right>>(acc).result((ba::accumulator = acc, ba::quantile_probability = 0.99))));

  return octave_value (result);
}
