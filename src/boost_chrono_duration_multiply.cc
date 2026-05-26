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

DEFUN_DLD (boost_chrono_duration_multiply, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} boost_chrono_duration_multiply (@var{value}, @var{unit}, @var{factor})\n\
\n\
Multiply a duration by a scalar factor. The duration is specified by a numeric\n\
value and a unit string. The result is returned as a struct with fields\n\
@qcode{\"count\"} and @qcode{\"unit\"}.\n\
\n\
Unit strings: 'hours', 'minutes', 'seconds', 'milliseconds', 'microseconds', 'nanoseconds'\n\
\n\
@example\n\
@group\n\
r = boost_chrono_duration_multiply (5, 'seconds', 2);\n\
@result{} r.count = 10000000000, r.unit = nanoseconds\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 3)
    print_usage ();

  double value = args(0).xdouble_value ("boost_chrono_duration_multiply: VALUE must be a numeric scalar");

  std::string unit_str = args(1).xstring_value ("boost_chrono_duration_multiply: UNIT must be a string");

  double factor = args(2).xdouble_value ("boost_chrono_duration_multiply: FACTOR must be a numeric scalar");

  ChronoUnit unit = parse_chrono_unit (unit_str);
  auto ns = to_nanoseconds (value, unit);
  auto result = ns * static_cast<long> (factor);

  octave_scalar_map ret;
  ret.setfield ("count", octave_value (static_cast<double> (result.count ())));
  ret.setfield ("unit", octave_value ("nanoseconds"));

  return octave_value (ret);
}
