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

DEFUN_DLD (boost_ptime_time_of_day, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tod} =} boost_ptime_time_of_day (@var{ptime})\n\
\n\
Extract the time-of-day part from a ptime (date-time) value.\n\
Returns a struct with fields @qcode{\"hours\"}, @qcode{\"minutes\"}, @qcode{\"seconds\"},\n\
@qcode{\"fractional_seconds\"}, and @qcode{\"total_seconds\"}.\n\
\n\
The input @var{ptime} can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"date\"} (date struct) and optionally @qcode{\"time_of_day\"}\n\
@item A string in @qcode{\"YYYY-MM-DD HH:MM:SS\"} or ISO extended format\n\
@item A special value string: @qcode{\"neg_infin\"}, @qcode{\"pos_infin\"}, @qcode{\"not_a_date_time\"}\n\
@end itemize\n\
\n\
Based on @code{boost::posix_time::ptime::time_of_day()}.\n\
\n\
@example\n\
@group\n\
tod = boost_ptime_time_of_day (\"2002-01-10 01:02:03\");\n\
@result{} tod.hours = 1, tod.minutes = 2, tod.seconds = 3\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1)
    print_usage ();

  bp::ptime t = parse_ptime (args, 0);
  if (t.is_not_a_date_time ())
    error ("boost_ptime_time_of_day: ptime is not-a-date-time");

  bp::time_duration td = t.time_of_day ();

  octave_scalar_map ret;
  ret.setfield ("hours", octave_value (static_cast<double> (td.hours ())));
  ret.setfield ("minutes", octave_value (static_cast<double> (td.minutes ())));
  ret.setfield ("seconds", octave_value (static_cast<double> (td.seconds ())));
  ret.setfield ("fractional_seconds", octave_value (static_cast<double> (td.fractional_seconds ())));
  ret.setfield ("total_seconds", octave_value (static_cast<double> (td.total_seconds ())));

  return octave_value (ret);
}
