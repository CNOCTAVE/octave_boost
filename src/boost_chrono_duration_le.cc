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

DEFUN_DLD (boost_chrono_duration_le, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tf} =} boost_chrono_duration_le (@var{value1}, @var{unit1}, @var{value2}, @var{unit2})\n\
\n\
Compare two durations for less-than-or-equal. Returns true if the first\n\
duration represents a time interval not longer than the second.\n\
Each duration is specified by a numeric value and a unit string.\n\
\n\
Unit strings: 'hours', 'minutes', 'seconds', 'milliseconds', 'microseconds', 'nanoseconds'\n\
\n\
@example\n\
@group\n\
tf = boost_chrono_duration_le (1000, 'milliseconds', 1, 'seconds');\n\
@result{} tf = 1 (true)\n\
tf = boost_chrono_duration_le (31, 'seconds', 1, 'minutes');\n\
@result{} tf = 0 (false)\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 4)
    print_usage ();

  double v1 = args(0).xdouble_value ("boost_chrono_duration_le: VALUE1 must be a numeric scalar");

  std::string u1 = args(1).xstring_value ("boost_chrono_duration_le: UNIT1 must be a string");

  double v2 = args(2).xdouble_value ("boost_chrono_duration_le: VALUE2 must be a numeric scalar");

  std::string u2 = args(3).xstring_value ("boost_chrono_duration_le: UNIT2 must be a string");

  ChronoUnit unit1 = parse_chrono_unit (u1);
  ChronoUnit unit2 = parse_chrono_unit (u2);

  auto ns1 = to_nanoseconds (v1, unit1);
  auto ns2 = to_nanoseconds (v2, unit2);

  return octave_value (ns1 <= ns2);
}
