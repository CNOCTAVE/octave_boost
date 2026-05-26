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

namespace ba = boost::accumulators;

DEFUN_DLD (boost_accumulators_extended_p_square_quantile_and_variants, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_extended_p_square_quantile_and_variants (@var{data}, @var{probabilities})\n\
\n\
Compute quantile estimates using the extended P^2 algorithm with\n\
multiple accumulator variants (unweighted, weighted, quadratic,\n\
weighted quadratic) from Boost.Accumulators.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{quantile} - basic extended P^2 quantile\n\
@item @code{quantile_quadratic} - quadratic extended P^2 quantile\n\
@item @code{weighted_quantile} - weighted extended P^2 quantile\n\
@item @code{weighted_quantile_quadratic} - weighted quadratic extended P^2 quantile\n\
@end itemize\n\
\n\
@example\n\
@group\n\
probs = [0.25, 0.5, 0.75];\n\
r = boost_accumulators_extended_p_square_quantile_and_variants (randn (1000, 1), probs);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 2)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_extended_p_square_quantile_and_variants: DATA must be a numeric matrix");

  NDArray probs = args(1).xarray_value ("boost_accumulators_extended_p_square_quantile_and_variants: PROBABILITIES must be a numeric vector");

  octave_idx_type nprobs = probs.numel ();
  if (nprobs < 1)
    error ("boost_accumulators_extended_p_square_quantile_and_variants: "
           "PROBABILITIES must contain at least one probability");

  if (nprobs > 20)
    error ("boost_accumulators_extended_p_square_quantile_and_variants: "
           "at most 20 probabilities are supported");

  std::vector<double> prob_vec (nprobs);
  for (octave_idx_type i = 0; i < nprobs; ++i)
    prob_vec[i] = probs(i);

  // Four accumulator variants
  ba::accumulator_set<double, ba::stats<ba::tag::extended_p_square_quantile> > acc (
    ba::extended_p_square_probabilities = prob_vec
  );

  ba::accumulator_set<double, ba::stats<ba::tag::weighted_extended_p_square_quantile>, double > acc_weighted (
    ba::extended_p_square_probabilities = prob_vec
  );

  ba::accumulator_set<double, ba::stats<ba::tag::extended_p_square_quantile(ba::quadratic)> > acc_quad (
    ba::extended_p_square_probabilities = prob_vec
  );

  ba::accumulator_set<double, ba::stats<ba::tag::weighted_extended_p_square_quantile(ba::quadratic)>, double > acc_wquad (
    ba::extended_p_square_probabilities = prob_vec
  );

  const double *ptr = data.data ();
  octave_idx_type n = data.numel ();

  for (octave_idx_type i = 0; i < n; ++i)
    {
      double val = ptr[i];
      if (! std::isnan (val))
        {
          acc (val);
          acc_quad (val);
          acc_weighted (val, ba::weight = 1.);
          acc_wquad (val, ba::weight = 1.);
        }
    }

  // Build result struct for each probability
  octave_scalar_map result;

  NDArray quantile_vals (dim_vector (1, nprobs));
  NDArray quantile_quad_vals (dim_vector (1, nprobs));
  NDArray wquantile_vals (dim_vector (1, nprobs));
  NDArray wquantile_quad_vals (dim_vector (1, nprobs));

  // Compute quantile for each probability
  // Note: extended_p_square_quantile returns the quantile for given probability
  // We use quantile() with quantile_probability
  for (octave_idx_type i = 0; i < nprobs; ++i)
    {
      quantile_vals(i) = ba::quantile (acc, ba::quantile_probability = prob_vec[i]);
      quantile_quad_vals(i) = ba::quantile (acc_quad, ba::quantile_probability = prob_vec[i]);
      wquantile_vals(i) = ba::quantile (acc_weighted, ba::quantile_probability = prob_vec[i]);
      wquantile_quad_vals(i) = ba::quantile (acc_wquad, ba::quantile_probability = prob_vec[i]);
    }

  result.setfield ("quantile", octave_value (quantile_vals));
  result.setfield ("quantile_quadratic", octave_value (quantile_quad_vals));
  result.setfield ("weighted_quantile", octave_value (wquantile_vals));
  result.setfield ("weighted_quantile_quadratic", octave_value (wquantile_quad_vals));

  return octave_value (result);
}
