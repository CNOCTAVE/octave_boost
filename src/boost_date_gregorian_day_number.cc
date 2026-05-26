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

DEFUN_DLD (boost_date_gregorian_day_number, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{num} =} boost_date_gregorian_day_number (@var{date})\n\
\n\
Convert a date into a day number using the Gregorian calendar.\n\
The day number is an implementation-defined serial day count.\n\
The library epoch (1400-01-01) has day number 2232400.\n\
\n\
The input @var{date} can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"year\"}, @qcode{\"month\"}, @qcode{\"day\"}\n\
@item A string in @qcode{\"YYYY-MM-DD\"} format or a special value\n\
@item Three numeric arguments: @var{year}, @var{month}, @var{day}\n\
@end itemize\n\
\n\
Based on @code{boost::gregorian::gregorian_calendar::day_number()}.\n\
\n\
@example\n\
@group\n\
num = boost_date_gregorian_day_number (2002, 1, 10);\n\
@result{} num = 731228\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1 || args.length () > 3)
    print_usage ();

  bg::date d = parse_date (args, 0);
  if (d.is_not_a_date ())
    error ("boost_date_gregorian_day_number: date is not-a-date-time");

  bg::date::ymd_type ymd = d.year_month_day ();
  bg::gregorian_calendar::date_int_type day_num
    = bg::gregorian_calendar::day_number (ymd);

  return octave_value (static_cast<double> (day_num));
}
