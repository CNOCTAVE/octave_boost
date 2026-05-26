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

using namespace boost::accumulators;

DEFUN_DLD (boost_accumulators_weighted_tail_quantile, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_weighted_tail_quantile (@var{data}, @var{weights})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_weighted_tail_quantile (@var{data}, @var{weights}, @var{cache_size})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_weighted_tail_quantile (@var{data}, @var{weights}, @var{cache_size}, @var{tail_type})\n\
\n\
Tail quantile estimation based on order statistics of weighted samples\n\
for both left and right tails using Boost.Accumulators.\n\
@var{weights} is a vector of weights the same size as @var{data}.\n\
Optional @var{cache_size} (default 10000) specifies the tail cache size.\n\
Optional @var{tail_type} is 'right' (default) or 'left'.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{tail_quantile} - estimated tail quantile\n\
@item @code{tail_type} - 'right' or 'left'\n\
@end itemize\n\
\n\
@example\n\
@group\n\
w = rand (10000, 1);\n\
r = boost_accumulators_weighted_tail_quantile (randn (10000, 1), w);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2 || args.length () > 4)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_weighted_tail_quantile: "
                                        "DATA must be a numeric matrix");

  NDArray weights = args(1).xarray_value ("boost_accumulators_weighted_tail_quantile: "
                                           "WEIGHTS must be a numeric matrix");

  if (weights.numel () != data.numel ())
    error ("boost_accumulators_weighted_tail_quantile: "
           "WEIGHTS must have the same number of elements as DATA");

  std::size_t cache_size = 10000;
  if (args.length () >= 3)
    {
      cache_size = args(2).xuint64_value ("boost_accumulators_weighted_tail_quantile: "
                                           "CACHE_SIZE must be a positive integer >= 2");
      if (cache_size < 2)
        error ("boost_accumulators_weighted_tail_quantile: "
               "CACHE_SIZE must be a positive integer >= 2");
    }

  bool right_tail = true;
  if (args.length () >= 4)
    {
      std::string tail_type = args(3).xstring_value ("boost_accumulators_weighted_tail_quantile: "
                                                       "TAIL_TYPE must be 'right' or 'left'");
      if (tail_type == "left")
        right_tail = false;
      else if (tail_type != "right")
        error ("boost_accumulators_weighted_tail_quantile: "
               "TAIL_TYPE must be 'right' or 'left'");
    }

  const double *ptr = data.data ();
  const double *wptr = weights.data ();
  octave_idx_type n = data.numel ();
  bool any_data = false;

  octave_scalar_map result;

  if (right_tail)
    {
      accumulator_set<double, stats<tag::tail_quantile<right> >, double> acc(
        tag::tail<right>::cache_size = cache_size
      );

      for (octave_idx_type i = 0; i < n; ++i)
        {
          double val = ptr[i];
          double w = wptr[i];
          if (! std::isnan (val) && ! std::isnan (w))
            {
              any_data = true;
              acc (val, weight = w);
            }
        }

      if (! any_data)
        error ("boost_accumulators_weighted_tail_quantile: "
               "DATA contains only NaN values");

      result.setfield ("tail_quantile", octave_value (
        find_accumulator<tag::tail_quantile<right>>(acc).result(
          (accumulator = acc, quantile_probability = 0.95)
        )
      ));
      result.setfield ("tail_type", octave_value ("right"));
    }
  else
    {
      accumulator_set<double, stats<tag::tail_quantile<left> >, double> acc(
        tag::tail<left>::cache_size = cache_size
      );

      for (octave_idx_type i = 0; i < n; ++i)
        {
          double val = ptr[i];
          double w = wptr[i];
          if (! std::isnan (val) && ! std::isnan (w))
            {
              any_data = true;
              acc (val, weight = w);
            }
        }

      if (! any_data)
        error ("boost_accumulators_weighted_tail_quantile: "
               "DATA contains only NaN values");

      result.setfield ("tail_quantile", octave_value (
        find_accumulator<tag::tail_quantile<left>>(acc).result(
          (accumulator = acc, quantile_probability = 0.05)
        )
      ));
      result.setfield ("tail_type", octave_value ("left"));
    }

  return octave_value (result);
}

