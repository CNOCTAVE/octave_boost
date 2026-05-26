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

%% -*- texinfo -*-
%% @deftypefn {} {} demo_boost_date_time ()
%% Demo script for Boost.Date_Time functions in octave_boost.
%%
%% This script demonstrates all 40 Boost.Date_Time functions included
%% in the octave_boost package.
%%
%% Usage:
%% @example
%% pkg load octave_boost
%% demo_boost_date_time
%% @end example
%% @end deftypefn

function demo_boost_date_time ()

  printf ("\n=== octave_boost - Boost.Date_Time Demonstration ===\n\n");

  %% ===== 1. boost_date_get_year =====
  printf ("1. boost_date_get_year:\n");
  y = boost_date_get_year (2002, 1, 10);
  printf ("   boost_date_get_year (2002, 1, 10) = %d\n", y);

  % Using a struct
  date_struct.year = 2002;
  date_struct.month = 1;
  date_struct.day = 10;
  y2 = boost_date_get_year (date_struct);
  printf ("   Using struct: year=%d (expected 2002)\n", y2);

  % Using a string
  y3 = boost_date_get_year ("2002-01-10");
  printf ("   Using string '2002-01-10': year=%d (expected 2002)\n", y3);
  printf ("\n");

  %% ===== 2. boost_date_get_month =====
  printf ("2. boost_date_get_month:\n");
  m = boost_date_get_month (2002, 1, 10);
  printf ("   boost_date_get_month (2002, 1, 10) = %d (expected 1)\n", m);
  printf ("\n");

  %% ===== 3. boost_date_get_day =====
  printf ("3. boost_date_get_day:\n");
  d = boost_date_get_day (2002, 1, 10);
  printf ("   boost_date_get_day (2002, 1, 10) = %d (expected 10)\n", d);
  printf ("\n");

  %% ===== 4. boost_date_get_year_month_day =====
  printf ("4. boost_date_get_year_month_day:\n");
  ymd = boost_date_get_year_month_day (2002, 1, 10);
  printf ("   year=%d, month=%d, day=%d (expected 2002, 1, 10)\n",
          ymd.year, ymd.month, ymd.day);
  printf ("\n");

  %% ===== 5. boost_date_get_day_of_week =====
  printf ("5. boost_date_get_day_of_week:\n");
  dow = boost_date_get_day_of_week (2002, 1, 10);
  printf ("   Jan 10, 2002 is day_of_week=%d (0=Sunday, 4=Thursday)\n", dow);
  printf ("\n");

  %% ===== 6. boost_date_get_day_of_year =====
  printf ("6. boost_date_get_day_of_year:\n");
  doy = boost_date_get_day_of_year (2000, 1, 10);
  printf ("   Jan 10, 2000 is day_of_year=%d (expected 10)\n", doy);

  % Leap year test
  doy2 = boost_date_get_day_of_year (2000, 3, 1);
  printf ("   Mar 1, 2000 is day_of_year=%d (leap year, expected 61)\n", doy2);
  printf ("\n");

  %% ===== 7. boost_date_get_end_of_month =====
  printf ("7. boost_date_get_end_of_month:\n");
  eom = boost_date_get_end_of_month (2000, 1, 10);
  printf ("   End of Jan 2000: year=%d, month=%d, day=%d (expected 2000,1,31)\n",
          eom.year, eom.month, eom.day);

  % Leap year Feb
  eom2 = boost_date_get_end_of_month (2000, 2, 15);
  printf ("   End of Feb 2000: year=%d, month=%d, day=%d (leap, expected 2000,2,29)\n",
          eom2.year, eom2.month, eom2.day);

  % Non-leap year Feb
  eom3 = boost_date_get_end_of_month (2001, 2, 15);
  printf ("   End of Feb 2001: year=%d, month=%d, day=%d (non-leap, expected 2001,2,28)\n",
          eom3.year, eom3.month, eom3.day);
  printf ("\n");

  %% ===== 8. boost_date_is_infinity =====
  printf ("8. boost_date_is_infinity:\n");
  tf1 = boost_date_is_infinity ('pos_infin');
  printf ("   'pos_infin' is infinity = %d (expected 1)\n", tf1);

  tf2 = boost_date_is_infinity ('neg_infin');
  printf ("   'neg_infin' is infinity = %d (expected 1)\n", tf2);

  tf3 = boost_date_is_infinity (2002, 1, 10);
  printf ("   2002-01-10 is infinity = %d (expected 0)\n", tf3);
  printf ("\n");

  %% ===== 9. boost_date_is_neg_infinity =====
  printf ("9. boost_date_is_neg_infinity:\n");
  nf1 = boost_date_is_neg_infinity ('neg_infin');
  printf ("   'neg_infin' is neg_infinity = %d (expected 1)\n", nf1);

  nf2 = boost_date_is_neg_infinity ('pos_infin');
  printf ("   'pos_infin' is neg_infinity = %d (expected 0)\n", nf2);
  printf ("\n");

  %% ===== 10. boost_date_is_pos_infinity =====
  printf ("10. boost_date_is_pos_infinity:\n");
  pf1 = boost_date_is_pos_infinity ('pos_infin');
  printf ("    'pos_infin' is pos_infinity = %d (expected 1)\n", pf1);

  pf2 = boost_date_is_pos_infinity ('neg_infin');
  printf ("    'neg_infin' is pos_infinity = %d (expected 0)\n", pf2);
  printf ("\n");

  %% ===== 11. boost_date_is_not_a_date =====
  printf ("11. boost_date_is_not_a_date:\n");
  nad1 = boost_date_is_not_a_date ('not_a_date_time');
  printf ("    'not_a_date_time' is_not_a_date = %d (expected 1)\n", nad1);

  nad2 = boost_date_is_not_a_date (2002, 1, 10);
  printf ("    2002-01-10 is_not_a_date = %d (expected 0)\n", nad2);
  printf ("\n");

  %% ===== 12. boost_date_is_special =====
  printf ("12. boost_date_is_special:\n");
  sp1 = boost_date_is_special ('pos_infin');
  printf ("    'pos_infin' is_special = %d (expected 1)\n", sp1);

  sp2 = boost_date_is_special ('not_a_date_time');
  printf ("    'not_a_date_time' is_special = %d (expected 1)\n", sp2);

  sp3 = boost_date_is_special (2005, 3, 1);
  printf ("    2005-03-01 is_special = %d (expected 0)\n", sp3);
  printf ("\n");

  %% ===== 13. boost_date_modjulian_day =====
  printf ("13. boost_date_modjulian_day:\n");
  mjd = boost_date_modjulian_day (2002, 1, 10);
  printf ("    MJD of 2002-01-10 = %d (expected 52284)\n", mjd);
  printf ("\n");

  %% ===== 14. boost_date_julian_day =====
  printf ("14. boost_date_julian_day:\n");
  jd = boost_date_julian_day (2002, 1, 10);
  printf ("    JD of 2002-01-10 = %d (expected 2452284)\n", jd);
  printf ("\n");

  %% ===== 15. boost_date_week_number =====
  printf ("15. boost_date_week_number:\n");
  wn = boost_date_week_number (2002, 1, 10);
  printf ("    ISO week number of 2002-01-10 = %d (expected 2)\n", wn);
  printf ("\n");

  %% ===== 16. boost_date_end_of_month =====
  printf ("16. boost_date_end_of_month:\n");
  eom4 = boost_date_end_of_month (2000, 2, 1);
  printf ("    End of Feb 2000: year=%d, month=%d, day=%d (leap, expected 2000,2,29)\n",
          eom4.year, eom4.month, eom4.day);

  eom5 = boost_date_end_of_month (2001, 2, 1);
  printf ("    End of Feb 2001: year=%d, month=%d, day=%d (non-leap, expected 2001,2,28)\n",
          eom5.year, eom5.month, eom5.day);
  printf ("\n");

  %% ===== 17. boost_date_to_simple_string =====
  printf ("17. boost_date_to_simple_string:\n");
  sstr = boost_date_to_simple_string (2002, 1, 1);
  printf ("    to_simple_string (2002,1,1) = '%s' (expected '2002-Jan-01')\n", sstr);
  printf ("\n");

  %% ===== 18. boost_date_to_iso_string =====
  printf ("18. boost_date_to_iso_string:\n");
  istr = boost_date_to_iso_string (2002, 1, 31);
  printf ("    to_iso_string (2002,1,31) = '%s' (expected '20020131')\n", istr);
  printf ("\n");

  %% ===== 19. boost_date_to_iso_extended_string =====
  printf ("19. boost_date_to_iso_extended_string:\n");
  eistr = boost_date_to_iso_extended_string (2002, 1, 31);
  printf ("    to_iso_extended_string (2002,1,31) = '%s' (expected '2002-01-31')\n", eistr);
  printf ("\n");

  %% ===== 20. boost_date_eq =====
  printf ("20. boost_date_eq:\n");
  eq1 = boost_date_eq (2002, 1, 10, 2002, 1, 10);
  printf ("    2002-01-10 == 2002-01-10 = %d (expected 1)\n", eq1);

  eq2 = boost_date_eq (2002, 1, 10, 2002, 1, 11);
  printf ("    2002-01-10 == 2002-01-11 = %d (expected 0)\n", eq2);
  printf ("\n");

  %% ===== Special values =====
  printf ("--- Special value handling ---\n");
  try
    % not_a_date_time should error for extractors
    y_nan = boost_date_get_year ('not_a_date_time');
    printf ("   WARNING: not_a_date_time returned year=%d (unexpected)\n", y_nan);
  catch
    printf ("   boost_date_get_year('not_a_date_time') correctly raised an error\n");
  end_try_catch

  %% ===== 21. boost_date_ne =====
  printf ("21. boost_date_ne:\n");
  ne1 = boost_date_ne (2002, 1, 10, 2002, 1, 11);
  printf ("    2002-01-10 != 2002-01-11 = %d (expected 1)\n", ne1);
  ne2 = boost_date_ne (2002, 1, 10, 2002, 1, 10);
  printf ("    2002-01-10 != 2002-01-10 = %d (expected 0)\n", ne2);
  printf ("\n");

  %% ===== 22. boost_date_gt =====
  printf ("22. boost_date_gt:\n");
  gt1 = boost_date_gt (2002, 1, 11, 2002, 1, 10);
  printf ("    2002-01-11 > 2002-01-10 = %d (expected 1)\n", gt1);
  gt2 = boost_date_gt (2002, 1, 10, 2002, 1, 11);
  printf ("    2002-01-10 > 2002-01-11 = %d (expected 0)\n", gt2);
  printf ("\n");

  %% ===== 23. boost_date_lt =====
  printf ("23. boost_date_lt:\n");
  lt1 = boost_date_lt (2002, 1, 10, 2002, 1, 11);
  printf ("    2002-01-10 < 2002-01-11 = %d (expected 1)\n", lt1);
  lt2 = boost_date_lt (2002, 1, 11, 2002, 1, 10);
  printf ("    2002-01-11 < 2002-01-10 = %d (expected 0)\n", lt2);
  printf ("\n");

  %% ===== 24. boost_date_ge =====
  printf ("24. boost_date_ge:\n");
  ge1 = boost_date_ge (2002, 1, 11, 2002, 1, 10);
  printf ("    2002-01-11 >= 2002-01-10 = %d (expected 1)\n", ge1);
  ge2 = boost_date_ge (2002, 1, 10, 2002, 1, 10);
  printf ("    2002-01-10 >= 2002-01-10 = %d (expected 1)\n", ge2);
  ge3 = boost_date_ge (2002, 1, 9, 2002, 1, 10);
  printf ("    2002-01-09 >= 2002-01-10 = %d (expected 0)\n", ge3);
  printf ("\n");

  %% ===== 25. boost_date_le =====
  printf ("25. boost_date_le:\n");
  le1 = boost_date_le (2002, 1, 10, 2002, 1, 11);
  printf ("    2002-01-10 <= 2002-01-11 = %d (expected 1)\n", le1);
  le2 = boost_date_le (2002, 1, 10, 2002, 1, 10);
  printf ("    2002-01-10 <= 2002-01-10 = %d (expected 1)\n", le2);
  le3 = boost_date_le (2002, 1, 11, 2002, 1, 10);
  printf ("    2002-01-11 <= 2002-01-10 = %d (expected 0)\n", le3);
  printf ("\n");

  %% ===== 26. boost_date_add_days =====
  printf ("26. boost_date_add_days:\n");
  add_d = boost_date_add_days (2002, 1, 1, 10);
  printf ("    2002-01-01 + 10 days = %d-%02d-%02d (expected 2002-01-11)\n",
          add_d.year, add_d.month, add_d.day);
  % Using struct input
  ds.year = 2002; ds.month = 1; ds.day = 28;
  add_d2 = boost_date_add_days (ds, 3);
  printf ("    2002-01-28 + 3 days = %d-%02d-%02d (expected 2002-01-31)\n",
          add_d2.year, add_d2.month, add_d2.day);
  printf ("\n");

  %% ===== 27. boost_date_minus_days =====
  printf ("27. boost_date_minus_days:\n");
  sub_d = boost_date_minus_days (2002, 1, 10, 5);
  printf ("    2002-01-10 - 5 days = %d-%02d-%02d (expected 2002-01-05)\n",
          sub_d.year, sub_d.month, sub_d.day);
  sub_d2 = boost_date_minus_days (2002, 3, 1, 1);
  printf ("    2002-03-01 - 1 day = %d-%02d-%02d (expected 2002-02-28)\n",
          sub_d2.year, sub_d2.month, sub_d2.day);
  printf ("\n");

  %% ===== 28. boost_date_minus_date =====
  printf ("28. boost_date_minus_date:\n");
  diff1 = boost_date_minus_date (2002, 1, 11, 2002, 1, 10);
  printf ("    2002-01-11 - 2002-01-10 = %d days (expected 1)\n", diff1);
  diff2 = boost_date_minus_date (2002, 1, 10, 2002, 1, 11);
  printf ("    2002-01-10 - 2002-01-11 = %d days (expected -1)\n", diff2);
  diff3 = boost_date_minus_date (2002, 1, 1, 2002, 1, 1);
  printf ("    2002-01-01 - 2002-01-01 = %d days (expected 0)\n", diff3);
  printf ("\n");

  %% ===== 29. boost_date_to_tm =====
  printf ("29. boost_date_to_tm:\n");
  tm = boost_date_to_tm (2005, 1, 1);
  printf ("    2005-01-01 -> tm_year=%d, tm_mon=%d, tm_mday=%d, tm_wday=%d, tm_yday=%d, tm_hour=%d, tm_min=%d, tm_sec=%d, tm_isdst=%d\n",
          tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_wday, tm.tm_yday,
          tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_isdst);
  printf ("    (expected: tm_year=105, tm_mon=0, tm_mday=1, tm_wday=6, tm_yday=0, tm_hour=0, tm_min=0, tm_sec=0, tm_isdst=-1)\n");
  printf ("\n");

  %% ===== 30. boost_date_from_tm =====
  printf ("30. boost_date_from_tm:\n");
  tm2.tm_year = 105;
  tm2.tm_mon = 0;
  tm2.tm_mday = 1;
  d_from_tm = boost_date_from_tm (tm2);
  printf ("    tm(105,0,1) -> date = %d-%02d-%02d (expected 2005-01-01)\n",
          d_from_tm.year, d_from_tm.month, d_from_tm.day);
  printf ("\n");

  %% ===== 31. boost_date_period_shift =====
  printf ("31. boost_date_period_shift:\n");
  dp = boost_date_period_shift (2005, 1, 1, 3, 3);
  printf ("    shift(2005-Jan-01, 3 days) by 3 days: begin=%d-%02d-%02d, last=%d-%02d-%02d, length=%d\n",
          dp.begin.year, dp.begin.month, dp.begin.day,
          dp.last.year, dp.last.month, dp.last.day,
          dp.length);
  printf ("    (expected: Jan-04 to Jan-07, length=3)\n");
  printf ("\n");

  %% ===== 32. boost_date_period_expand =====
  printf ("32. boost_date_period_expand:\n");
  dp2 = boost_date_period_expand (2005, 1, 2, 2, 1);
  printf ("    expand(2005-Jan-02, 2 days) by 1 day: begin=%d-%02d-%02d, last=%d-%02d-%02d, length=%d\n",
          dp2.begin.year, dp2.begin.month, dp2.begin.day,
          dp2.last.year, dp2.last.month, dp2.last.day,
          dp2.length);
  printf ("    (expected: Jan-01 to Jan-04, length=3)\n");
  printf ("\n");

  %% ===== 33. boost_date_period_begin =====
  printf ("33. boost_date_period_begin:\n");
  d31 = boost_date_period_begin (2002, 1, 1, 2002, 1, 10);
  printf ("    begin(2002-01-01 to 2002-01-10): year=%d, month=%d, day=%d (expected 2002, 1, 1)\n",
          d31.year, d31.month, d31.day);
  printf ("\n");

  %% ===== 34. boost_date_period_last =====
  printf ("34. boost_date_period_last:\n");
  d32 = boost_date_period_last (2002, 1, 1, 2002, 1, 10);
  printf ("    last(2002-01-01 to 2002-01-10): year=%d, month=%d, day=%d (expected 2002, 1, 9)\n",
          d32.year, d32.month, d32.day);
  printf ("\n");

  %% ===== 35. boost_date_period_end =====
  printf ("35. boost_date_period_end:\n");
  d33 = boost_date_period_end (2002, 1, 1, 2002, 1, 10);
  printf ("    end(2002-01-01 to 2002-01-10): year=%d, month=%d, day=%d (expected 2002, 1, 10)\n",
          d33.year, d33.month, d33.day);
  printf ("\n");

  %% ===== 36. boost_date_period_length =====
  printf ("36. boost_date_period_length:\n");
  len1 = boost_date_period_length (2002, 1, 1, 2);
  printf ("    length(2002-01-01, 2 days) = %d (expected 2)\n", len1);
  printf ("\n");

  %% ===== 37. boost_date_period_is_null =====
  printf ("37. boost_date_period_is_null:\n");
  null1 = boost_date_period_is_null (2002, 1, 10, 2002, 1, 1);
  printf ("    is_null(2002-01-10 to 2002-01-01) = %d (expected 1, true)\n", null1);
  null2 = boost_date_period_is_null (2002, 1, 1, 2002, 1, 10);
  printf ("    is_null(2002-01-01 to 2002-01-10) = %d (expected 0, false)\n", null2);
  printf ("\n");

  %% ===== 38. boost_date_period_contains_date =====
  printf ("38. boost_date_period_contains_date:\n");
  c1 = boost_date_period_contains_date (2002, 1, 1, 2002, 1, 10, 2002, 1, 2);
  printf ("    [2002-01-01, 2002-01-10) contains 2002-01-02 = %d (expected 1, true)\n", c1);
  c2 = boost_date_period_contains_date (2002, 1, 1, 2002, 1, 10, 2002, 1, 10);
  printf ("    [2002-01-01, 2002-01-10) contains 2002-01-10 = %d (expected 0, false)\n", c2);
  % Zero-length period test
  c3 = boost_date_period_contains_date (2002, 1, 1, 2002, 1, 1, 2002, 1, 1);
  printf ("    [2002-01-01, 2002-01-01) contains 2002-01-01 = %d (expected 0, false)\n", c3);
  printf ("\n");

  %% ===== 39. boost_date_period_contains_date_period =====
  printf ("39. boost_date_period_contains_date_period:\n");
  i1 = boost_date_period_contains_date_period (2002, 1, 1, 2002, 1, 10, 2002, 1, 2, 2002, 1, 3);
  printf ("    [2002-01-01, 2002-01-10) intersects [2002-01-02, 2002-01-03) = %d (expected 1, true)\n", i1);
  i2 = boost_date_period_contains_date_period (2002, 1, 1, 2002, 1, 2, 2002, 1, 10, 2002, 1, 11);
  printf ("    [2002-01-01, 2002-01-02) intersects [2002-01-10, 2002-01-11) = %d (expected 0, false)\n", i2);
  printf ("\n");

  %% ===== 40. boost_date_period_intersection =====
  printf ("40. boost_date_period_intersection:\n");
  dp3 = boost_date_period_intersection (2002, 1, 1, 2002, 1, 10, 2002, 1, 2, 2002, 1, 3);
  printf ("    intersection: begin=%d-%02d-%02d, last=%d-%02d-%02d, length=%d (expected 2002-01-02 to 2002-01-03, length=1)\n",
          dp3.begin.year, dp3.begin.month, dp3.begin.day,
          dp3.last.year, dp3.last.month, dp3.last.day,
          dp3.length);
  % No intersection
  dp4 = boost_date_period_intersection (2002, 1, 1, 2002, 1, 2, 2002, 1, 10, 2002, 1, 11);
  printf ("    no intersection: length=%d (expected 0)\n", dp4.length);
  printf ("\n");

  %% ===== 41. boost_date_period_is_adjacent =====
  printf ("41. boost_date_period_is_adjacent:\n");
  adj1 = boost_date_period_is_adjacent (2002, 1, 1, 2002, 1, 3, 2002, 1, 3, 2002, 1, 10);
  printf ("    [2002-01-01, 2002-01-03) adjacent [2002-01-03, 2002-01-10) = %d (expected 1, true)\n", adj1);
  adj2 = boost_date_period_is_adjacent (2002, 1, 1, 2002, 1, 5, 2002, 1, 3, 2002, 1, 10);
  printf ("    [2002-01-01, 2002-01-05) adjacent [2002-01-03, 2002-01-10) = %d (expected 0, false)\n", adj2);
  printf ("\n");

  %% ===== 42. boost_date_period_is_after =====
  printf ("42. boost_date_period_is_after:\n");
  after1 = boost_date_period_is_after (2002, 1, 10, 2002, 1, 30, 2002, 1, 3);
  printf ("    [2002-01-10, 2002-01-30) after 2002-01-03 = %d (expected 1, true)\n", after1);
  after2 = boost_date_period_is_after (2002, 1, 1, 2002, 1, 10, 2002, 1, 15);
  printf ("    [2002-01-01, 2002-01-10) after 2002-01-15 = %d (expected 0, false)\n", after2);
  printf ("\n");

  %% ===== 43. boost_date_period_is_before =====
  printf ("43. boost_date_period_is_before:\n");
  before1 = boost_date_period_is_before (2002, 1, 1, 2002, 1, 3, 2002, 1, 10);
  printf ("    [2002-01-01, 2002-01-03) before 2002-01-10 = %d (expected 1, true)\n", before1);
  before2 = boost_date_period_is_before (2002, 1, 10, 2002, 1, 30, 2002, 1, 5);
  printf ("    [2002-01-10, 2002-01-30) before 2002-01-05 = %d (expected 0, false)\n", before2);
  printf ("\n");

  %% ===== 44. boost_date_period_merge =====
  printf ("44. boost_date_period_merge:\n");
  dp5 = boost_date_period_merge (2002, 1, 1, 2002, 1, 10, 2002, 1, 9, 2002, 1, 31);
  printf ("    merge: begin=%d-%02d-%02d, last=%d-%02d-%02d, length=%d (expected 2002-01-01 to 2002-01-31, length=30)\n",
          dp5.begin.year, dp5.begin.month, dp5.begin.day,
          dp5.last.year, dp5.last.month, dp5.last.day,
          dp5.length);
  % No intersection -> null period
  dp6 = boost_date_period_merge (2002, 1, 1, 2002, 1, 5, 2002, 1, 10, 2002, 1, 31);
  printf ("    no intersection merge: length=%d (expected 0)\n", dp6.length);
  printf ("\n");

  %% ===== 45. boost_date_period_span =====
  printf ("45. boost_date_period_span:\n");
  dp7 = boost_date_period_span (2002, 1, 1, 2002, 1, 5, 2002, 1, 9, 2002, 1, 31);
  printf ("    span: begin=%d-%02d-%02d, last=%d-%02d-%02d, length=%d (expected 2002-01-01 to 2002-01-31, length=30)\n",
          dp7.begin.year, dp7.begin.month, dp7.begin.day,
          dp7.last.year, dp7.last.month, dp7.last.day,
          dp7.length);
  printf ("\n");

  %% ===== 46. boost_date_span_to_simple_string =====
  printf ("46. boost_date_span_to_simple_string:\n");
  str1 = boost_date_span_to_simple_string (2002, 1, 1, 2002, 1, 31);
  printf ("    span string = %s (expected [2002-Jan-01/2002-Jan-31])\n", str1);
  printf ("\n");

  %% ===== 47. boost_date_period_eq =====
  printf ("47. boost_date_period_eq:\n");
  eq1 = boost_date_period_eq (2002, 1, 1, 2002, 1, 10, 2002, 1, 1, 2002, 1, 10);
  printf ("    [2002-01-01, 2002-01-10) == [2002-01-01, 2002-01-10) = %d (expected 1, true)\n", eq1);
  eq2 = boost_date_period_eq (2002, 1, 1, 2002, 1, 10, 2002, 1, 1, 2002, 1, 11);
  printf ("    [2002-01-01, 2002-01-10) == [2002-01-01, 2002-01-11) = %d (expected 0, false)\n", eq2);
  printf ("\n");

  %% ===== 48. boost_date_period_ne =====
  printf ("48. boost_date_period_ne:\n");
  ne1 = boost_date_period_ne (2002, 1, 1, 2002, 1, 10, 2002, 1, 1, 2002, 1, 11);
  printf ("    [2002-01-01, 2002-01-10) != [2002-01-01, 2002-01-11) = %d (expected 1, true)\n", ne1);
  ne2 = boost_date_period_ne (2002, 1, 1, 2002, 1, 10, 2002, 1, 1, 2002, 1, 10);
  printf ("    [2002-01-01, 2002-01-10) != [2002-01-01, 2002-01-10) = %d (expected 0, false)\n", ne2);
  printf ("\n");

  %% ===== 49. boost_date_period_gt =====
  printf ("49. boost_date_period_gt:\n");
  gt1 = boost_date_period_gt (2002, 1, 10, 2002, 1, 30, 2002, 1, 1, 2002, 1, 10);
  printf ("    [2002-01-10, 2002-01-30) > [2002-01-01, 2002-01-10) = %d (expected 1, true)\n", gt1);
  gt2 = boost_date_period_gt (2002, 1, 1, 2002, 1, 10, 2002, 1, 10, 2002, 1, 30);
  printf ("    [2002-01-01, 2002-01-10) > [2002-01-10, 2002-01-30) = %d (expected 0, false)\n", gt2);
  printf ("\n");

  %% ===== 50. boost_date_period_lt =====
  printf ("50. boost_date_period_lt:\n");
  lt1 = boost_date_period_lt (2002, 1, 1, 2002, 1, 10, 2002, 1, 10, 2002, 1, 30);
  printf ("    [2002-01-01, 2002-01-10) < [2002-01-10, 2002-01-30) = %d (expected 1, true)\n", lt1);
  lt2 = boost_date_period_lt (2002, 1, 10, 2002, 1, 30, 2002, 1, 1, 2002, 1, 10);
  printf ("    [2002-01-10, 2002-01-30) < [2002-01-01, 2002-01-10) = %d (expected 0, false)\n", lt2);
  printf ("\n");

  %% ===== 51. boost_date_gregorian_day_number =====
  printf ("51. boost_date_gregorian_day_number:\n");
  dn = boost_date_gregorian_day_number (2002, 1, 10);
  printf ("    boost_date_gregorian_day_number (2002, 1, 10) = %d (expected 731228)\n", dn);
  % Also test epoch: 1400-01-01 should give 2232400
  dn_epoch = boost_date_gregorian_day_number (1400, 1, 1);
  printf ("    boost_date_gregorian_day_number (1400, 1, 1) = %d (expected 2232400)\n", dn_epoch);
  printf ("\n");

  %% ===== 52. boost_date_gregorian_end_of_month_day =====
  printf ("52. boost_date_gregorian_end_of_month_day:\n");
  eom1 = boost_date_gregorian_end_of_month_day (2000, 2);
  printf ("    boost_date_gregorian_end_of_month_day (2000, 2) = %d (expected 29)\n", eom1);
  eom2 = boost_date_gregorian_end_of_month_day (2001, 2);
  printf ("    boost_date_gregorian_end_of_month_day (2001, 2) = %d (expected 28)\n", eom2);
  eom3 = boost_date_gregorian_end_of_month_day (2002, 1);
  printf ("    boost_date_gregorian_end_of_month_day (2002, 1) = %d (expected 31)\n", eom3);
  printf ("\n");

  %% ===== 53. boost_date_gregorian_from_day_number =====
  printf ("53. boost_date_gregorian_from_day_number:\n");
  d_from_dn = boost_date_gregorian_from_day_number (731228);
  printf ("    boost_date_gregorian_from_day_number (731228) = %d-%02d-%02d (expected 2002-01-10)\n",
          d_from_dn.year, d_from_dn.month, d_from_dn.day);
  printf ("\n");

  %% ===== 54. boost_date_gregorian_is_leap_year =====
  printf ("54. boost_date_gregorian_is_leap_year:\n");
  leap1 = boost_date_gregorian_is_leap_year (2000);
  printf ("    boost_date_gregorian_is_leap_year (2000) = %d (expected 1)\n", leap1);
  leap2 = boost_date_gregorian_is_leap_year (2001);
  printf ("    boost_date_gregorian_is_leap_year (2001) = %d (expected 0)\n", leap2);
  leap3 = boost_date_gregorian_is_leap_year (2024);
  printf ("    boost_date_gregorian_is_leap_year (2024) = %d (expected 1)\n", leap3);
  printf ("\n");

  %% ===== 55. boost_ptime_date =====
  printf ("55. boost_ptime_date:\n");
  pt_d = boost_ptime_date ("2002-01-10 01:00:00");
  printf ("    boost_ptime_date ('2002-01-10 01:00:00') = %d-%02d-%02d (expected 2002-01-10)\n",
          pt_d.year, pt_d.month, pt_d.day);
  % Using struct input
  ptime_struct.date.year = 2002;
  ptime_struct.date.month = 1;
  ptime_struct.date.day = 10;
  ptime_struct.time_of_day.hours = 1;
  ptime_struct.time_of_day.minutes = 0;
  ptime_struct.time_of_day.seconds = 0;
  pt_d2 = boost_ptime_date (ptime_struct);
  printf ("    Using struct: date = %d-%02d-%02d (expected 2002-01-10)\n",
          pt_d2.year, pt_d2.month, pt_d2.day);
  printf ("\n");

  %% ===== 56. boost_ptime_time_of_day =====
  printf ("56. boost_ptime_time_of_day:\n");
  tod = boost_ptime_time_of_day ("2002-01-10 01:02:03");
  printf ("    boost_ptime_time_of_day ('2002-01-10 01:02:03') = %d:%02d:%02d (expected 1:02:03)\n",
          tod.hours, tod.minutes, tod.seconds);
  printf ("    total_seconds = %d (expected 3723)\n", tod.total_seconds);
  printf ("\n");

  %% ===== 57. boost_ptime_is_infinity =====
  printf ("57. boost_ptime_is_infinity:\n");
  inf1 = boost_ptime_is_infinity ("pos_infin");
  printf ("    boost_ptime_is_infinity ('pos_infin') = %d (expected 1)\n", inf1);
  inf2 = boost_ptime_is_infinity ("2002-01-10 01:00:00");
  printf ("    boost_ptime_is_infinity ('2002-01-10 01:00:00') = %d (expected 0)\n", inf2);
  printf ("\n");

  %% ===== 58. boost_ptime_is_neg_infinity =====
  printf ("58. boost_ptime_is_neg_infinity:\n");
  neg_inf1 = boost_ptime_is_neg_infinity ("neg_infin");
  printf ("    boost_ptime_is_neg_infinity ('neg_infin') = %d (expected 1)\n", neg_inf1);
  neg_inf2 = boost_ptime_is_neg_infinity ("pos_infin");
  printf ("    boost_ptime_is_neg_infinity ('pos_infin') = %d (expected 0)\n", neg_inf2);
  printf ("\n");

  %% ===== 59. boost_ptime_is_pos_infinity =====
  printf ("59. boost_ptime_is_pos_infinity:\n");
  pos_inf1 = boost_ptime_is_pos_infinity ("pos_infin");
  printf ("    boost_ptime_is_pos_infinity ('pos_infin') = %d (expected 1)\n", pos_inf1);
  pos_inf2 = boost_ptime_is_pos_infinity ("neg_infin");
  printf ("    boost_ptime_is_pos_infinity ('neg_infin') = %d (expected 0)\n", pos_inf2);
  printf ("\n");

  %% ===== 60. boost_ptime_is_not_a_date_time =====
  printf ("60. boost_ptime_is_not_a_date_time:\n");
  nad1 = boost_ptime_is_not_a_date_time ("not_a_date_time");
  printf ("    boost_ptime_is_not_a_date_time ('not_a_date_time') = %d (expected 1)\n", nad1);
  nad2 = boost_ptime_is_not_a_date_time ("2002-01-10 01:00:00");
  printf ("    boost_ptime_is_not_a_date_time ('2002-01-10 01:00:00') = %d (expected 0)\n", nad2);
  printf ("\n");

  %% ===== 71. boost_time_duration_hours =====
  printf ("71. boost_time_duration_hours:\n");
  td_hours = boost_time_duration_hours (1, 2, 3);
  printf ("    boost_time_duration_hours (1, 2, 3) → %d (expected 1)\n", td_hours);
  printf ("\n");

  %% ===== 72. boost_time_duration_minutes =====
  printf ("72. boost_time_duration_minutes:\n");
  td_minutes = boost_time_duration_minutes (1, 2, 3);
  printf ("    boost_time_duration_minutes (1, 2, 3) → %d (expected 2)\n", td_minutes);
  printf ("\n");

  %% ===== 73. boost_time_duration_seconds =====
  printf ("73. boost_time_duration_seconds:\n");
  td_seconds = boost_time_duration_seconds (1, 2, 3);
  printf ("    boost_time_duration_seconds (1, 2, 3) → %d (expected 3)\n", td_seconds);
  printf ("\n");

  %% ===== 74. boost_time_duration_total_seconds =====
  printf ("74. boost_time_duration_total_seconds:\n");
  td_total_sec = boost_time_duration_total_seconds (1, 2, 3, 10);
  printf ("    boost_time_duration_total_seconds (1, 2, 3, 10) → %d (expected 3723)\n", td_total_sec);
  printf ("\n");

  %% ===== 75. boost_time_duration_total_milliseconds =====
  printf ("75. boost_time_duration_total_milliseconds:\n");
  td_total_ms = boost_time_duration_total_milliseconds (1, 2, 3, 123456789);
  printf ("    boost_time_duration_total_milliseconds (1, 2, 3, 123456789) → %d (expected 3723123)\n", td_total_ms);
  printf ("\n");

  %% ===== 76. boost_time_duration_total_microseconds =====
  printf ("76. boost_time_duration_total_microseconds:\n");
  td_total_us = boost_time_duration_total_microseconds (1, 2, 3, 123456789);
  printf ("    boost_time_duration_total_microseconds (1, 2, 3, 123456789) → %d (expected 3723123456)\n", td_total_us);
  printf ("\n");

  %% ===== 77. boost_time_duration_total_nanoseconds =====
  printf ("77. boost_time_duration_total_nanoseconds:\n");
  td_total_ns = boost_time_duration_total_nanoseconds (1, 2, 3, 123456789);
  printf ("    boost_time_duration_total_nanoseconds (1, 2, 3, 123456789) → %d (expected 3723123456789)\n", td_total_ns);
  printf ("\n");

  %% ===== 78. boost_time_duration_fractional_seconds =====
  printf ("78. boost_time_duration_fractional_seconds:\n");
  td_fs = boost_time_duration_fractional_seconds (1, 2, 3, 1000);
  printf ("    boost_time_duration_fractional_seconds (1, 2, 3, 1000) → %d (expected 1000)\n", td_fs);
  printf ("\n");

  %% ===== 79. boost_time_duration_is_negative =====
  printf ("79. boost_time_duration_is_negative:\n");
  td_neg1 = boost_time_duration_is_negative (-1, 0, 0);
  td_neg2 = boost_time_duration_is_negative (1, 0, 0);
  printf ("    boost_time_duration_is_negative (-1, 0, 0) → %d (expected 1)\n", td_neg1);
  printf ("    boost_time_duration_is_negative (1, 0, 0) → %d (expected 0)\n", td_neg2);
  printf ("\n");

  %% ===== 80. boost_time_duration_is_zero =====
  printf ("80. boost_time_duration_is_zero:\n");
  td_zero1 = boost_time_duration_is_zero (0, 0, 0);
  td_zero2 = boost_time_duration_is_zero (1, 0, 0);
  printf ("    boost_time_duration_is_zero (0, 0, 0) → %d (expected 1)\n", td_zero1);
  printf ("    boost_time_duration_is_zero (1, 0, 0) → %d (expected 0)\n", td_zero2);
  printf ("\n");

  printf ("\n=== Demo Complete! ===\n");

  %% ===== 61. boost_ptime_add_days =====
  printf ("61. boost_ptime_add_days:\n");
  pt1 = boost_ptime_add_days ("2002-01-01 00:05:00", 1);
  printf ("    boost_ptime_add_days ('2002-01-01 00:05:00', 1): date=%d-%02d-%02d, time=%02d:%02d\n",
          pt1.date.year, pt1.date.month, pt1.date.day,
          pt1.time_of_day.hours, pt1.time_of_day.minutes);
  printf ("\n");

  %% ===== 62. boost_ptime_minus_days =====
  printf ("62. boost_ptime_minus_days:\n");
  pt2 = boost_ptime_minus_days ("2002-01-10 00:05:00", 1);
  printf ("    boost_ptime_minus_days ('2002-01-10 00:05:00', 1): date=%d-%02d-%02d, time=%02d:%02d\n",
          pt2.date.year, pt2.date.month, pt2.date.day,
          pt2.time_of_day.hours, pt2.time_of_day.minutes);
  printf ("\n");

  %% ===== 63. boost_ptime_add_time_duration =====
  printf ("63. boost_ptime_add_time_duration:\n");
  pt3 = boost_ptime_add_time_duration ("2002-01-01 00:05:00", 1, 2, 0);
  printf ("    boost_ptime_add_time_duration ('2002-01-01 00:05:00', 1, 2, 0): date=%d-%02d-%02d, time=%02d:%02d:%02d\n",
          pt3.date.year, pt3.date.month, pt3.date.day,
          pt3.time_of_day.hours, pt3.time_of_day.minutes, pt3.time_of_day.seconds);
  printf ("\n");

  %% ===== 64. boost_ptime_minus_time_duration =====
  printf ("64. boost_ptime_minus_time_duration:\n");
  pt4 = boost_ptime_minus_time_duration ("2002-01-01 00:05:00", 0, 2, 0);
  printf ("    boost_ptime_minus_time_duration ('2002-01-01 00:05:00', 0, 2, 0): date=%d-%02d-%02d, time=%02d:%02d:%02d\n",
          pt4.date.year, pt4.date.month, pt4.date.day,
          pt4.time_of_day.hours, pt4.time_of_day.minutes, pt4.time_of_day.seconds);
  printf ("\n");

  %% ===== 65. boost_ptime_minus_ptime =====
  printf ("65. boost_ptime_minus_ptime:\n");
  td5 = boost_ptime_minus_ptime ("2002-01-01 00:00:05", "2002-01-01 00:05:00");
  printf ("    boost_ptime_minus_ptime (05s, 5min): hours=%d, minutes=%d, seconds=%d (expected -1, -4, -55)\n",
          td5.hours, td5.minutes, td5.seconds);
  printf ("\n");

  %% ===== 66. boost_ptime_to_tm =====
  printf ("66. boost_ptime_to_tm:\n");
  tm6 = boost_ptime_to_tm ("2005-01-01 01:02:03");
  printf ("    boost_ptime_to_tm ('2005-01-01 01:02:03'):\n");
  printf ("      tm_year=%d (expected 105), tm_mon=%d (expected 0), tm_mday=%d (expected 1)\n",
          tm6.tm_year, tm6.tm_mon, tm6.tm_mday);
  printf ("      tm_wday=%d (expected 6), tm_yday=%d (expected 0)\n",
          tm6.tm_wday, tm6.tm_yday);
  printf ("      tm_hour=%d (expected 1), tm_min=%d (expected 2), tm_sec=%d (expected 3)\n",
          tm6.tm_hour, tm6.tm_min, tm6.tm_sec);
  printf ("\n");

  %% ===== 67. boost_ptime_from_tm =====
  printf ("67. boost_ptime_from_tm:\n");
  tm7.tm_year = 105;
  tm7.tm_mon = 0;
  tm7.tm_mday = 1;
  tm7.tm_hour = 1;
  tm7.tm_min = 2;
  tm7.tm_sec = 3;
  pt7 = boost_ptime_from_tm (tm7);
  printf ("    boost_ptime_from_tm (tm struct): date=%d-%02d-%02d, time=%02d:%02d:%02d (expected 2005-01-01 01:02:03)\n",
          pt7.date.year, pt7.date.month, pt7.date.day,
          pt7.time_of_day.hours, pt7.time_of_day.minutes, pt7.time_of_day.seconds);
  printf ("\n");

  %% ===== 68. boost_time_duration_to_tm =====
  printf ("68. boost_time_duration_to_tm:\n");
  tm8 = boost_time_duration_to_tm (1, 2, 3);
  printf ("    boost_time_duration_to_tm (1, 2, 3):\n");
  printf ("      tm_hour=%d (expected 1), tm_min=%d (expected 2), tm_sec=%d (expected 3)\n",
          tm8.tm_hour, tm8.tm_min, tm8.tm_sec);
  printf ("      tm_year=%d (expected 0), tm_mon=%d (expected 0), tm_mday=%d (expected 0)\n",
          tm8.tm_year, tm8.tm_mon, tm8.tm_mday);
  printf ("\n");

  %% ===== 69. boost_ptime_from_time_t =====
  printf ("69. boost_ptime_from_time_t:\n");
  pt9 = boost_ptime_from_time_t (1118158776);
  printf ("    boost_ptime_from_time_t (1118158776): date=%d-%02d-%02d, time=%02d:%02d:%02d (expected 2005-06-07 15:39:36)\n",
          pt9.date.year, pt9.date.month, pt9.date.day,
          pt9.time_of_day.hours, pt9.time_of_day.minutes, pt9.time_of_day.seconds);
  printf ("\n");

  %% ===== 70. boost_ptime_from_ftime =====
  printf ("70. boost_ptime_from_ftime:\n");
  try
    pt10 = boost_ptime_from_ftime (29715317, 3865122988);
    printf ("    boost_ptime_from_ftime (29715317, 3865122988): date=%d-%02d-%02d, time=%02d:%02d:%02d (expected 2005-06-07 15:30:57)\n",
            pt10.date.year, pt10.date.month, pt10.date.day,
            pt10.time_of_day.hours, pt10.time_of_day.minutes, pt10.time_of_day.seconds);
  catch
    printf ("    boost_ptime_from_ftime not available on this platform\n");
  end_try_catch
  printf ("\n");

  %% ===== 71. boost_time_duration_is_positive =====
  printf ("71. boost_time_duration_is_positive:\n");
  pos1 = boost_time_duration_is_positive (1, 0, 0);
  pos2 = boost_time_duration_is_positive (-1, 0, 0);
  printf ("    boost_time_duration_is_positive (1, 0, 0) = %d (expected 1)\n", pos1);
  printf ("    boost_time_duration_is_positive (-1, 0, 0) = %d (expected 0)\n", pos2);
  printf ("\n");

  %% ===== 72. boost_time_duration_invert_sign =====
  printf ("72. boost_time_duration_invert_sign:\n");
  inv1 = boost_time_duration_invert_sign (-1, 0, 0);
  printf ("    boost_time_duration_invert_sign (-1, 0, 0): hours=%d, minutes=%d, seconds=%d (expected 1, 0, 0)\n",
          inv1.hours, inv1.minutes, inv1.seconds);
  printf ("\n");

  %% ===== 73. boost_time_duration_abs =====
  printf ("73. boost_time_duration_abs:\n");
  abs1 = boost_time_duration_abs (-1, 30, 45);
  printf ("    boost_time_duration_abs (-1, 30, 45): hours=%d, minutes=%d, seconds=%d (expected 1, 30, 45)\n",
          abs1.hours, abs1.minutes, abs1.seconds);
  printf ("\n");

  %% ===== 74. boost_time_duration_resolution =====
  printf ("74. boost_time_duration_resolution:\n");
  res1 = boost_time_duration_resolution ();
  printf ("    boost_time_duration_resolution () = '%s' (expected 'nano')\n", res1);
  printf ("\n");

  %% ===== 75. boost_time_duration_num_fractional_digits =====
  printf ("75. boost_time_duration_num_fractional_digits:\n");
  nfd1 = boost_time_duration_num_fractional_digits ();
  printf ("    boost_time_duration_num_fractional_digits () = %d (expected 9 for nano)\n", nfd1);
  printf ("\n");

  %% ===== 76. boost_time_duration_ticks_per_second =====
  printf ("76. boost_time_duration_ticks_per_second:\n");
  tps1 = boost_time_duration_ticks_per_second ();
  printf ("    boost_time_duration_ticks_per_second () = %d (expected 1000000000 for nano)\n", tps1);
  printf ("\n");

  %% ===== 77. boost_time_duration_ticks =====
  printf ("77. boost_time_duration_ticks:\n");
  t1 = boost_time_duration_ticks (0, 0, 0, 1000);
  printf ("    boost_time_duration_ticks (0, 0, 0, 1000) = %d (expected 1000)\n", t1);
  printf ("\n");

  %% ===== 78. boost_time_duration_unit =====
  printf ("78. boost_time_duration_unit:\n");
  u1 = boost_time_duration_unit ();
  printf ("    boost_time_duration_unit (): hours=%d, minutes=%d, seconds=%d, fractional_seconds=%d (expected 0, 0, 0, 1)\n",
          u1.hours, u1.minutes, u1.seconds, u1.fractional_seconds);
  printf ("\n");

  %% ===== 79. boost_time_duration_is_neg_infinity =====
  printf ("79. boost_time_duration_is_neg_infinity:\n");
  ni1 = boost_time_duration_is_neg_infinity ("neg_infin");
  ni2 = boost_time_duration_is_neg_infinity (1, 0, 0);
  printf ("    boost_time_duration_is_neg_infinity ('neg_infin') = %d (expected 1)\n", ni1);
  printf ("    boost_time_duration_is_neg_infinity (1, 0, 0) = %d (expected 0)\n", ni2);
  printf ("\n");

  %% ===== 80. boost_time_duration_is_pos_infinity =====
  printf ("80. boost_time_duration_is_pos_infinity:\n");
  pi1 = boost_time_duration_is_pos_infinity ("pos_infin");
  pi2 = boost_time_duration_is_pos_infinity (1, 0, 0);
  printf ("    boost_time_duration_is_pos_infinity ('pos_infin') = %d (expected 1)\n", pi1);
  printf ("    boost_time_duration_is_pos_infinity (1, 0, 0) = %d (expected 0)\n", pi2);
  printf ("\n");

  %% ===== 81. boost_time_duration_plus_time_duration =====
  printf ("81. boost_time_duration_plus_time_duration:\n");
  td_plus = boost_time_duration_plus_time_duration (1, 2, 30, 0, 10, 30);
  printf ("    boost_time_duration_plus_time_duration (1, 2, 30, 0, 10, 30): hours=%d, minutes=%d, seconds=%d (expected 1, 13, 0)\n",
          td_plus.hours, td_plus.minutes, td_plus.seconds);
  printf ("\n");

  %% ===== 82. boost_time_duration_minus_time_duration =====
  printf ("82. boost_time_duration_minus_time_duration:\n");
  td_minus = boost_time_duration_minus_time_duration (2, 30, 0, 1, 45, 0);
  printf ("    boost_time_duration_minus_time_duration (2, 30, 0, 1, 45, 0): hours=%d, minutes=%d, seconds=%d (expected 0, 45, 0)\n",
          td_minus.hours, td_minus.minutes, td_minus.seconds);
  printf ("\n");

  %% ===== 83. boost_time_duration_divide_by_days =====
  printf ("83. boost_time_duration_divide_by_days:\n");
  td_div = boost_time_duration_divide_by_days (3, 0, 0, 2);
  printf ("    boost_time_duration_divide_by_days (3, 0, 0, 2): hours=%d, minutes=%d, seconds=%d (expected 1, 30, 0)\n",
          td_div.hours, td_div.minutes, td_div.seconds);
  printf ("\n");

  %% ===== 84. boost_time_duration_multiply_by_days =====
  printf ("84. boost_time_duration_multiply_by_days:\n");
  td_mul = boost_time_duration_multiply_by_days (3, 0, 0, 2);
  printf ("    boost_time_duration_multiply_by_days (3, 0, 0, 2): hours=%d, minutes=%d, seconds=%d (expected 6, 0, 0)\n",
          td_mul.hours, td_mul.minutes, td_mul.seconds);
  printf ("\n");

  %% ===== 85. boost_time_period_begin =====
  printf ("85. boost_time_period_begin:\n");
  tp_begin = boost_time_period_begin (struct ('begin', '2002-01-10 00:00:10', 'end', '2002-01-10 10:00:00'));
  printf ("    boost_time_period_begin: date=%d-%02d-%02d, hours=%d (expected 2002-01-10, 0)\n",
          tp_begin.date.year, tp_begin.date.month, tp_begin.date.day,
          tp_begin.time_of_day.hours);
  printf ("\n");

  %% ===== 86. boost_time_period_last =====
  printf ("86. boost_time_period_last:\n");
  tp_last = boost_time_period_last (struct ('begin', '2002-01-10 00:00:10', 'end', '2002-01-10 10:00:00'));
  printf ("    boost_time_period_last: hours=%d (expected 9)\n",
          tp_last.time_of_day.hours);
  printf ("\n");

  %% ===== 87. boost_time_period_end =====
  printf ("87. boost_time_period_end:\n");
  tp_end = boost_time_period_end (struct ('begin', '2002-01-10 00:00:10', 'end', '2002-01-10 10:00:00'));
  printf ("    boost_time_period_end: hours=%d (expected 10)\n",
          tp_end.time_of_day.hours);
  printf ("\n");

  %% ===== 88. boost_time_period_length =====
  printf ("88. boost_time_period_length:\n");
  tp_len = boost_time_period_length (struct ('begin', '2002-01-10 00:00:00', 'length', struct ('hours', 1, 'minutes', 30, 'seconds', 0)));
  printf ("    boost_time_period_length: hours=%d, minutes=%d, seconds=%d (expected 1, 30, 0)\n",
          tp_len.hours, tp_len.minutes, tp_len.seconds);
  printf ("\n");

  %% ===== 89. boost_time_period_is_null =====
  printf ("89. boost_time_period_is_null:\n");
  tp_null = boost_time_period_is_null ('2002-01-10 12:00:00', '2002-01-10 09:00:00');
  printf ("    boost_time_period_is_null (end before begin) = %d (expected 1, true)\n", tp_null);
  tp_not_null = boost_time_period_is_null ('2002-01-10 09:00:00', '2002-01-10 12:00:00');
  printf ("    boost_time_period_is_null (end after begin) = %d (expected 0, false)\n", tp_not_null);
  printf ("\n");

  %% ===== 90. boost_time_period_contains_ptime =====
  printf ("90. boost_time_period_contains_ptime:\n");
  tp_contains = boost_time_period_contains_ptime ('2002-01-10 00:00:10', '2002-01-10 10:00:00', '2002-01-10 02:00:00');
  printf ("    boost_time_period_contains_ptime (02:00 inside [00:00:10, 10:00:00)) = %d (expected 1, true)\n", tp_contains);
  tp_not_contains = boost_time_period_contains_ptime ('2002-01-10 00:00:10', '2002-01-10 10:00:00', '2002-01-10 10:00:00');
  printf ("    boost_time_period_contains_ptime (10:00 at boundary) = %d (expected 0, false)\n", tp_not_contains);
  printf ("\n");

  printf ("\n=== Demo Complete! ===\n");
endfunction
