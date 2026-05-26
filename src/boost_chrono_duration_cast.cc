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

DEFUN_DLD (boost_chrono_duration_cast, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{value} =} boost_chrono_duration_cast (@var{count}, @var{unit}, @var{target_unit})\n\
\n\
Cast a duration from one time unit to another. The input duration is\n\
specified by a numeric count and a unit string. The target unit is also\n\
a string. Returns the count in the target unit (as a floating-point value).\n\
\n\
Unit strings: 'hours', 'minutes', 'seconds', 'milliseconds', 'microseconds', 'nanoseconds'\n\
\n\
@example\n\
@group\n\
v = boost_chrono_duration_cast (3600, 'seconds', 'hours');\n\
@result{} v = 1.0\n\
v = boost_chrono_duration_cast (1000, 'milliseconds', 'seconds');\n\
@result{} v = 1.0\n\
v = boost_chrono_duration_cast (1, 'hours', 'seconds');\n\
@result{} v = 3600.0\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 3)
    print_usage ();

  double count = args(0).xdouble_value ("boost_chrono_duration_cast: COUNT must be a numeric scalar");

  std::string unit_str = args(1).xstring_value ("boost_chrono_duration_cast: UNIT must be a string");

  std::string target_str = args(2).xstring_value ("boost_chrono_duration_cast: TARGET_UNIT must be a string");

  ChronoUnit src_unit = parse_chrono_unit (unit_str);
  ChronoUnit dst_unit = parse_chrono_unit (target_str);

  // Convert source to nanoseconds first
  auto ns = to_nanoseconds (count, src_unit);

  // Convert nanoseconds to target unit
  double result = 0.0;
  switch (dst_unit)
    {
    case ChronoUnit::HOURS:
      {
        auto d = boost::chrono::duration_cast<boost::chrono::hours> (ns);
        result = static_cast<double> (d.count ());
        break;
      }
    case ChronoUnit::MINUTES:
      {
        auto d = boost::chrono::duration_cast<boost::chrono::minutes> (ns);
        result = static_cast<double> (d.count ());
        break;
      }
    case ChronoUnit::SECONDS:
      {
        auto d = boost::chrono::duration_cast<boost::chrono::seconds> (ns);
        result = static_cast<double> (d.count ());
        break;
      }
    case ChronoUnit::MILLISECONDS:
      {
        auto d = boost::chrono::duration_cast<boost::chrono::milliseconds> (ns);
        result = static_cast<double> (d.count ());
        break;
      }
    case ChronoUnit::MICROSECONDS:
      {
        auto d = boost::chrono::duration_cast<boost::chrono::microseconds> (ns);
        result = static_cast<double> (d.count ());
        break;
      }
    case ChronoUnit::NANOSECONDS:
      {
        result = static_cast<double> (ns.count ());
        break;
      }
    }

  return octave_value (result);
}
