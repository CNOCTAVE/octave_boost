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

DEFUN_DLD (boost_time_duration_resolution, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{resolution} =} boost_time_duration_resolution ()\n\
\n\
Return the resolution capability of the time_duration class as a string.\n\
Possible values are @qcode{\"nano\"}, @qcode{\"micro\"}, @qcode{\"milli\"}, etc.\n\
\n\
This is a static member of @code{boost::posix_time::time_duration}.\n\
\n\
Based on @code{boost::posix_time::time_duration::resolution()}.\n\
\n\
@example\n\
@group\n\
boost_time_duration_resolution ()\n\
@result{} \"nano\"\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 0)
    print_usage ();

  // resolution() is a static method of the time_resolution_traits class,
  // accessible through time_duration's rep_type typedef.
  // It returns a boost::date_time::time_resolutions enum value.
  boost::date_time::time_resolutions res
    = bp::time_duration::rep_type::resolution ();
  std::string res_str;

  switch (res)
    {
    case boost::date_time::sec:
      res_str = "sec";
      break;
    case boost::date_time::tenth:
      res_str = "tenth";
      break;
    case boost::date_time::hundredth:
      res_str = "hundredth";
      break;
    case boost::date_time::milli:
      res_str = "milli";
      break;
    case boost::date_time::ten_thousandth:
      res_str = "ten_thousandth";
      break;
    case boost::date_time::micro:
      res_str = "micro";
      break;
    case boost::date_time::nano:
      res_str = "nano";
      break;
    default:
      res_str = "unknown";
      break;
    }

  return octave_value (res_str);
}
