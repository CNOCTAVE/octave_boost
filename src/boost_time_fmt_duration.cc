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
// Use v2 chrono I/O API which provides time_fmt manipulator and timezone enum
#include <boost/chrono/io/time_point_io.hpp>
#include <sstream>

DEFUN_DLD (boost_time_fmt_duration, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{str} =} boost_time_fmt_duration (@var{format}, @var{count}, @var{unit})\n\
\n\
Format a duration as a time string using the given @var{format} pattern.\n\
The duration is specified by @var{count} (numeric value) and @var{unit}\n\
(string: hours, minutes, seconds, milliseconds, microseconds, nanoseconds).\n\
The format string follows the C/POSIX strftime pattern (e.g.\n\
\"%H:%M:%S\").  If the duration exceeds 24 hours, %H will wrap;\n\
use %X or custom patterns as needed.\n\
\n\
Based on boost::chrono::time_fmt I/O manipulator.\n\
\n\
@example\n\
@group\n\
str = boost_time_fmt_duration (\"%H:%M:%S\", 3661, \"seconds\");\n\
@result{} str = '01:01:01'\n\
str = boost_time_fmt_duration (\"%M minutes %S seconds\", 90, \"seconds\");\n\
@result{} str = '01 minutes 30 seconds'\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 3)
    print_usage ();

  std::string fmt = args(0).string_value ();
  double count = args(1).double_value ();
  std::string unit_str = args(2).string_value ();

  ChronoUnit unit = parse_chrono_unit (unit_str);
  auto ns = to_nanoseconds (count, unit);

  // Create a time_point from the duration (treating it as time since epoch
  // of system_clock for formatting purposes)
  boost::chrono::system_clock::time_point tp (ns);

  std::ostringstream oss;
  oss << boost::chrono::time_fmt (boost::chrono::timezone::utc, fmt)
      << tp;
  return octave_value (oss.str ());
}
