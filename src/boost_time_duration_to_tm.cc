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

DEFUN_DLD (boost_time_duration_to_tm, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tm_struct} =} boost_time_duration_to_tm (@var{hours}, @var{minutes}, @var{seconds})\n\
\n\
Convert a time duration to a @code{tm} struct (compatible with C's @code{struct tm}).\n\
\n\
The time duration is specified as @var{hours}, @var{minutes}, and @var{seconds}.\n\
\n\
Returns a struct with fields:\n\
@itemize @bullet\n\
@item @qcode{\"tm_year\"} - always 0\n\
@item @qcode{\"tm_mon\"} - always 0\n\
@item @qcode{\"tm_mday\"} - always 0\n\
@item @qcode{\"tm_wday\"} - always 0\n\
@item @qcode{\"tm_yday\"} - always 0\n\
@item @qcode{\"tm_hour\"} - hours (0-23)\n\
@item @qcode{\"tm_min\"} - minutes (0-59)\n\
@item @qcode{\"tm_sec\"} - seconds (0-59)\n\
@item @qcode{\"tm_isdst\"} - always -1\n\
@end itemize\n\
\n\
Based on @code{boost::posix_time::to_tm(time_duration)}.\n\
\n\
@example\n\
@group\n\
tm = boost_time_duration_to_tm (1, 2, 3);\n\
@result{} tm.tm_year = 0, tm.tm_mon = 0, tm.tm_mday = 0,\n\
        tm.tm_wday = 0, tm.tm_yday = 0,\n\
        tm.tm_hour = 1, tm.tm_min = 2, tm.tm_sec = 3,\n\
        tm.tm_isdst = -1\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 3)
    print_usage ();

  long hrs = args(0).long_value ();
  long mins = args(1).long_value ();
  long secs = args(2).long_value ();

  bp::time_duration td (hrs, mins, secs, 0);
  std::tm td_tm = bp::to_tm (td);

  octave_scalar_map ret;
  ret.setfield ("tm_year", octave_value (static_cast<double> (td_tm.tm_year)));
  ret.setfield ("tm_mon", octave_value (static_cast<double> (td_tm.tm_mon)));
  ret.setfield ("tm_mday", octave_value (static_cast<double> (td_tm.tm_mday)));
  ret.setfield ("tm_wday", octave_value (static_cast<double> (td_tm.tm_wday)));
  ret.setfield ("tm_yday", octave_value (static_cast<double> (td_tm.tm_yday)));
  ret.setfield ("tm_hour", octave_value (static_cast<double> (td_tm.tm_hour)));
  ret.setfield ("tm_min", octave_value (static_cast<double> (td_tm.tm_min)));
  ret.setfield ("tm_sec", octave_value (static_cast<double> (td_tm.tm_sec)));
  ret.setfield ("tm_isdst", octave_value (static_cast<double> (td_tm.tm_isdst)));

  return octave_value (ret);
}
