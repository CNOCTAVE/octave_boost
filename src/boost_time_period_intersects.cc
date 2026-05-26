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

DEFUN_DLD (boost_time_period_intersects, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tf} =} boost_time_period_intersects (@var{period1}, @var{period2})\n\
\n\
Check if two time_periods overlap. Returns logical true if the periods have any\n\
overlap (intersection is non-null).\n\
\n\
Each @var{period} can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"begin\"} (ptime struct) and @qcode{\"end\"} (ptime struct)\n\
  or @qcode{\"begin\"} and @qcode{\"length\"} (time_duration struct)\n\
@item Two ptimes (each as struct or string)\n\
@item A ptime (struct/string) and a time_duration (3-4 numerics)\n\
@end itemize\n\
\n\
Based on @code{boost::posix_time::time_period::intersects(time_period)}.\n\
\n\
Example:\n\
@example\n\
@group\n\
tp1 = struct ('begin', '2002-01-01 01:00:00', 'end', '2002-01-01 12:00:00');\n\
tp2 = struct ('begin', '2002-01-01 02:00:00', 'end', '2002-01-01 04:00:00');\n\
tf = boost_time_period_intersects (tp1, tp2);\n\
@result{} tf = 1 (true)\n\
@end group\n\
@end example\n\
@seealso{boost_time_period_contains_time_period, boost_time_period_intersection, boost_time_period_merge}\n\
@end deftypefn")
{
  if (args.length () < 2)
    print_usage ();

  auto res1 = parse_time_period (args, 0);
  bp::time_period tp1 = res1.period;

  auto res2 = parse_time_period (args, res1.consumed);
  bp::time_period tp2 = res2.period;

  return octave_value (tp1.intersects (tp2));
}
