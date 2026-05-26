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

// Must be defined BEFORE any boost/accumulators/numeric/functional.hpp includes,
// so that std::vector<double> gets operator+ support needed by tail_variate_means_impl.
#define BOOST_NUMERIC_FUNCTIONAL_STD_VECTOR_SUPPORT

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/accumulators/statistics/tail.hpp>
#include <boost/accumulators/statistics/tail_variate.hpp>
#include <boost/accumulators/statistics/tail_variate_means.hpp>

#include <octave/oct.h>

#include <vector>

using namespace boost::accumulators;

DEFUN_DLD (boost_accumulators_weighted_tail_variate_means_and_variants, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_accumulators_weighted_tail_variate_means_and_variants (@var{data}, @var{weights})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_accumulators_weighted_tail_variate_means_and_variants (@var{data}, @var{weights}, @var{cache_size})\n\
\n\
Estimate the absolute and relative weighted tail variate means for both\n\
left and right tails using Boost.Accumulators.\n\
@var{data} must be an NxM matrix where the first column is the sample value\n\
and the remaining columns are the variates.\n\
@var{weights} is a vector of weights the same size as number of rows of data.\n\
Optional @var{cache_size} (default 5) specifies the tail cache size.\n\
Returns a struct with fields:\n\
@itemize\n\
@item @code{relative_tail_variate_means_right} - relative means (right tail)\n\
@item @code{absolute_tail_variate_means_right} - absolute means (right tail)\n\
@item @code{relative_tail_variate_means_left} - relative means (left tail)\n\
@item @code{absolute_tail_variate_means_left} - absolute means (left tail)\n\
@end itemize\n\
\n\
@example\n\
@group\n\
data = [100, 10, 20, 30, 40; 50, 26, 4, 17, 3; 200, 46, 64, 40, 50; \\\n\
        80, 1, 3, 70, 6; 20, 2, 2, 2, 14];\n\
w = [0.8; 0.9; 1.0; 1.1; 1.2];\n\
r = boost_accumulators_weighted_tail_variate_means_and_variants (data, w, 5);\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2 || args.length () > 3)
    print_usage ();

  NDArray data = args(0).xarray_value ("boost_accumulators_weighted_tail_variate_means_and_variants: "
                                        "DATA must be a numeric matrix");

  if (data.columns () < 2)
    error ("boost_accumulators_weighted_tail_variate_means_and_variants: "
           "DATA must have at least 2 columns (sample + variates)");

  NDArray weights = args(1).xarray_value ("boost_accumulators_weighted_tail_variate_means_and_variants: "
                                           "WEIGHTS must be a numeric matrix");

  if (static_cast<octave_idx_type> (weights.numel ()) != data.rows ())
    error ("boost_accumulators_weighted_tail_variate_means_and_variants: "
           "WEIGHTS must have the same number of elements as DATA rows");

  std::size_t cache_size = 5;
  if (args.length () >= 3)
    {
      cache_size = args(2).xuint64_value ("boost_accumulators_weighted_tail_variate_means_and_variants: "
                                           "CACHE_SIZE must be a positive integer");
      if (cache_size < 1)
        error ("boost_accumulators_weighted_tail_variate_means_and_variants: "
               "CACHE_SIZE must be a positive integer");
    }

  octave_idx_type n_variates = data.columns () - 1;
  typedef std::vector<double> variate_set_type;

  // Right tail accumulators with relative and absolute
  accumulator_set<double, stats<
    tag::tail_variate_means<right, variate_set_type, tag::covariate1>(relative)
  >, double> acc_rel_right(right_tail_cache_size = cache_size);

  accumulator_set<double, stats<
    tag::tail_variate_means<right, variate_set_type, tag::covariate1>(absolute)
  >, double> acc_abs_right(right_tail_cache_size = cache_size);

  // Left tail accumulators with relative and absolute
  accumulator_set<double, stats<
    tag::tail_variate_means<left, variate_set_type, tag::covariate1>(relative)
  >, double> acc_rel_left(left_tail_cache_size = cache_size);

  accumulator_set<double, stats<
    tag::tail_variate_means<left, variate_set_type, tag::covariate1>(absolute)
  >, double> acc_abs_left(left_tail_cache_size = cache_size);

  octave_idx_type n = data.rows ();
  bool any_data = false;

  for (octave_idx_type i = 0; i < n; ++i)
    {
      double sample = data(i, 0);
      double w = weights(i);
      if (std::isnan (sample) || std::isnan (w))
        continue;

      any_data = true;
      variate_set_type cov (n_variates);
      for (octave_idx_type j = 0; j < n_variates; ++j)
        cov[j] = data(i, j + 1);

      acc_rel_right (sample, weight = w, covariate1 = cov);
      acc_abs_right (sample, weight = w, covariate1 = cov);
      acc_rel_left (sample, weight = w, covariate1 = cov);
      acc_abs_left (sample, weight = w, covariate1 = cov);
    }

  if (! any_data)
    error ("boost_accumulators_weighted_tail_variate_means_and_variants: "
           "DATA contains only NaN values");

  // Helper lambda to convert iterator_range to NDArray
  auto to_ndarray = [] (const auto &range) -> NDArray
  {
    std::size_t sz = range.size ();
    NDArray out (dim_vector (1, sz));
    for (std::size_t i = 0; i < sz; ++i)
      out(i) = range[i];
    return out;
  };

  octave_scalar_map result;

  // Right tail: relative (quantile_probability = 0.7 default)
  auto rel_right = relative_weighted_tail_variate_means (acc_rel_right, quantile_probability = 0.7);
  result.setfield ("relative_tail_variate_means_right",
                   octave_value (to_ndarray (rel_right)));

  // Right tail: absolute
  auto abs_right = weighted_tail_variate_means (acc_abs_right, quantile_probability = 0.7);
  result.setfield ("absolute_tail_variate_means_right",
                   octave_value (to_ndarray (abs_right)));

  // Left tail: relative (quantile_probability = 0.3 default)
  auto rel_left = relative_weighted_tail_variate_means (acc_rel_left, quantile_probability = 0.3);
  result.setfield ("relative_tail_variate_means_left",
                   octave_value (to_ndarray (rel_left)));

  // Left tail: absolute
  auto abs_left = weighted_tail_variate_means (acc_abs_left, quantile_probability = 0.3);
  result.setfield ("absolute_tail_variate_means_left",
                   octave_value (to_ndarray (abs_left)));

  return octave_value (result);
}

