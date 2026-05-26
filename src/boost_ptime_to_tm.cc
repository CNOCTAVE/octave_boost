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
#include <ctime>

DEFUN_DLD (boost_ptime_to_tm, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tm_struct} =} boost_ptime_to_tm (@var{ptime})\n\
\n\
Convert a ptime to a @code{tm} struct (compatible with C's @code{struct tm}).\n\
\n\
The input @var{ptime} can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"date\"} (date struct) and optionally @qcode{\"time_of_day\"}\n\
@item A string in @qcode{\"YYYY-MM-DD HH:MM:SS\"} or ISO extended format\n\
@item A special value string: @qcode{\"neg_infin\"}, @qcode{\"pos_infin\"}, @qcode{\"not_a_date_time\"}\n\
@end itemize\n\
\n\
Returns a struct with fields:\n\
@itemize @bullet\n\
@item @qcode{\"tm_year\"} - years since 1900 (e.g., 105 for 2005)\n\
@item @qcode{\"tm_mon\"} - month (0-11, 0=January)\n\
@item @qcode{\"tm_mday\"} - day of month (1-31)\n\
@item @qcode{\"tm_wday\"} - day of week (0=Sunday, 6=Saturday)\n\
@item @qcode{\"tm_yday\"} - day of year (0-365)\n\
@item @qcode{\"tm_hour\"} - hours (0-23)\n\
@item @qcode{\"tm_min\"} - minutes (0-59)\n\
@item @qcode{\"tm_sec\"} - seconds (0-59)\n\
@item @qcode{\"tm_isdst\"} - always -1\n\
@end itemize\n\
\n\
Based on @code{boost::posix_time::to_tm()}.\n\
\n\
@example\n\
@group\n\
tm = boost_ptime_to_tm (\"2005-01-01 01:02:03\");\n\
@result{} tm.tm_year = 105, tm.tm_mon = 0, tm.tm_mday = 1,\n\
        tm.tm_wday = 6, tm.tm_yday = 0,\n\
        tm.tm_hour = 1, tm.tm_min = 2, tm.tm_sec = 3,\n\
        tm.tm_isdst = -1\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1)
    print_usage ();

  bp::ptime t = parse_ptime (args, 0);

  if (t.is_not_a_date_time () || t.is_infinity ())
    error ("boost_ptime_to_tm: cannot convert special ptime value to tm struct");

  std::tm pt_tm = bp::to_tm (t);

  octave_scalar_map ret;
  ret.setfield ("tm_year", octave_value (static_cast<double> (pt_tm.tm_year)));
  ret.setfield ("tm_mon", octave_value (static_cast<double> (pt_tm.tm_mon)));
  ret.setfield ("tm_mday", octave_value (static_cast<double> (pt_tm.tm_mday)));
  ret.setfield ("tm_wday", octave_value (static_cast<double> (pt_tm.tm_wday)));
  ret.setfield ("tm_yday", octave_value (static_cast<double> (pt_tm.tm_yday)));
  ret.setfield ("tm_hour", octave_value (static_cast<double> (pt_tm.tm_hour)));
  ret.setfield ("tm_min", octave_value (static_cast<double> (pt_tm.tm_min)));
  ret.setfield ("tm_sec", octave_value (static_cast<double> (pt_tm.tm_sec)));
  ret.setfield ("tm_isdst", octave_value (static_cast<double> (pt_tm.tm_isdst)));

  return octave_value (ret);
}
