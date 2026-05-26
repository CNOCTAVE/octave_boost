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

DEFUN_DLD (boost_time_duration_ticks_per_second, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{ticks} =} boost_time_duration_ticks_per_second ()\n\
\n\
Return the number of ticks in a second. For example, if the duration supports nanoseconds\n\
then the returned result will be 1,000,000,000 (1e+9).\n\
\n\
This is a static member of @code{boost::posix_time::time_duration}.\n\
\n\
Based on @code{boost::posix_time::time_duration::ticks_per_second()}.\n\
\n\
@example\n\
@group\n\
boost_time_duration_ticks_per_second ()\n\
@result{} 1000000000\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 0)
    print_usage ();

  return octave_value (static_cast<double> (bp::time_duration::ticks_per_second ()));
}
