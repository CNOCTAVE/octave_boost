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

DEFUN_DLD (boost_ptime_lt, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tf} =} boost_ptime_lt (@var{ptime1}, @var{ptime2})\n\
\n\
Compare if the first ptime is less than the second.\n\
\n\
Each input @var{ptime} can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"date\"} (date struct) and optionally @qcode{\"time_of_day\"}\n\
@item A string in @qcode{\"YYYY-MM-DD HH:MM:SS\"} or ISO extended format\n\
@item A special value string: @qcode{\"neg_infin\"}, @qcode{\"pos_infin\"}, @qcode{\"not_a_date_time\"}\n\
@end itemize\n\
\n\
Based on @code{boost::posix_time::ptime::operator<}.\n\
\n\
@example\n\
@group\n\
tf = boost_ptime_lt (\"2002-01-10 01:00:00\", \"2002-01-11 01:00:00\");\n\
@result{} tf = 1 (true)\n\
\n\
tf = boost_ptime_lt (\"2002-01-11 01:00:00\", \"2002-01-10 01:00:00\");\n\
@result{} tf = 0 (false)\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2)
    print_usage ();

  bp::ptime t1 = parse_ptime (args, 0);
  bp::ptime t2 = parse_ptime (args, 1);

  return octave_value (t1 < t2);
}
