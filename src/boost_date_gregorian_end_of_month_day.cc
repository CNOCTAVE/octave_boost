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

DEFUN_DLD (boost_date_gregorian_end_of_month_day, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{day} =} boost_date_gregorian_end_of_month_day (@var{year}, @var{month})\n\
\n\
Return the last day of the month for a given year and month.\n\
This is a calendar-level function that does not require creating a date object.\n\
\n\
Based on @code{boost::gregorian::gregorian_calendar::end_of_month_day()}.\n\
\n\
@example\n\
@group\n\
d = boost_date_gregorian_end_of_month_day (2000, 2);\n\
@result{} d = 29\n\
\n\
d = boost_date_gregorian_end_of_month_day (2001, 2);\n\
@result{} d = 28\n\
\n\
d = boost_date_gregorian_end_of_month_day (2002, 1);\n\
@result{} d = 31\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 2)
    print_usage ();

  double y = args (0).double_value ();
  double m = args (1).double_value ();

  bg::gregorian_calendar::month_type mo = static_cast<bg::gregorian_calendar::month_type> (m);
  bg::gregorian_calendar::year_type yr = static_cast<bg::gregorian_calendar::year_type> (y);

  short last_day = bg::gregorian_calendar::end_of_month_day (yr, mo);

  return octave_value (static_cast<double> (last_day));
}
