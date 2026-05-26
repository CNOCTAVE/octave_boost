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

DEFUN_DLD (boost_accumulators_tail_quantile, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_tail_quantile (@var{data})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_tail_quantile (@var{data}, @var{cache_size})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_tail_quantile (@var{data}, @var{cache_size}, @var{tail_type})\n\
\n\
Tail quantile estimation based on order statistics for both left and\n\
right tails using Boost.Accumulators.\n\
Optional @var{cache_size} (default 10000) specifies the tail cache size.\n\
Optional @var{tail_type} is 'right' (default) or 'left'.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{tail_quantile} - estimated tail quantile value\n\
@item @code{tail_type} - 'right' or 'left'\n\
@end itemize\n\
\n\
@example\n\
@group\n\
r = boost_accumulators_tail_quantile (randn (10000, 1));\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1 || args.length () > 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_tail_quantile: "
                                        "DATA must be a numeric matrix");

  std::size_t cache_size = 10000;
  if (args.length () >= 2)
    {
      cache_size = args(1).uint64_value ();
      if (cache_size < 2)
        error ("boost_accumulators_tail_quantile: "
               "CACHE_SIZE must be a positive integer >= 2");
    }

  bool right_tail = true;
  if (args.length () >= 3)
    {
      std::string tail_type = args(2).string_value ();
      if (tail_type == "left")
        right_tail = false;
      else if (tail_type != "right")
        error ("boost_accumulators_tail_quantile: "
               "TAIL_TYPE must be 'right' or 'left'");
    }

  const double *ptr = data.data ();
  octave_idx_type n = data.numel ();
  bool any_data = false;

  octave_scalar_map result;

  if (right_tail)
    {
      ba::accumulator_set<double, ba::stats<ba::tag::tail_quantile<ba::right> > > acc(
        ba::tag::tail<ba::right>::cache_size = cache_size
      );

      for (octave_idx_type i = 0; i < n; ++i)
        {
          double val = ptr[i];
          if (! std::isnan (val))
            {
              any_data = true;
              acc (val);
            }
        }

      if (! any_data)
        error ("boost_accumulators_tail_quantile: DATA contains only NaN values");

      result.setfield ("tail_quantile", octave_value (ba::find_accumulator<ba::tag::tail_quantile<ba::right>>(acc).result((ba::accumulator = acc, ba::quantile_probability = 0.95))));
      result.setfield ("tail_type", octave_value ("right"));
    }
  else
    {
      ba::accumulator_set<double, ba::stats<ba::tag::tail_quantile<ba::left> > > acc(
        ba::tag::tail<ba::left>::cache_size = cache_size
      );

      for (octave_idx_type i = 0; i < n; ++i)
        {
          double val = ptr[i];
          if (! std::isnan (val))
            {
              any_data = true;
              acc (val);
            }
        }

      if (! any_data)
        error ("boost_accumulators_tail_quantile: DATA contains only NaN values");

      result.setfield ("tail_quantile", octave_value (ba::find_accumulator<ba::tag::tail_quantile<ba::left>>(acc).result((ba::accumulator = acc, ba::quantile_probability = 0.05))));
      result.setfield ("tail_type", octave_value ("left"));
    }

  return octave_value (result);
}
