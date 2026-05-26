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

DEFUN_DLD (boost_time_period_contains_ptime, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tf} =} boost_time_period_contains_ptime (@var{period}, @var{ptime})\n\
\n\
Check if a ptime is contained within a time_period. Zero-length periods cannot\n\
contain any points. Returns logical true if the point is within the period.\n\
\n\
The @var{period} can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"begin\"} (ptime struct) and @qcode{\"end\"} (ptime struct)\n\
  or @qcode{\"begin\"} and @qcode{\"length\"} (time_duration struct)\n\
@item Two ptimes (each as struct or string)\n\
@item A ptime (struct/string) and a time_duration (3-4 numerics)\n\
@end itemize\n\
\n\
The @var{ptime} can be specified as a struct or a datetime string.\n\
\n\
Based on @code{boost::posix_time::time_period::contains()}.\n\
\n\
Example:\n\
@example\n\
@group\n\
tf = boost_time_period_contains_ptime ('2002-01-10 00:00:10', '2002-01-10 10:00:00', '2002-01-10 02:00:00');\n\
@result{} tf = 1 (true)\n\
@end group\n\
@end example\n\
@seealso{boost_time_period_begin, boost_time_period_end, boost_time_period_is_null}\n\
@end deftypefn")
{
  if (args.length () < 2)
    print_usage ();

  auto res = parse_time_period (args, 0);
  bp::time_period tp = res.period;

  bp::ptime pt = parse_ptime (args, res.consumed);

  return octave_value (tp.contains (pt));
}
