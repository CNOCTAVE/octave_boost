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

DEFUN_DLD (boost_time_duration_to_simple_string, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{str} =} boost_time_duration_to_simple_string (@var{td})\n\
\n\
Convert a time duration to a string in the format \"HH:MM:SS.fffffffff\",\n\
where fractional seconds are only included if non-zero.\n\
\n\
The input @var{td} can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"hours\"}, @qcode{\"minutes\"}, @qcode{\"seconds\"}, and optionally @qcode{\"fractional_seconds\"}\n\
@item A string in @qcode{\"HH:MM:SS.fff\"} format\n\
@item 3 or 4 numeric arguments: @var{hours}, @var{minutes}, @var{seconds}[, @var{fractional_seconds}]\n\
@end itemize\n\
\n\
Based on @code{boost::posix_time::to_simple_string(time_duration)}.\n\
\n\
@example\n\
@group\n\
boost_time_duration_to_simple_string (10, 0, 1, 123456789)\n\
@result{} \"10:00:01.123456789\"\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1)
    print_usage ();

  bp::time_duration td = parse_time_duration (args, 0);
  if (td.is_not_a_date_time ())
    error ("boost_time_duration_to_simple_string: time_duration is not-a-date-time");

  return octave_value (bp::to_simple_string (td));
}
