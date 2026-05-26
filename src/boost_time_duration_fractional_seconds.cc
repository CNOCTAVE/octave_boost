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

DEFUN_DLD (boost_time_duration_fractional_seconds, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{fractional_seconds} =} boost_time_duration_fractional_seconds (@var{td})\n\
\n\
Get the number of fractional seconds from a time duration.\n\
\n\
The input @var{td} can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"hours\"}, @qcode{\"minutes\"}, @qcode{\"seconds\"}, and optionally @qcode{\"fractional_seconds\"}\n\
@item A string in @qcode{\"HH:MM:SS.fff\"} format\n\
@item 3 or 4 numeric arguments: @var{hours}, @var{minutes}, @var{seconds}[, @var{fractional_seconds}]\n\
@end itemize\n\
\n\
@strong{Note:} This function will give unpredictable results if called on a special value time_duration.\n\
\n\
Based on @code{boost::posix_time::time_duration::fractional_seconds()}.\n\
\n\
@example\n\
@group\n\
boost_time_duration_fractional_seconds (1, 2, 3, 1000)\n\
@result{} 1000\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1)
    print_usage ();

  bp::time_duration td = parse_time_duration (args, 0);

  if (td.is_special ())
    error ("boost_time_duration_fractional_seconds: time_duration is a special value");

  return octave_value (static_cast<double> (td.fractional_seconds ()));
}
