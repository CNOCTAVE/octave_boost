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

DEFUN_DLD (boost_time_duration_multiply_by_days, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{td} =} boost_time_duration_multiply_by_days (@var{td}, @var{multiplier})\n\
\n\
Multiply a time_duration by an integer multiplier and return the result.\n\
\n\
The time_duration can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"hours\"}, @qcode{\"minutes\"}, @qcode{\"seconds\"}, and optionally @qcode{\"fractional_seconds\"}\n\
@item A string in @qcode{\"HH:MM:SS.fff\"} format\n\
@item 3 or 4 numeric arguments: @var{hours}, @var{minutes}, @var{seconds}[, @var{fractional_seconds}]\n\
@end itemize\n\
\n\
The @var{multiplier} is an integer scalar.\n\
\n\
Returns a struct with fields @qcode{\"hours\"}, @qcode{\"minutes\"}, @qcode{\"seconds\"},\n\
@qcode{\"fractional_seconds\"}, and @qcode{\"total_seconds\"}.\n\
\n\
Based on @code{boost::posix_time::time_duration::operator*}.\n\
\n\
@example\n\
@group\n\
td = boost_time_duration_multiply_by_days (3, 0, 0, 2);\n\
@result{} td.hours = 6, td.minutes = 0, td.seconds = 0\n\
@end group\n\
@end example\n\
@seealso{boost_time_duration_divide_by_days, boost_chrono_duration_multiply, boost_time_duration_hours}\n\
@end deftypefn")
{
  if (args.length () < 2)
    print_usage ();

  int n1;
  if (args(0).isstruct () || args(0).is_string ())
    n1 = 1;
  else
    n1 = (args.length () > 4) ? 4 : 3;

  bp::time_duration td = parse_time_duration (args, 0);

  double multiplier = args(n1).double_value ();

  bp::time_duration result = td * static_cast<long> (multiplier);

  return octave_value (time_duration_to_struct (result));
}
