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

DEFUN_DLD (boost_time_fmt_system_clock, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{str} =} boost_time_fmt_system_clock (@var{format})\n\
\n\
Return the current system clock time formatted according to the given\n\
@var{format} pattern, using boost::chrono::time_fmt I/O manipulator.\n\
The format string follows the C/POSIX strftime pattern.\n\
\n\
Common format specifiers:\n\
@table @code\n\
@item %%Y  Year with century (e.g., 2025)\n\
@item %%m  Month as decimal (01-12)\n\
@item %%d  Day of month (01-31)\n\
@item %%H  Hour (00-23)\n\
@item %%M  Minute (00-59)\n\
@item %%S  Second (00-60)\n\
@item %%F  Shortcut for %%Y-%%m-%%d\n\
@item %%T  Shortcut for %%H:%%M:%%S\n\
@item %%X  Locale's time representation\n\
@item %%c  Locale's date and time representation\n\
@end table\n\
\n\
Based on boost::chrono::time_fmt with boost::chrono::system_clock::now().\n\
\n\
@example\n\
@group\n\
str = boost_time_fmt_system_clock (\"%%Y-%%m-%%d %%H:%%M:%%S\");\n\
@result{} str = '2025-06-01 12:34:56'\n\
str = boost_time_fmt_system_clock (\"%%F\");\n\
@result{} str = '2025-06-01'\n\
str = boost_time_fmt_system_clock (\"%%T\");\n\
@result{} str = '12:34:56'\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 1)
    print_usage ();

  std::string fmt = args(0).string_value ();

  auto tp = boost::chrono::system_clock::now ();

  std::ostringstream oss;
  oss << boost::chrono::time_fmt (boost::chrono::timezone::utc, fmt)
      << tp;
  return octave_value (oss.str ());
}
