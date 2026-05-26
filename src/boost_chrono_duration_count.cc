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

#include "boost_chrono_common.h"

DEFUN_DLD (boost_chrono_duration_count, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{cnt} =} boost_chrono_duration_count (@var{value}, @var{unit})\n\
\n\
Return the tick count of a duration specified by @var{value} and @var{unit}.\n\
The unit is a string: 'hours', 'minutes', 'seconds', 'milliseconds',\n\
'microseconds', or 'nanoseconds'.\n\
\n\
@example\n\
@group\n\
cnt = boost_chrono_duration_count (5, 'seconds');\n\
@result{} cnt = 5\n\
cnt = boost_chrono_duration_count (1000, 'milliseconds');\n\
@result{} cnt = 1000\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 2)
    print_usage ();

  double value = args(0).xdouble_value ("boost_chrono_duration_count: VALUE must be a numeric scalar");

  std::string unit_str = args(1).xstring_value ("boost_chrono_duration_count: UNIT must be a string");

  ChronoUnit unit = parse_chrono_unit (unit_str);

  // Convert to nanoseconds and count
  auto ns = to_nanoseconds (value, unit);
  return octave_value (static_cast<double> (ns.count ()));
}
