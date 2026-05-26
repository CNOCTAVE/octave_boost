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

DEFUN_DLD (boost_time_period_begin, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{ptime} =} boost_time_period_begin (@var{period})\n\
\n\
Return the first time (begin) of a time_period.\n\
\n\
The input @var{period} can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"begin\"} (ptime struct) and @qcode{\"end\"} (ptime struct)\n\
  or @qcode{\"begin\"} and @qcode{\"length\"} (time_duration struct)\n\
@item Two ptimes (each as struct or string)\n\
@item A ptime (struct/string) and a time_duration (3-4 numerics)\n\
@end itemize\n\
\n\
Returns a ptime struct with fields @qcode{\"date\"} and @qcode{\"time_of_day\"}.\n\
\n\
Based on @code{boost::posix_time::time_period::begin()}.\n\
\n\
Example:\n\
@example\n\
@group\n\
pt = boost_time_period_begin (struct ('begin', '2002-01-10 00:00:10', 'end', '2002-01-10 10:00:00'));\n\
@result{} pt.date.year = 2002, pt.date.month = 1, pt.date.day = 10, pt.time_of_day.hours = 0, ...\n\
@end group\n\
@end example\n\
@seealso{boost_time_period_last, boost_time_period_end}\n\
@end deftypefn")
{
  if (args.length () < 1)
    print_usage ();

  auto res = parse_time_period (args, 0);
  bp::time_period tp = res.period;

  return octave_value (ptime_to_struct (tp.begin ()));
}
