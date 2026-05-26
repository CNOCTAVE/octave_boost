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

DEFUN_DLD (boost_accumulators_pot_quantile_and_variants, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_pot_quantile_and_variants (@var{data}, @var{alpha})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_pot_quantile_and_variants (@var{data}, @var{alpha}, @var{threshold})\n\
\n\
Compute quantile estimation based on Peaks Over Threshold (POT) method\n\
using Boost.Accumulators.\n\
@var{alpha} is the quantile probability (0 < alpha < 1).\n\
Optional @var{threshold} specifies the threshold value.\n\
If not given, @var{threshold_probability} = 0.99 is used with cache_size = 2000.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{quantile_threshold_value} - POT quantile with fixed threshold\n\
@item @code{quantile_threshold_probability} - POT quantile with probability threshold\n\
@end itemize\n\
\n\
@example\n\
@group\n\
r = boost_accumulators_pot_quantile_and_variants (randn (10000, 1), 0.999, 3.0);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2 || args.length () > 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_pot_quantile_and_variants: "
                                        "DATA must be a numeric matrix");

  double alpha = args(1).double_value ();
  if (alpha <= 0.0 || alpha >= 1.0)
    error ("boost_accumulators_pot_quantile_and_variants: "
           "ALPHA must be a scalar in (0,1)");

  bool has_threshold = (args.length () >= 3);
  double threshold_value = 3.0;
  double threshold_prob = 0.99;
  std::size_t cache_size = 2000;

  if (has_threshold)
    {
      threshold_value = args(2).double_value ();
    }

  // POT with threshold_value method (right tail)
  ba::accumulator_set<double, ba::stats<ba::tag::peaks_over_threshold<ba::right>,
                                         ba::tag::pot_quantile<ba::right> > > acc1_pot(
    ba::tag::peaks_over_threshold<ba::right>::threshold_value = threshold_value
  );

  // POT with threshold_probability method (right tail)
  ba::accumulator_set<double, ba::stats<ba::tag::peaks_over_threshold_prob<ba::right>,
                                         ba::tag::pot_quantile<ba::right> > > acc2_pot(
    ba::tag::tail<ba::right>::cache_size = cache_size,
    ba::tag::peaks_over_threshold_prob<ba::right>::threshold_probability = threshold_prob
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
          acc1_pot (val);
          acc2_pot (val);
        }
    }

  if (! any_data)
    error ("boost_accumulators_pot_quantile_and_variants: "
           "DATA contains only NaN values");

  octave_scalar_map result;

  // Use extractor<tag::pot_quantile> to get POT quantile from the accumulator set
  result.setfield ("quantile_threshold_value",
                   octave_value (ba::find_accumulator<ba::tag::pot_quantile<ba::right> >(acc1_pot).result((ba::accumulator = acc1_pot, ba::quantile_probability = alpha))));
  result.setfield ("quantile_threshold_probability",
                   octave_value (ba::find_accumulator<ba::tag::pot_quantile<ba::right> >(acc2_pot).result((ba::accumulator = acc2_pot, ba::quantile_probability = alpha))));

  return octave_value (result);
}
