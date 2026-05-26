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

DEFUN_DLD (boost_date_gregorian_is_leap_year, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tf} =} boost_date_gregorian_is_leap_year (@var{year})\n\
\n\
Return true if the specified year is a leap year in the Gregorian calendar.\n\
\n\
Based on @code{boost::gregorian::gregorian_calendar::is_leap_year()}.\n\
\n\
@example\n\
@group\n\
tf = boost_date_gregorian_is_leap_year (2000);\n\
@result{} tf = 1 (true)\n\
\n\
tf = boost_date_gregorian_is_leap_year (2001);\n\
@result{} tf = 0 (false)\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 1)
    print_usage ();

  double y = args (0).double_value ();

  bool is_leap = bg::gregorian_calendar::is_leap_year (
    static_cast<bg::gregorian_calendar::year_type> (y));

  return octave_value (is_leap);
}
