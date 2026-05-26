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

// NOTE: We avoid the Boost.Accumulators tail() extractor here because
// Boost 1.85+ has an internal template instantiation issue where
// extractor<tag::abstract_tail> cannot resolve result_type when the
// accumulator_set only has tag::tail<LeftRight>.  Instead we maintain
// a std::multimap that pairs each sample value with its covariate and
// extract the top/bottom N entries directly.

#include <octave/oct.h>

#include <vector>
#include <map>
#include <functional>
#include <algorithm>

DEFUN_DLD (boost_accumulators_tail_variate, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_tail_variate (@var{data})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_tail_variate (@var{data}, @var{cache_size})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_tail_variate (@var{data}, @var{cache_size}, @var{tail_type})\n\
\n\
Track the covariates of the largest or smallest N samples using\n\
Boost.Accumulators. @var{data} must be an Nx2 matrix where the first\n\
column is the sample value and the second column is the covariate.\n\
Optional @var{cache_size} (default 10) specifies how many extreme values\n\
to keep.  Optional @var{tail_type} is 'right' (default) or 'left'.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{tail} - vector of extreme sample values\n\
@item @code{tail_variate} - vector of corresponding covariate values\n\
@item @code{tail_type} - 'right' or 'left'\n\
@item @code{count} - number of values in the tail\n\
@end itemize\n\
\n\
@example\n\
@group\n\
r = boost_accumulators_tail_variate ([8, 3; 5, 7; 9, 2; 3, 6], 3);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1 || args.length () > 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_tail_variate: DATA must be a numeric matrix");

  if (data.columns () < 2)
    error ("boost_accumulators_tail_variate: DATA must have at least 2 columns (value and covariate)");

  std::size_t cache_size = 10;
  if (args.length () >= 2)
    cache_size = args(1).xuint64_value ("boost_accumulators_tail_variate: CACHE_SIZE must be a positive integer");

  bool right_tail = true;
  if (args.length () >= 3)
    {
      std::string tail_type = args(2).xstring_value ("boost_accumulators_tail_variate: TAIL_TYPE must be 'right' or 'left'");
      if (tail_type == "left")
        right_tail = false;
      else if (tail_type != "right")
        error ("boost_accumulators_tail_variate: TAIL_TYPE must be 'right' or 'left'");
    }

  octave_idx_type n = data.rows ();

  // Strategy: maintain a multimap of (sample_value, covariate) pairs sorted
  // in descending order (largest first).  For right tail we take the first
  // min(cache_size, N) entries; for left tail we take the last entries
  // (smallest values) by reverse iteration.

  std::multimap<double, double, std::greater<double>> sorted;

  bool any_data = false;

  for (octave_idx_type i = 0; i < n; ++i)
    {
      double val = data(i, 0);
      double cov = data(i, 1);
      if (! std::isnan (val) && ! std::isnan (cov))
        {
          any_data = true;
          sorted.insert (std::make_pair (val, cov));
        }
    }

  if (! any_data)
    error ("boost_accumulators_tail_variate: DATA contains no valid (non-NaN) rows");

  std::size_t m = std::min (cache_size, sorted.size ());

  NDArray tail_vals (dim_vector (1, m));
  NDArray cov_vals (dim_vector (1, m));

  if (right_tail)
    {
      // Right tail: largest values are at the beginning (greater ordering)
      auto it = sorted.begin ();
      for (std::size_t i = 0; i < m; ++i, ++it)
        {
          tail_vals(i) = it->first;
          cov_vals(i) = it->second;
        }

      octave_scalar_map result;
      result.setfield ("tail", octave_value (tail_vals));
      result.setfield ("tail_variate", octave_value (cov_vals));
      result.setfield ("tail_type", octave_value ("right"));
      result.setfield ("count", octave_value (static_cast<double> (m)));
      return octave_value (result);
    }
  else
    {
      // Left tail: smallest values are at the end (reverse of greater ordering)
      auto it = sorted.rbegin ();
      for (std::size_t i = 0; i < m; ++i, ++it)
        {
          tail_vals(i) = it->first;
          cov_vals(i) = it->second;
        }

      octave_scalar_map result;
      result.setfield ("tail", octave_value (tail_vals));
      result.setfield ("tail_variate", octave_value (cov_vals));
      result.setfield ("tail_type", octave_value ("left"));
      result.setfield ("count", octave_value (static_cast<double> (m)));
      return octave_value (result);
    }
}
