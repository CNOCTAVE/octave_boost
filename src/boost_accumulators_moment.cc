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

DEFUN_DLD (boost_accumulators_moment, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{m} =} boost_accumulators_moment (@var{data}, @var{k})\n\
\n\
Compute the @var{k}-th moment of @var{data} using Boost.Accumulators.\n\
@var{k} must be a positive integer (1-5).\n\
The @var{k}-th moment is defined as @code{sum(x_i^k) / N}.\n\
\n\
@example\n\
@group\n\
m = boost_accumulators_moment ([2, 4, 5], 2);\n\
@result{} m = 15\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 2)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_moment: DATA must be a numeric matrix");

  int k = args(1).int_value ();
  if (k < 1 || k > 5)
    error ("boost_accumulators_moment: K must be a positive integer between 1 and 5");

  const double *ptr = data.data ();
  octave_idx_type n = data.numel ();

  double result = 0.0;

  switch (k)
    {
    case 1:
      {
        ba::accumulator_set<double, ba::stats<ba::tag::moment<1> > > acc;
        for (octave_idx_type i = 0; i < n; ++i)
          if (! std::isnan (ptr[i])) acc (ptr[i]);
        result = ba::moment<1> (acc);
      }
      break;
    case 2:
      {
        ba::accumulator_set<double, ba::stats<ba::tag::moment<2> > > acc;
        for (octave_idx_type i = 0; i < n; ++i)
          if (! std::isnan (ptr[i])) acc (ptr[i]);
        result = ba::moment<2> (acc);
      }
      break;
    case 3:
      {
        ba::accumulator_set<double, ba::stats<ba::tag::moment<3> > > acc;
        for (octave_idx_type i = 0; i < n; ++i)
          if (! std::isnan (ptr[i])) acc (ptr[i]);
        result = ba::moment<3> (acc);
      }
      break;
    case 4:
      {
        ba::accumulator_set<double, ba::stats<ba::tag::moment<4> > > acc;
        for (octave_idx_type i = 0; i < n; ++i)
          if (! std::isnan (ptr[i])) acc (ptr[i]);
        result = ba::moment<4> (acc);
      }
      break;
    case 5:
      {
        ba::accumulator_set<double, ba::stats<ba::tag::moment<5> > > acc;
        for (octave_idx_type i = 0; i < n; ++i)
          if (! std::isnan (ptr[i])) acc (ptr[i]);
        result = ba::moment<5> (acc);
      }
      break;
    }

  return octave_value (result);
}
