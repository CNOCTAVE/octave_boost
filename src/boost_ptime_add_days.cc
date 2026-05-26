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

DEFUN_DLD (boost_ptime_add_days, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{ptime_out} =} boost_ptime_add_days (@var{ptime}, @var{days})\n\
\n\
Add a number of days to a ptime and return the new ptime.\n\
\n\
The input @var{ptime} can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"date\"} (date struct) and optionally @qcode{\"time_of_day\"}\n\
@item A string in @qcode{\"YYYY-MM-DD HH:MM:SS\"} or ISO extended format\n\
@item A special value string: @qcode{\"neg_infin\"}, @qcode{\"pos_infin\"}, @qcode{\"not_a_date_time\"}\n\
@end itemize\n\
The @var{days} parameter is an integer number of days to add.\n\
\n\
Returns a struct with fields @qcode{\"date\"} and @qcode{\"time_of_day\"}.\n\
\n\
Based on @code{boost::posix_time::ptime + boost::gregorian::days}.\n\
\n\
@example\n\
@group\n\
t = boost_ptime_add_days (\"2002-01-01 00:05:00\", 1);\n\
@result{} t.date.year = 2002, t.date.month = 1, t.date.day = 2,\n\
        t.time_of_day.hours = 0, t.time_of_day.minutes = 5\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2)
    print_usage ();

  bp::ptime t = parse_ptime (args, 0);
  long ndays = args(1).long_value ();

  bp::ptime result = t + bg::days (ndays);

  return octave_value (ptime_to_struct (result));
}
