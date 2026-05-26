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

DEFUN_DLD (boost_ptime_from_time_t, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{ptime} =} boost_ptime_from_time_t (@var{time_t_value})\n\
\n\
Create a ptime from a @code{time_t} value (seconds since 1970-01-01 00:00:00 UTC).\n\
\n\
The input @var{time_t_value} is a numeric scalar representing the number of seconds\n\
since the Unix epoch (1970-Jan-01).\n\
\n\
Returns a struct with fields @qcode{\"date\"} and @qcode{\"time_of_day\"}.\n\
\n\
Based on @code{boost::posix_time::from_time_t()}.\n\
\n\
@example\n\
@group\n\
pt = boost_ptime_from_time_t (1118158776);\n\
@result{} pt.date.year = 2005, pt.date.month = 6, pt.date.day = 7,\n\
        pt.time_of_day.hours = 15, pt.time_of_day.minutes = 39, pt.time_of_day.seconds = 36\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1)
    print_usage ();

  std::time_t t = static_cast<std::time_t> (args(0).double_value ());

  bp::ptime pt = bp::from_time_t (t);

  return octave_value (ptime_to_struct (pt));
}
