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

DEFUN_DLD (boost_time_duration_eq, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tf} =} boost_time_duration_eq (@var{td1}, @var{td2})\n\
\n\
Compare two time durations for equality.\n\
\n\
Each input @var{td} can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"hours\"}, @qcode{\"minutes\"}, @qcode{\"seconds\"}, and optionally @qcode{\"fractional_seconds\"}\n\
@item A string in @qcode{\"HH:MM:SS.fff\"} format\n\
@item 3 or 4 numeric arguments: @var{hours}, @var{minutes}, @var{seconds}[, @var{fractional_seconds}]\n\
@end itemize\n\
\n\
Based on @code{boost::posix_time::time_duration::operator==}.\n\
\n\
@example\n\
@group\n\
boost_time_duration_eq (1, 2, 3, 1, 2, 3)\n\
@result{} 1\n\
\n\
boost_time_duration_eq (1, 2, 3, 4, 5, 6)\n\
@result{} 0\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2)
    print_usage ();

  // Split arguments for two time_duration values.
  // Each td can be 1 arg (struct/string) or 3-4 numeric args (hours,minutes,seconds[,fractional_seconds])
  // Determine where td1 ends and td2 begins.
  int n1;
  if (args (0).isstruct () || args (0).is_string ())
    n1 = 1;
  else if (args.length () >= 7)
    n1 = 4;  // 4 numeric args for td1
  else if (args.length () >= 3)
    n1 = 3;  // 3 numeric args for td1
  else
    error ("boost_time_duration_eq: invalid arguments for first time_duration");

  bp::time_duration td1 = parse_time_duration (args, 0);
  bp::time_duration td2 = parse_time_duration (args, n1);

  return octave_value (td1 == td2);
}
