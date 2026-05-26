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

DEFUN_DLD (boost_ptime_to_iso_extended_string, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{str} =} boost_ptime_to_iso_extended_string (@var{ptime})\n\
\n\
Convert a ptime to ISO 8601 extended format string \"YYYY-MM-DDTHH:MM:SS.fffffffff\"\n\
where T is the date-time separator and fractional seconds are only\n\
included if non-zero.\n\
\n\
The input @var{ptime} can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"date\"} (date struct) and optionally @qcode{\"time_of_day\"}\n\
@item A string in @qcode{\"YYYY-MM-DD HH:MM:SS\"} or ISO extended format\n\
@item A special value string: @qcode{\"neg_infin\"}, @qcode{\"pos_infin\"}, @qcode{\"not_a_date_time\"}\n\
@end itemize\n\
\n\
Based on @code{boost::posix_time::to_iso_extended_string()}.\n\
\n\
@example\n\
@group\n\
str = boost_ptime_to_iso_extended_string (\"2002-01-31 10:00:01\");\n\
@result{} str = \"2002-01-31T10:00:01\"\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1)
    print_usage ();

  bp::ptime t = parse_ptime (args, 0);
  if (t.is_not_a_date_time ())
    error ("boost_ptime_to_iso_extended_string: ptime is not-a-date-time");

  return octave_value (bp::to_iso_extended_string (t));
}
