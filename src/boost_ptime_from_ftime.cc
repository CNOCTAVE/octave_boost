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

#if defined (BOOST_HAS_FTIME)
#  if defined (_WIN32)
#    include <windows.h>
#  endif
#endif

DEFUN_DLD (boost_ptime_from_ftime, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{ptime} =} boost_ptime_from_ftime (@var{dwHighDateTime}, @var{dwLowDateTime})\n\
\n\
Create a ptime from a @code{FILETIME} struct components (Windows file time).\n\
\n\
The input consists of two 32-bit integers:\n\
@itemize @bullet\n\
@item @var{dwHighDateTime} - the high 32 bits of the file time\n\
@item @var{dwLowDateTime} - the low 32 bits of the file time\n\
@end itemize\n\
\n\
@code{FILETIME} represents the number of 100-nanosecond intervals since\n\
January 1, 1601 (UTC).\n\
\n\
Returns a struct with fields @qcode{\"date\"} and @qcode{\"time_of_day\"}.\n\
\n\
Based on @code{boost::posix_time::from_ftime<ptime>()}.\n\
\n\
@example\n\
@group\n\
pt = boost_ptime_from_ftime (29715317, 3865122988);\n\
@result{} pt.date.year = 2005, pt.date.month = 6, pt.date.day = 7,\n\
        pt.time_of_day.hours = 15, pt.time_of_day.minutes = 30, pt.time_of_day.seconds = 57\n\
@end group\n\
@end example\n\
@end deftypefn")
{
#if defined (BOOST_HAS_FTIME)
  if (args.length () < 2)
    print_usage ();

  FILETIME ft;
  ft.dwHighDateTime = static_cast<DWORD> (args(0).long_value ());
  ft.dwLowDateTime  = static_cast<DWORD> (args(1).long_value ());

  bp::ptime pt = bp::from_ftime<bp::ptime> (ft);

  return octave_value (ptime_to_struct (pt));
#else
  error ("boost_ptime_from_ftime: from_ftime is not available on this platform (requires FILETIME support)");
  return octave_value ();
#endif
}
