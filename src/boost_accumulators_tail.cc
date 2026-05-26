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
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/tail.hpp>
#include <boost/accumulators/statistics/tail_quantile.hpp>

#include <vector>
#include <algorithm>

using namespace boost::accumulators;

DEFUN_DLD (boost_accumulators_tail, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_tail (@var{data})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_tail (@var{data}, @var{cache_size})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_tail (@var{data}, @var{cache_size}, @var{tail_type})\n\
\n\
Return the largest (right tail) or smallest (left tail) N values\n\
from @var{data} using Boost.Accumulators.\n\
Optional @var{cache_size} (default 10) specifies how many extreme values to keep.\n\
Optional @var{tail_type} is 'right' (default) or 'left'.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{tail} - vector of extreme values\n\
@item @code{tail_type} - 'right' or 'left'\n\
@item @code{count} - number of values in the tail\n\
@end itemize\n\
\n\
@example\n\
@group\n\
r = boost_accumulators_tail ([8, 3, 1, 5, 9, 2], 3);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1 || args.length () > 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_tail: DATA must be a numeric matrix");

  std::size_t cache_size = 10;
  if (args.length () >= 2)
    {
      cache_size = args(1).uint64_value ();
      if (cache_size < 1)
        error ("boost_accumulators_tail: CACHE_SIZE must be a positive integer");
    }

  bool right_tail = true;
  if (args.length () >= 3)
    {
      std::string tail_type = args(2).string_value ();
      if (tail_type == "left")
        right_tail = false;
      else if (tail_type != "right")
        error ("boost_accumulators_tail: TAIL_TYPE must be 'right' or 'left'");
    }

  const double *ptr = data.data ();
  octave_idx_type n = data.numel ();
  bool any_data = false;

  // Using double type throughout, with a plain tag::tail<LeftRight> stat.
  // No tail_variate is needed for simple tail extraction.
  if (right_tail)
    {
      accumulator_set<double, stats<tag::tail<right> > > acc (
        tag::tail<right>::cache_size = cache_size
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
        error ("boost_accumulators_tail: DATA contains only NaN values");

      // tail() returns an iterator_range<const double*>
      auto t = tail (acc);
      std::size_t m = t.size ();

      NDArray tail_vals (dim_vector (1, m));
      for (std::size_t i = 0; i < m; ++i)
        tail_vals(i) = t[i];

      octave_scalar_map result;
      result.setfield ("tail", octave_value (tail_vals));
      result.setfield ("tail_type", octave_value ("right"));
      result.setfield ("count", octave_value (static_cast<double> (m)));

      return octave_value (result);
    }
  else
    {
      accumulator_set<double, stats<tag::tail<left> > > acc (
        tag::tail<left>::cache_size = cache_size
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
        error ("boost_accumulators_tail: DATA contains only NaN values");

      auto t = tail (acc);
      std::size_t m = t.size ();

      NDArray tail_vals (dim_vector (1, m));
      for (std::size_t i = 0; i < m; ++i)
        tail_vals(i) = t[i];

      octave_scalar_map result;
      result.setfield ("tail", octave_value (tail_vals));
      result.setfield ("tail_type", octave_value ("left"));
      result.setfield ("count", octave_value (static_cast<double> (m)));

      return octave_value (result);
    }
}
