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

DEFUN_DLD (boost_time_duration_plus_time_duration, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{td} =} boost_time_duration_plus_time_duration (@var{td1}, @var{td2})\n\
\n\
Add two time_duration values and return the result as a new time_duration.\n\
\n\
Each time_duration can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"hours\"}, @qcode{\"minutes\"}, @qcode{\"seconds\"}, and optionally @qcode{\"fractional_seconds\"}\n\
@item A string in @qcode{\"HH:MM:SS.fff\"} format\n\
@item 3 or 4 numeric arguments: @var{hours}, @var{minutes}, @var{seconds}[, @var{fractional_seconds}]\n\
@end itemize\n\
\n\
Returns a struct with fields @qcode{\"hours\"}, @qcode{\"minutes\"}, @qcode{\"seconds\"},\n\
@qcode{\"fractional_seconds\"}, and @qcode{\"total_seconds\"}.\n\
\n\
Based on @code{boost::posix_time::time_duration::operator+}.\n\
\n\
@example\n\
@group\n\
td = boost_time_duration_plus_time_duration (1, 2, 30, 0, 10, 30);\n\
@result{} td.hours = 1, td.minutes = 13, td.seconds = 0\n\
@end group\n\
@end example\n\
@seealso{boost_time_duration_minus_time_duration, boost_chrono_duration_add, boost_time_duration_hours}\n\
@end deftypefn")
{
  if (args.length () < 2)
    print_usage ();

  // Determine consumed count for td1
  int n1;
  if (args(0).isstruct () || args(0).is_string ())
    n1 = 1;
  else
    n1 = (args.length () >= 7) ? 4 : 3;

  bp::time_duration td1 = parse_time_duration (args, 0);
  bp::time_duration td2 = parse_time_duration (args, n1);

  bp::time_duration result = td1 + td2;

  return octave_value (time_duration_to_struct (result));
}
