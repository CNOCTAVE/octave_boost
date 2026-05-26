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

DEFUN_DLD (boost_chrono_duration_add, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_chrono_duration_add (@var{value1}, @var{unit1}, @var{value2}, @var{unit2})\n\
\n\
Add two durations. Each duration is specified by a numeric value and a unit string.\n\
The result is returned as a struct with fields @qcode{\"count\"} and\n\
@qcode{\"unit\"}, representing the sum in the common type.\n\
\n\
Unit strings: 'hours', 'minutes', 'seconds', 'milliseconds', 'microseconds', 'nanoseconds'\n\
\n\
@example\n\
@group\n\
r = boost_chrono_duration_add (3, 'milliseconds', 10, 'microseconds');\n\
@result{} r.count = 3010, r.unit = microseconds\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 4)
    print_usage ();

  double v1 = args(0).xdouble_value ("boost_chrono_duration_add: VALUE1 must be a numeric scalar");

  std::string u1 = args(1).xstring_value ("boost_chrono_duration_add: UNIT1 must be a string");

  double v2 = args(2).xdouble_value ("boost_chrono_duration_add: VALUE2 must be a numeric scalar");

  std::string u2 = args(3).xstring_value ("boost_chrono_duration_add: UNIT2 must be a string");

  ChronoUnit unit1 = parse_chrono_unit (u1);
  ChronoUnit unit2 = parse_chrono_unit (u2);

  // Convert both to nanoseconds, add, return result
  auto ns1 = to_nanoseconds (v1, unit1);
  auto ns2 = to_nanoseconds (v2, unit2);
  auto result = ns1 + ns2;

  octave_scalar_map ret;
  ret.setfield ("count", octave_value (static_cast<double> (result.count ())));
  ret.setfield ("unit", octave_value ("nanoseconds"));

  return octave_value (ret);
}
