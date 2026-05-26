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
#include <boost/accumulators/statistics/weighted_moment.hpp>

namespace ba = boost::accumulators;

DEFUN_DLD (boost_accumulators_weighted_moment, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{m} =} boost_accumulators_weighted_moment (@var{data}, @var{weights}, @var{k})\n\
\n\
Compute the weighted k-th moment of @var{data} using Boost.Accumulators.\n\
@var{weights} is a vector of weights the same size as @var{data}.\n\
@var{k} is the order of the moment (positive integer, 1-5).\n\
The k-th weighted moment is defined as sum(w_i * x_i^k) / sum(w_i).\n\
\n\
@example\n\
@group\n\
m = boost_accumulators_weighted_moment ([2.1, 2.7, 1.8], [0.7, 1.4, 0.9], 2);\n\
@result{} m = 5.403\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_weighted_moment: "
                                        "DATA must be a numeric matrix");

  NDArray weights = args(1).xarray_value ("boost_accumulators_weighted_moment: "
                                           "WEIGHTS must be a numeric matrix");

  if (weights.numel () != data.numel ())
    error ("boost_accumulators_weighted_moment: "
           "WEIGHTS must have the same number of elements as DATA");

  int k = args(2).int_value ();
  if (k < 1 || k > 5)
    error ("boost_accumulators_weighted_moment: K must be a positive integer between 1 and 5");

  const double *ptr = data.data ();
  const double *wptr = weights.data ();
  octave_idx_type n = data.numel ();
  bool any_data = false;

  double result = 0.0;

  // Dispatch based on moment order k
  switch (k)
    {
    case 1:
      {
        ba::accumulator_set<double, ba::stats<ba::tag::weighted_moment<1> >, double> acc;
        for (octave_idx_type i = 0; i < n; ++i)
          {
            double val = ptr[i];
            double w = wptr[i];
            if (! std::isnan (val) && ! std::isnan (w))
              {
                any_data = true;
                acc (val, ba::weight = w);
              }
          }
        if (! any_data)
          error ("boost_accumulators_weighted_moment: DATA contains only NaN values");
        result = ba::weighted_moment<1> (acc);
        break;
      }
    case 2:
      {
        ba::accumulator_set<double, ba::stats<ba::tag::weighted_moment<2> >, double> acc;
        for (octave_idx_type i = 0; i < n; ++i)
          {
            double val = ptr[i];
            double w = wptr[i];
            if (! std::isnan (val) && ! std::isnan (w))
              {
                any_data = true;
                acc (val, ba::weight = w);
              }
          }
        if (! any_data)
          error ("boost_accumulators_weighted_moment: DATA contains only NaN values");
        result = ba::weighted_moment<2> (acc);
        break;
      }
    case 3:
      {
        ba::accumulator_set<double, ba::stats<ba::tag::weighted_moment<3> >, double> acc;
        for (octave_idx_type i = 0; i < n; ++i)
          {
            double val = ptr[i];
            double w = wptr[i];
            if (! std::isnan (val) && ! std::isnan (w))
              {
                any_data = true;
                acc (val, ba::weight = w);
              }
          }
        if (! any_data)
          error ("boost_accumulators_weighted_moment: DATA contains only NaN values");
        result = ba::weighted_moment<3> (acc);
        break;
      }
    case 4:
      {
        ba::accumulator_set<double, ba::stats<ba::tag::weighted_moment<4> >, double> acc;
        for (octave_idx_type i = 0; i < n; ++i)
          {
            double val = ptr[i];
            double w = wptr[i];
            if (! std::isnan (val) && ! std::isnan (w))
              {
                any_data = true;
                acc (val, ba::weight = w);
              }
          }
        if (! any_data)
          error ("boost_accumulators_weighted_moment: DATA contains only NaN values");
        result = ba::weighted_moment<4> (acc);
        break;
      }
    case 5:
      {
        ba::accumulator_set<double, ba::stats<ba::tag::weighted_moment<5> >, double> acc;
        for (octave_idx_type i = 0; i < n; ++i)
          {
            double val = ptr[i];
            double w = wptr[i];
            if (! std::isnan (val) && ! std::isnan (w))
              {
                any_data = true;
                acc (val, ba::weight = w);
              }
          }
        if (! any_data)
          error ("boost_accumulators_weighted_moment: DATA contains only NaN values");
        result = ba::weighted_moment<5> (acc);
        break;
      }
    }

  return octave_value (result);
}
