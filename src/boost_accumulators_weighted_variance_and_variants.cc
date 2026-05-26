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
#include <boost/accumulators/statistics/weighted_variance.hpp>

using namespace boost::accumulators;

DEFUN_DLD (boost_accumulators_weighted_variance_and_variants, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_weighted_variance_and_variants (@var{data}, @var{weights})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_weighted_variance_and_variants (@var{data}, @var{weights}, @var{method})\n\
\n\
Compute the weighted variance of @var{data} using Boost.Accumulators\n\
with both lazy (computes on demand) and immediate (iterative) methods.\n\
@var{weights} is a vector of weights the same size as @var{data}.\n\
Optional @var{method} is 'lazy' (default) or 'immediate'.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{variance} - weighted variance estimate\n\
@item @code{weighted_mean} - weighted mean\n\
@item @code{count} - number of elements\n\
@item @code{method} - computation method used ('lazy' or 'immediate')\n\
@end itemize\n\
\n\
@example\n\
@group\n\
r = boost_accumulators_weighted_variance_and_variants ([1, 2, 3, 4, 5], [2, 3, 1, 4, 1]);\n\
@result{} r.variance = 1.719\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2 || args.length () > 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_weighted_variance_and_variants: "
                                        "DATA must be a numeric matrix");

  NDArray weights = args(1).xarray_value ("boost_accumulators_weighted_variance_and_variants: "
                                           "WEIGHTS must be a numeric matrix");

  if (weights.numel () != data.numel ())
    error ("boost_accumulators_weighted_variance_and_variants: "
           "WEIGHTS must have the same number of elements as DATA");

  bool lazy_method = true;
  if (args.length () >= 3)
    {
      std::string method = args(2).xstring_value ("boost_accumulators_weighted_variance_and_variants: "
                                                    "METHOD must be 'lazy' or 'immediate'");
      if (method == "immediate")
        lazy_method = false;
      else if (method != "lazy")
        error ("boost_accumulators_weighted_variance_and_variants: "
               "METHOD must be 'lazy' or 'immediate'");
    }

  const double *ptr = data.data ();
  const double *wptr = weights.data ();
  octave_idx_type n = data.numel ();

  octave_scalar_map result;

  if (lazy_method)
    {
      // Lazy weighted variance (computes on demand)
      accumulator_set<double, stats<tag::weighted_variance(lazy)>, double> acc;

      for (octave_idx_type i = 0; i < n; ++i)
        {
          double val = ptr[i];
          double w = wptr[i];
          if (! std::isnan (ptr[i]) && ! std::isnan (wptr[i]))
            acc (val, weight = w);
        }

      result.setfield ("variance", octave_value (weighted_variance (acc)));
      result.setfield ("weighted_mean", octave_value (weighted_mean (acc)));
      result.setfield ("count", octave_value (static_cast<double> (count (acc))));
      result.setfield ("method", octave_value ("lazy"));
    }
  else
    {
      // Immediate weighted variance (iterative computation)
      accumulator_set<double, stats<tag::weighted_variance>, double> acc;

      for (octave_idx_type i = 0; i < n; ++i)
        {
          double val = ptr[i];
          double w = wptr[i];
          if (! std::isnan (ptr[i]) && ! std::isnan (wptr[i]))
            acc (val, weight = w);
        }

      result.setfield ("variance", octave_value (weighted_variance (acc)));
      result.setfield ("weighted_mean", octave_value (weighted_mean (acc)));
      result.setfield ("count", octave_value (static_cast<double> (count (acc))));
      result.setfield ("method", octave_value ("immediate"));
    }

  return octave_value (result);
}

