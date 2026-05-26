## Copyright (C) 2024-2026 Yu Hongbo <yuhongbo@member.fsf.org>,
##                    CNOCTAVE <cnoctave@qq.com>
##
## This program is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free Software
## Foundation; either version 3 of the License, or (at your option) any later
## version.
##
## This program is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
## details.
##
## You should have received a copy of the GNU General Public License along with
## this program; if not, see <http://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn {} {} demo_boost_accumulators ()
## Run demos for all Boost.Accumulators functions.
## Requires package octave_boost to be loaded.
## @end deftypefn

function demo_boost_accumulators ()
  printf ("\n=== Boost.Accumulators Demo ===\n\n");

  %% 1. count
  printf ("1. boost_accumulators_count\n");
  data = [1, 2, 3, 4, 5];
  cnt = boost_accumulators_count (data);
  printf ("   count(%s) = %d\n", mat2str (data), cnt);

  %% 2. covariance
  printf ("\n2. boost_accumulators_covariance\n");
  data2 = [1, 2; 1, 4; 2, 3; 6, 1];
  c = boost_accumulators_covariance (data2);
  printf ("   covariance = %f\n", c);

  %% 3. density
  printf ("\n3. boost_accumulators_density\n");
  rngdata = randn (1000, 1);
  d = boost_accumulators_density (rngdata);
  printf ("   density returned %d points\n", rows (d));

  %% 4. error_of_mean
  printf ("\n4. boost_accumulators_error_of_mean\n");
  e = boost_accumulators_error_of_mean ([1.1, 1.2, 1.3]);
  printf ("   error_of_mean = %f\n", e);

  %% 5. extended_p_square
  printf ("\n5. boost_accumulators_extended_p_square\n");
  probs = [0.25, 0.5, 0.75];
  q = boost_accumulators_extended_p_square (rngdata, probs);
  printf ("   quantiles: %s\n", mat2str (q));

  %% 6. extended_p_square_quantile_and_variants
  printf ("\n6. boost_accumulators_extended_p_square_quantile_and_variants\n");
  r1 = boost_accumulators_extended_p_square_quantile_and_variants (rngdata, probs);
  printf ("   quantile = %s\n", mat2str (r1.quantile));

  %% 7. kurtosis
  printf ("\n7. boost_accumulators_kurtosis\n");
  k = boost_accumulators_kurtosis ([2, 7, 4, 9, 3]);
  printf ("   kurtosis = %f\n", k);

  %% 8. max
  printf ("\n8. boost_accumulators_max\n");
  m = boost_accumulators_max ([1, 0, 2, 5, 3]);
  printf ("   max = %d\n", m);

  %% 9. mean_and_variants
  printf ("\n9. boost_accumulators_mean_and_variants\n");
  mr = boost_accumulators_mean_and_variants ([1, 0, 2]);
  printf ("   mean = %f, count = %d, sum = %d\n", mr.mean, mr.count, mr.sum);

  printf ("\n   mean with weights:\n");
  mrw = boost_accumulators_mean_and_variants ([1, 0, 2], [2, 4, 9]);
  printf ("   weighted mean = %f, mean_of_weights = %f\n", ...
          mrw.mean, mrw.mean_of_weights);

  %% 10. median_and_variants
  printf ("\n10. boost_accumulators_median_and_variants\n");
  meddata = randn (1000, 1);
  mr2 = boost_accumulators_median_and_variants (meddata);
  printf ("   median(P^2) = %f\n", mr2.median_p_square);
  printf ("   median(density) = %f\n", mr2.median_density);
  printf ("   median(CDist) = %f\n", mr2.median_cdist);

  %% 11. min
  printf ("\n11. boost_accumulators_min\n");
  mn = boost_accumulators_min ([1, 0, 2, 5, 3]);
  printf ("   min = %d\n", mn);

  %% 12. moment
  printf ("\n12. boost_accumulators_moment\n");
  mom = boost_accumulators_moment ([2, 4, 5], 2);
  printf ("   moment(2) = %f\n", mom);

  %% 13. p_square_cumulative_distribution
  printf ("\n13. boost_accumulators_p_square_cumulative_distribution\n");
  hist = boost_accumulators_p_square_cumulative_distribution (rngdata);
  printf ("   CDF returned %d points\n", rows (hist));

  %% 14. p_square_quantile_and_variants
  printf ("\n14. boost_accumulators_p_square_quantile_and_variants\n");
  q50 = boost_accumulators_p_square_quantile_and_variants (rngdata, 0.5);
  printf ("   median (P^2 quantile) = %f\n", q50.quantile);

  %% 15. peaks_over_threshold_and_variants
  printf ("\n15. boost_accumulators_peaks_over_threshold_and_variants\n");
  pot_data = randn (1000, 1);
  pot_r = boost_accumulators_peaks_over_threshold_and_variants (pot_data, 2.0);
  printf ("   POT quantile = %f, tail mean = %f\n", ...
          pot_r.pot_quantile, pot_r.pot_tail_mean);

  %% 16. pot_quantile_and_variants
  printf ("\n16. boost_accumulators_pot_quantile_and_variants\n");
  potq_data = randn (10000, 1);
  potq_r = boost_accumulators_pot_quantile_and_variants (potq_data, 0.999, 3.0);
  printf ("   POT quantile (threshold_value) = %f\n", potq_r.quantile_threshold_value);
  printf ("   POT quantile (threshold_prob) = %f\n", potq_r.quantile_threshold_probability);

  %% 17. pot_tail_mean
  printf ("\n17. boost_accumulators_pot_tail_mean\n");
  pottm_data = randn (10000, 1);
  pottm_r = boost_accumulators_pot_tail_mean (pottm_data, 2.0);
  printf ("   tail mean (right) = %f\n", pottm_r.tail_mean_right);
  printf ("   tail mean (left) = %f\n", pottm_r.tail_mean_left);

  %% 18. skewness
  printf ("\n18. boost_accumulators_skewness\n");
  s = boost_accumulators_skewness ([2, 7, 4, 9, 3]);
  printf ("   skewness = %f\n", s);

  %% 19. sum_and_variants
  printf ("\n19. boost_accumulators_sum_and_variants\n");
  sr = boost_accumulators_sum_and_variants ([1, 2, 3]);
  printf ("   sum = %f, count = %d\n", sr.sum, sr.count);

  printf ("\n   sum with weights:\n");
  sw = boost_accumulators_sum_and_variants ([1, 2, 3], [2, 4, 6]);
  printf ("   weighted sum = %f, sum_of_weights = %f\n", sw.sum, sw.sum_of_weights);

  %% 20. tail
  printf ("\n20. boost_accumulators_tail\n");
  tail_r = boost_accumulators_tail ([8, 3, 1, 5, 9, 2], 3);
  printf ("   right tail: %s\n", mat2str (tail_r.tail));
  tail_l = boost_accumulators_tail ([8, 3, 1, 5, 9, 2], 3, "left");
  printf ("   left tail: %s\n", mat2str (tail_l.tail));

  %% 21. coherent_tail_mean
  printf ("\n21. boost_accumulators_coherent_tail_mean\n");
  ctm_data = randn (10000, 1);
  ctm_r = boost_accumulators_coherent_tail_mean (ctm_data);
  printf ("   coherent_tail_mean_right = %f\n", ctm_r.coherent_tail_mean_right);
  printf ("   coherent_tail_mean_left = %f\n", ctm_r.coherent_tail_mean_left);

  %% 22. non_coherent_tail_mean
  printf ("\n22. boost_accumulators_non_coherent_tail_mean\n");
  nctm_data = randn (10000, 1);
  nctm_r = boost_accumulators_non_coherent_tail_mean (nctm_data);
  printf ("   non_coherent_tail_mean_right = %f\n", nctm_r.non_coherent_tail_mean_right);
  printf ("   non_coherent_tail_mean_left = %f\n", nctm_r.non_coherent_tail_mean_left);

  %% 23. tail_quantile
  printf ("\n23. boost_accumulators_tail_quantile\n");
  tq_data = randn (10000, 1);
  tq_r = boost_accumulators_tail_quantile (tq_data);
  printf ("   tail_quantile (right) = %f\n", tq_r.tail_quantile);
  tq_l = boost_accumulators_tail_quantile (tq_data, 10000, "left");
  printf ("   tail_quantile (left) = %f\n", tq_l.tail_quantile);

  %% 24. tail_variate
  printf ("\n24. boost_accumulators_tail_variate\n");
  tv_data = [8, 3; 5, 7; 9, 2; 3, 6];
  tv_r = boost_accumulators_tail_variate (tv_data, 3);
  printf ("   tail: %s\n", mat2str (tv_r.tail));
  printf ("   tail_variate: %s\n", mat2str (tv_r.tail_variate));

  %% 25. tail_variate_means_and_variants
  printf ("\n25. boost_accumulators_tail_variate_means_and_variants\n");
  tvm_data = [100, 10, 20, 30, 40; 50, 26, 4, 17, 3; 200, 46, 64, 40, 50; ...
               80, 1, 3, 70, 6; 20, 2, 2, 2, 14];
  tvm_r = boost_accumulators_tail_variate_means_and_variants (tvm_data, 5);
  printf ("   relative right: %s\n", mat2str (tvm_r.relative_tail_variate_means_right));
  printf ("   absolute right: %s\n", mat2str (tvm_r.absolute_tail_variate_means_right));

  %% 26. variance_and_variants
  printf ("\n26. boost_accumulators_variance_and_variants\n");
  var_data = [1, 2, 3, 4, 5];
  var_r = boost_accumulators_variance_and_variants (var_data);
  printf ("   lazy variance = %f, mean = %f\n", var_r.variance, var_r.mean);
  var_i = boost_accumulators_variance_and_variants (var_data, "immediate");
  printf ("   immediate variance = %f, mean = %f\n", var_i.variance, var_i.mean);

  %% 27. weighted_covariance
  printf ("\n27. boost_accumulators_weighted_covariance\n");
  wc_data = [1, 2; 1, 4; 2, 3; 6, 1];
  wc = boost_accumulators_weighted_covariance (wc_data);
  printf ("   weighted covariance = %f\n", wc);
  wc_w = boost_accumulators_weighted_covariance (wc_data, [1.1, 2.2, 3.3, 4.4]);
  printf ("   weighted covariance (with weights) = %f\n", wc_w);

  %% 28. weighted_density
  printf ("\n28. boost_accumulators_weighted_density\n");
  wd_data = randn (1000, 1);
  wd_w = rand (1000, 1);
  wd = boost_accumulators_weighted_density (wd_data, wd_w);
  printf ("   weighted density returned %d points\n", rows (wd));

  %% 29. weighted_extended_p_square
  printf ("\n29. boost_accumulators_weighted_extended_p_square\n");
  weps_data = randn (10000, 1);
  weps_w = rand (10000, 1);
  weps_probs = [0.01, 0.1, 0.5, 0.9, 0.99];
  weps_q = boost_accumulators_weighted_extended_p_square (weps_data, weps_probs, weps_w);
  printf ("   weighted extended P^2 quantiles: %s\n", mat2str (weps_q));

  %% 30. weighted_kurtosis
  printf ("\n30. boost_accumulators_weighted_kurtosis\n");
  wk = boost_accumulators_weighted_kurtosis ([2, 7, 4, 9, 3], [4, 1, 3, 1, 2]);
  printf ("   weighted kurtosis = %f\n", wk);

  %% 31. weighted_mean_and_variants
  printf ("\n31. boost_accumulators_weighted_mean_and_variants\n");
  wmr = boost_accumulators_weighted_mean_and_variants ([10, 6, 4, 6], [2, 3, 4, 5]);
  printf ("   weighted_mean = %f, sum_of_weights = %f, count = %d\n", ...
          wmr.weighted_mean, wmr.sum_of_weights, wmr.count);

  %% 32. weighted_median_and_variants
  printf ("\n32. boost_accumulators_weighted_median_and_variants\n");
  wmed_data = randn (1000, 1);
  wmed_w = rand (1000, 1);
  wmed_r = boost_accumulators_weighted_median_and_variants (wmed_data, wmed_w);
  printf ("   weighted median (P^2) = %f\n", wmed_r.weighted_median_p_square);
  printf ("   weighted median (density) = %f\n", wmed_r.weighted_median_density);
  printf ("   weighted median (CDist) = %f\n", wmed_r.weighted_median_cdist);

  %% 33. weighted_moment
  printf ("\n33. boost_accumulators_weighted_moment\n");
  wm2 = boost_accumulators_weighted_moment ([2.1, 2.7, 1.8], [0.7, 1.4, 0.9], 2);
  printf ("   weighted moment (k=2) = %f (expected 5.403)\n", wm2);
  wm7 = boost_accumulators_weighted_moment ([2.1, 2.7, 1.8], [0.7, 1.4, 0.9], 7);
  printf ("   weighted moment (k=7) = %f (expected 548.54182)\n", wm7);

  %% 34. weighted_p_square_cumulative_distribution
  printf ("\n34. boost_accumulators_weighted_p_square_cumulative_distribution\n");
  wcdf_data = randn (10000, 1);
  wcdf_w = rand (10000, 1);
  wcdf_hist = boost_accumulators_weighted_p_square_cumulative_distribution (wcdf_data, wcdf_w);
  printf ("   weighted CDF returned %d points\n", rows (wcdf_hist));

  %% 35. weighted_p_square_quantile_and_variants
  printf ("\n35. boost_accumulators_weighted_p_square_quantile_and_variants\n");
  wq_data = rand (100000, 1);
  wq_w = rand (100000, 1);
  wq = boost_accumulators_weighted_p_square_quantile_and_variants (wq_data, wq_w, 0.5);
  printf ("   weighted median (P^2 quantile) = %f\n", wq);

  %% 36. weighted_peaks_over_threshold_and_variants
  printf ("\n36. boost_accumulators_weighted_peaks_over_threshold_and_variants\n");
  wpot_data = randn (1000, 1);
  wpot_w = rand (1000, 1);
  wpot_r = boost_accumulators_weighted_peaks_over_threshold_and_variants (wpot_data, 2.0, wpot_w);
  printf ("   weighted POT quantile = %f, tail mean = %f\n", ...
          wpot_r.pot_quantile, wpot_r.pot_tail_mean);

  %% 37. weighted_skewness
  printf ("\n37. boost_accumulators_weighted_skewness\n");
  wsk = boost_accumulators_weighted_skewness ([2, 7, 4, 9, 3], [4, 1, 3, 1, 2]);
  printf ("   weighted skewness = %f (expected 1.3071)\n", wsk);

  %% 38. weighted_sum_and_variants
  printf ("\n38. boost_accumulators_weighted_sum_and_variants\n");
  wsum_r = boost_accumulators_weighted_sum_and_variants ([1, 2, 4], [2, 3, 6]);
  printf ("   weighted_sum = %f, sum_of_weights = %f\n", ...
          wsum_r.weighted_sum, wsum_r.sum_of_weights);
  printf ("   weighted_sum_kahan = %f\n", wsum_r.weighted_sum_kahan);

  %% 39. non_coherent_weighted_tail_mean
  printf ("\n39. boost_accumulators_non_coherent_weighted_tail_mean\n");
  nwtm_data = randn (10000, 1);
  nwtm_w = rand (10000, 1);
  nwtm_r = boost_accumulators_non_coherent_weighted_tail_mean (nwtm_data, nwtm_w);
  printf ("   non-coherent weighted tail mean (right) = %f\n", ...
          nwtm_r.non_coherent_tail_mean_right);
  printf ("   non-coherent weighted tail mean (left) = %f\n", ...
          nwtm_r.non_coherent_tail_mean_left);

  %% 40. weighted_tail_quantile
  printf ("\n40. boost_accumulators_weighted_tail_quantile\n");
  wtq_data = randn (10000, 1);
  wtq_w = rand (10000, 1);
  wtq_r = boost_accumulators_weighted_tail_quantile (wtq_data, wtq_w);
  printf ("   weighted tail quantile (right) = %f\n", wtq_r.tail_quantile);
  wtq_l = boost_accumulators_weighted_tail_quantile (wtq_data, wtq_w, 10000, "left");
  printf ("   weighted tail quantile (left) = %f\n", wtq_l.tail_quantile);

  %% 41. weighted_tail_variate_means_and_variants
  printf ("\n41. boost_accumulators_weighted_tail_variate_means_and_variants\n");
  wtvm_data = [100, 10, 20, 30, 40; 50, 26, 4, 17, 3; 200, 46, 64, 40, 50; ...
                80, 1, 3, 70, 6; 20, 2, 2, 2, 14];
  wtvm_w = [0.8; 0.9; 1.0; 1.1; 1.2];
  wtvm_r = boost_accumulators_weighted_tail_variate_means_and_variants (wtvm_data, wtvm_w, 5);
  printf ("   weighted tail variate means (relative, right): %s\n", ...
          mat2str (wtvm_r.relative_tail_variate_means_right));
  printf ("   weighted tail variate means (absolute, right): %s\n", ...
          mat2str (wtvm_r.absolute_tail_variate_means_right));

  %% 42. weighted_variance_and_variants
  printf ("\n42. boost_accumulators_weighted_variance_and_variants\n");
  wvar_data = [1, 2, 3, 4, 5];
  wvar_w = [2, 3, 1, 4, 1];
  wvar_r = boost_accumulators_weighted_variance_and_variants (wvar_data, wvar_w);
  printf ("   weighted variance (lazy) = %f, mean = %f\n", ...
          wvar_r.variance, wvar_r.weighted_mean);
  wvar_i = boost_accumulators_weighted_variance_and_variants (wvar_data, wvar_w, "immediate");
  printf ("   weighted variance (immediate) = %f, mean = %f\n", ...
          wvar_i.variance, wvar_i.weighted_mean);

  printf ("\n=== Demo Complete ===\n");
endfunction
