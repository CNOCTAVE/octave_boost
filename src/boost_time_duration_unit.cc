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

static inline octave_scalar_map
time_duration_unit_to_struct (const bp::time_duration& td)
{
  octave_scalar_map ret;
  ret.setfield ("hours", octave_value (static_cast<double> (td.hours ())));
  ret.setfield ("minutes", octave_value (static_cast<double> (td.minutes ())));
  ret.setfield ("seconds", octave_value (static_cast<double> (td.seconds ())));
  ret.setfield ("fractional_seconds", octave_value (static_cast<double> (td.fractional_seconds ())));
  return ret;
}

DEFUN_DLD (boost_time_duration_unit, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{td_unit} =} boost_time_duration_unit ()\n\
\n\
Return the smallest possible unit of duration type, typically a time_duration of 1 nanosecond.\n\
\n\
This is a static member of @code{boost::posix_time::time_duration}.\n\
\n\
Returns a struct with fields @qcode{\"hours\"}, @qcode{\"minutes\"}, @qcode{\"seconds\"}, @qcode{\"fractional_seconds\"}.\n\
\n\
Based on @code{boost::posix_time::time_duration::unit()}.\n\
\n\
@example\n\
@group\n\
boost_time_duration_unit ()\n\
@result{} struct with hours=0, minutes=0, seconds=0, fractional_seconds=1\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 0)
    print_usage ();

  bp::time_duration unit = bp::time_duration::unit ();

  return octave_value (time_duration_unit_to_struct (unit));
}
