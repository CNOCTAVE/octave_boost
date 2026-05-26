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

DEFUN_DLD (boost_accumulators_min, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{m} =} boost_accumulators_min (@var{data})\n\
\n\
Compute the minimum value of @var{data} using Boost.Accumulators.\n\
\n\
@example\n\
@group\n\
m = boost_accumulators_min ([1, 0, 2, 5, 3]);\n\
@result{} m = 0\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 1)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_min: DATA must be a numeric matrix");

  ba::accumulator_set<double, ba::features<ba::tag::min> > acc;

  const double *ptr = data.data ();
  octave_idx_type n = data.numel ();

  for (octave_idx_type i = 0; i < n; ++i)
    {
      double val = ptr[i];
      if (! std::isnan (val))
        acc (val);
    }

  double result = ba::min (acc);
  return octave_value (result);
}
