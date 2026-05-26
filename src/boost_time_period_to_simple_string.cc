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

DEFUN_DLD (boost_time_period_to_simple_string, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{str} =} boost_time_period_to_simple_string (@var{period})\n\
\n\
Convert a time_period to a string in the format\n\
@qcode{\"[YYYY-mmm-DD hh:mm:ss.fffffffff/YYYY-mmm-DD hh:mm:ss.fffffffff]\"}\n\
where mmm is a 3-character month name and fractional seconds are only included\n\
if non-zero. The entire string occupies one line.\n\
\n\
The input @var{period} can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"begin\"} (ptime struct) and @qcode{\"end\"} (ptime struct)\n\
  or @qcode{\"begin\"} and @qcode{\"length\"} (time_duration struct)\n\
@item Two ptimes (each as struct or string)\n\
@item A ptime (struct/string) and a time_duration (3-4 numerics)\n\
@end itemize\n\
\n\
Based on @code{boost::posix_time::to_simple_string(time_period)}.\n\
\n\
Example:\n\
@example\n\
@group\n\
str = boost_time_period_to_simple_string ('2002-01-01 01:25:10', '2002-01-31 01:25:10.123456789');\n\
@result{} str = \"[2002-Jan-01 01:25:10/2002-Jan-31 01:25:10.123456789]\"\n\
@end group\n\
@end example\n\
@seealso{boost_time_period_begin, boost_time_period_end, boost_time_period_length}\n\
@end deftypefn")
{
  if (args.length () < 1)
    print_usage ();

  auto res = parse_time_period (args, 0);
  bp::time_period tp = res.period;

  return octave_value (bp::to_simple_string (tp));
}
