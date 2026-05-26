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

DEFUN_DLD (boost_ptime_from_tm, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{ptime} =} boost_ptime_from_tm (@var{tm_struct})\n\
\n\
Convert a @code{tm} struct back to a ptime.\n\
\n\
The input @var{tm_struct} must be a struct with at least these fields:\n\
@itemize @bullet\n\
@item @qcode{\"tm_year\"} - years since 1900\n\
@item @qcode{\"tm_mon\"} - month (0-11, 0=January)\n\
@item @qcode{\"tm_mday\"} - day of month (1-31)\n\
@item @qcode{\"tm_hour\"} - hours (0-23) (optional, default 0)\n\
@item @qcode{\"tm_min\"} - minutes (0-59) (optional, default 0)\n\
@item @qcode{\"tm_sec\"} - seconds (0-59) (optional, default 0)\n\
@end itemize\n\
\n\
Returns a struct with fields @qcode{\"date\"} and @qcode{\"time_of_day\"}.\n\
\n\
Based on @code{boost::posix_time::ptime_from_tm()}.\n\
\n\
@example\n\
@group\n\
tm.tm_year = 105;\n\
tm.tm_mon = 0;\n\
tm.tm_mday = 1;\n\
tm.tm_hour = 1;\n\
tm.tm_min = 2;\n\
tm.tm_sec = 3;\n\
pt = boost_ptime_from_tm (tm);\n\
@result{} pt.date.year = 2005, pt.date.month = 1, pt.date.day = 1,\n\
        pt.time_of_day.hours = 1, pt.time_of_day.minutes = 2, pt.time_of_day.seconds = 3\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1)
    print_usage ();

  if (! args(0).isstruct ())
    error ("boost_ptime_from_tm: argument must be a struct with fields 'tm_year', 'tm_mon', 'tm_mday'");

  octave_scalar_map m = args(0).scalar_map_value ();

  if (! m.isfield ("tm_year") || ! m.isfield ("tm_mon") || ! m.isfield ("tm_mday"))
    error ("boost_ptime_from_tm: struct must have fields 'tm_year', 'tm_mon', 'tm_mday'");

  std::tm pt_tm;
  pt_tm.tm_year = static_cast<int> (m.getfield ("tm_year").double_value ());
  pt_tm.tm_mon  = static_cast<int> (m.getfield ("tm_mon").double_value ());
  pt_tm.tm_mday = static_cast<int> (m.getfield ("tm_mday").double_value ());
  pt_tm.tm_hour = m.isfield ("tm_hour") ? static_cast<int> (m.getfield ("tm_hour").double_value ()) : 0;
  pt_tm.tm_min  = m.isfield ("tm_min") ? static_cast<int> (m.getfield ("tm_min").double_value ()) : 0;
  pt_tm.tm_sec  = m.isfield ("tm_sec") ? static_cast<int> (m.getfield ("tm_sec").double_value ()) : 0;
  pt_tm.tm_wday = 0;
  pt_tm.tm_yday = 0;
  pt_tm.tm_isdst = -1;

  bp::ptime pt = bp::ptime_from_tm (pt_tm);

  return octave_value (ptime_to_struct (pt));
}
