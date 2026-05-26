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

#include <vector>

using namespace boost::accumulators;

DEFUN_DLD (boost_accumulators_p_square_cumulative_distribution, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{hist} =} boost_accumulators_p_square_cumulative_distribution (@var{data})\n\
@deftypefnx {Loadable Function} {@var{hist} =} boost_accumulators_p_square_cumulative_distribution (@var{data}, @var{num_cells})\n\
\n\
Compute the cumulative distribution histogram using the P^2 algorithm\n\
from Boost.Accumulators.\n\
Optional @var{num_cells} (default 100) sets the number of histogram bins.\n\
Returns an Nx2 matrix where column 1 contains the values and column 2\n\
contains the cumulative probabilities.\n\
\n\
@example\n\
@group\n\
hist = boost_accumulators_p_square_cumulative_distribution (randn (1000, 1), 10);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1 || args.length () > 2)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_p_square_cumulative_distribution: "
                                        "DATA must be a numeric matrix");

  std::size_t num_cells = 100;
  if (args.length () >= 2)
    {
      num_cells = args(1).uint64_value ();
      if (num_cells < 2)
        error ("boost_accumulators_p_square_cumulative_distribution: "
               "NUM_CELLS must be a positive integer >= 2");
    }

  accumulator_set<double, stats<tag::p_square_cumulative_distribution> > acc(
    tag::p_square_cumulative_distribution::num_cells = num_cells
  );

  const double *ptr = data.data ();
  octave_idx_type n = data.numel ();

  for (octave_idx_type i = 0; i < n; ++i)
    {
      double val = ptr[i];
      if (! std::isnan (val))
        acc (val);
    }

  // Extract histogram: iterator_range of pairs (value, cumulative_prob)
  auto pdf_result = p_square_cumulative_distribution (acc);
  std::size_t num_points = pdf_result.size ();

  NDArray out (dim_vector (num_points, 2));

  for (std::size_t i = 0; i < num_points; ++i)
    {
      out(i, 0) = pdf_result[i].first;   // value
      out(i, 1) = pdf_result[i].second;  // cumulative probability
    }

  return octave_value (out);
}
