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

#include "boost_date_common.h"

DEFUN_DLD (boost_date_gregorian_from_day_number, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{date} =} boost_date_gregorian_from_day_number (@var{day_number})\n\
\n\
Convert a serial day number back into a date struct.\n\
Returns a struct with fields @qcode{\"year\"}, @qcode{\"month\"}, @qcode{\"day\"}.\n\
\n\
Based on the @code{boost::gregorian::date} constructor from a day number.\n\
\n\
If @var{day_number} is outside the representable date range (years 1400-9999),\n\
an error is thrown instead of a segfault or abort.\n\
\n\
@example\n\
@group\n\
d = boost_date_gregorian_from_day_number (731228);\n\
@result{} d.year = 2002, d.month = 1, d.day = 10\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 1)
    print_usage ();

  double day_num_val = args (0).double_value ();

  // Validate input: reject NaN, Inf, or non-finite values
  if (! std::isfinite (day_num_val))
    error ("boost_date_gregorian_from_day_number: day number must be a finite value, got %g",
           day_num_val);

  bg::date::date_int_type day_num
    = static_cast<bg::gregorian_calendar::date_int_type> (day_num_val);

  bg::date d;

  // The bg::date constructor from a day number may throw
  // boost::gregorian::bad_year (or other std::exception-derived exceptions)
  // when the day number maps to a year outside the valid range 1400-9999.
  // We catch it here and convert to a clean Octave error instead of aborting.
  try
    {
      d = bg::date (day_num);
    }
  catch (const std::exception& e)
    {
      error ("boost_date_gregorian_from_day_number: day number %g cannot be converted to a valid date: %s",
             day_num_val, e.what ());
    }

  if (d.is_not_a_date ())
    error ("boost_date_gregorian_from_day_number: day number %g produces an invalid date",
           day_num_val);

  bg::date::ymd_type ymd = d.year_month_day ();

  octave_scalar_map ret;
  ret.setfield ("year", octave_value (static_cast<double> (ymd.year)));
  ret.setfield ("month", octave_value (static_cast<double> (ymd.month)));
  ret.setfield ("day", octave_value (static_cast<double> (ymd.day)));

  return octave_value (ret);
}
