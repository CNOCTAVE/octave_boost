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

DEFUN_DLD (boost_date_get_year_month_day, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{ymd} =} boost_date_get_year_month_day (@var{date})\n\
\n\
Return a struct with fields @qcode{\"year\"}, @qcode{\"month\"}, @qcode{\"day\"}\n\
from a Boost.Gregorian date.\n\
\n\
The input @var{date} can be specified in any of the following forms:\n\
@itemize\n\
@item A struct with fields @qcode{\"year\"}, @qcode{\"month\"}, @qcode{\"day\"}\n\
@item A string in the format @qcode{\"YYYY-MM-DD\"}\n\
@item A special value string: @qcode{\"neg_infin\"}, @qcode{\"pos_infin\"}, @qcode{\"not_a_date_time\"}\n\
@item Three numeric arguments: @var{year}, @var{month}, @var{day}\n\
@end itemize\n\
\n\
Based on @code{boost::gregorian::date::year_month_day()}.\n\
\n\
@example\n\
@group\n\
ymd = boost_date_get_year_month_day (2002, 1, 10);\n\
@result{} ymd.year = 2002, ymd.month = 1, ymd.day = 10\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1 || args.length () > 3)
    print_usage ();

  bg::date d = parse_date (args, 0);
  if (d.is_not_a_date ())
    error ("boost_date_get_year_month_day: date is not-a-date-time");

  bg::date::ymd_type ymd = d.year_month_day ();

  octave_scalar_map ret;
  ret.setfield ("year", octave_value (static_cast<double> (ymd.year)));
  ret.setfield ("month", octave_value (static_cast<double> (ymd.month)));
  ret.setfield ("day", octave_value (static_cast<double> (ymd.day)));

  return octave_value (ret);
}
