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

DEFUN_DLD (boost_accumulators_variance_and_variants, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_variance_and_variants (@var{data})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_variance_and_variants (@var{data}, @var{method})\n\
\n\
Compute the variance of @var{data} using Boost.Accumulators with\n\
both lazy and immediate (iterative) methods.\n\
Optional @var{method} is 'lazy' (default) or 'immediate'.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{variance} - variance estimate\n\
@item @code{mean} - arithmetic mean\n\
@item @code{count} - number of elements\n\
@item @code{method} - computation method used\n\
@end itemize\n\
\n\
@example\n\
@group\n\
r = boost_accumulators_variance_and_variants ([1, 2, 3, 4, 5]);\n\
@result{} r.variance = 2, r.mean = 3, r.count = 5\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1 || args.length () > 2)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_variance_and_variants: DATA must be a numeric matrix");

  bool lazy_method = true;
  if (args.length () >= 2)
    {
      std::string method = args(1).xstring_value ("boost_accumulators_variance_and_variants: METHOD must be 'lazy' or 'immediate'");
      if (method == "immediate")
        lazy_method = false;
      else if (method != "lazy")
        error ("boost_accumulators_variance_and_variants: METHOD must be 'lazy' or 'immediate'");
    }

  const double *ptr = data.data ();
  octave_idx_type n = data.numel ();

  octave_scalar_map result;

  if (lazy_method)
    {
      // Lazy variance (computes on demand)
      ba::accumulator_set<double, ba::stats<ba::tag::variance(ba::lazy)> > acc;

      for (octave_idx_type i = 0; i < n; ++i)
        {
          double val = ptr[i];
          if (! std::isnan (val))
            acc (val);
        }

      result.setfield ("variance", octave_value (ba::variance (acc)));
      result.setfield ("mean", octave_value (ba::mean (acc)));
      result.setfield ("count", octave_value (static_cast<double> (ba::count (acc))));
      result.setfield ("method", octave_value ("lazy"));
    }
  else
    {
      // Immediate variance (iterative computation)
      ba::accumulator_set<double, ba::stats<ba::tag::variance> > acc;

      for (octave_idx_type i = 0; i < n; ++i)
        {
          double val = ptr[i];
          if (! std::isnan (val))
            acc (val);
        }

      result.setfield ("variance", octave_value (ba::variance (acc)));
      result.setfield ("mean", octave_value (ba::mean (acc)));
      result.setfield ("count", octave_value (static_cast<double> (ba::count (acc))));
      result.setfield ("method", octave_value ("immediate"));
    }

  return octave_value (result);
}
