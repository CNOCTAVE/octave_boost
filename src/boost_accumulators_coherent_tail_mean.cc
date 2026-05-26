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
#include <boost/accumulators/statistics/tail.hpp>

#include <vector>

namespace ba = boost::accumulators;

DEFUN_DLD (boost_accumulators_coherent_tail_mean, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_coherent_tail_mean (@var{data})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_coherent_tail_mean (@var{data}, @var{cache_size})\n\
\n\
Estimate the coherent tail mean based on order statistics for both left\n\
and right tails using Boost.Accumulators.\n\
Optional @var{cache_size} (default 10000) specifies the tail cache size.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{coherent_tail_mean_right} - right tail coherent tail mean\n\
@item @code{coherent_tail_mean_left} - left tail coherent tail mean\n\
@item @code{tail_quantile_right} - right tail quantile (for alpha=0.95)\n\
@item @code{tail_quantile_left} - left tail quantile (for alpha=0.05)\n\
@end itemize\n\
\n\
@example\n\
@group\n\
r = boost_accumulators_coherent_tail_mean (randn (10000, 1));\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1 || args.length () > 2)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_coherent_tail_mean: DATA must be a numeric matrix");

  std::size_t cache_size = 10000;
  if (args.length () >= 2)
    {
      cache_size = args(1).xuint64_value ("boost_accumulators_coherent_tail_mean: CACHE_SIZE must be a positive integer >= 2");
      if (cache_size < 2)
        error ("boost_accumulators_coherent_tail_mean: CACHE_SIZE must be a positive integer >= 2");
    }

  // Use a single accumulator per tail direction that includes both
  // coherent_tail_mean and tail_quantile.  Boost.Accumulators requires
  // tail_quantile to be explicitly listed in the stats so that the
  // abstract quantile dependency of coherent_tail_mean can be resolved.

  // Right tail accumulator (coherent_tail_mean + tail_quantile)
  ba::accumulator_set<double, ba::stats<ba::tag::coherent_tail_mean<ba::right>,
                                        ba::tag::tail_quantile<ba::right> > > acc_right(
    ba::right_tail_cache_size = cache_size
  );

  // Left tail accumulator (coherent_tail_mean + tail_quantile)
  ba::accumulator_set<double, ba::stats<ba::tag::coherent_tail_mean<ba::left>,
                                        ba::tag::tail_quantile<ba::left> > > acc_left(
    ba::left_tail_cache_size = cache_size
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
        }
    }

  if (! any_data)
    error ("boost_accumulators_coherent_tail_mean: DATA contains only NaN values");

  octave_scalar_map result;

  // coherent_tail_mean and tail_quantile need quantile_probability
  // passed explicitly alongside the accumulator set.
  // Note: the extra parentheses force the comma to be the Boost.Parameter
  // argument-pack operator rather than the C++ function-call argument separator.
  auto ctm_right = ba::find_accumulator<ba::tag::coherent_tail_mean<ba::right>>(acc_right).result(
    (ba::accumulator = acc_right, ba::quantile_probability = 0.95)
  );
  auto ctm_left = ba::find_accumulator<ba::tag::coherent_tail_mean<ba::left>>(acc_left).result(
    (ba::accumulator = acc_left, ba::quantile_probability = 0.05)
  );
  auto tq_right = ba::find_accumulator<ba::tag::tail_quantile<ba::right>>(acc_right).result(
    (ba::accumulator = acc_right, ba::quantile_probability = 0.95)
  );
  auto tq_left = ba::find_accumulator<ba::tag::tail_quantile<ba::left>>(acc_left).result(
    (ba::accumulator = acc_left, ba::quantile_probability = 0.05)
  );

  result.setfield ("coherent_tail_mean_right", octave_value (ctm_right));
  result.setfield ("coherent_tail_mean_left",  octave_value (ctm_left));
  result.setfield ("tail_quantile_right",       octave_value (tq_right));
  result.setfield ("tail_quantile_left",        octave_value (tq_left));

  return octave_value (result);
}
