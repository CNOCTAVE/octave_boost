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

DEFUN_DLD (boost_date_is_special, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tf} =} boost_date_is_special (@var{date})\n\
\n\
Return true if the given Boost.Gregorian date is a special value\n\
(pos_infin, neg_infin, or not_a_date_time).\n\
\n\
The input @var{date} can be specified in any of the following forms:\n\
@itemize\n\
@item A struct with fields @qcode{\"year\"}, @qcode{\"month\"}, @qcode{\"day\"}\n\
@item A string in the format @qcode{\"YYYY-MM-DD\"}\n\
@item A special value string: @qcode{\"neg_infin\"}, @qcode{\"pos_infin\"}, @qcode{\"not_a_date_time\"}\n\
@item Three numeric arguments: @var{year}, @var{month}, @var{day}\n\
@end itemize\n\
\n\
Based on @code{boost::gregorian::date::is_special()}.\n\
\n\
@example\n\
@group\n\
tf = boost_date_is_special ('pos_infin');\n\
@result{} tf = 1 (true)\n\
\n\
tf = boost_date_is_special ('not_a_date_time');\n\
@result{} tf = 1 (true)\n\
\n\
tf = boost_date_is_special (2005, 3, 1);\n\
@result{} tf = 0 (false)\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1 || args.length () > 3)
    print_usage ();

  bg::date d = parse_date (args, 0);

  return octave_value (d.is_special ());
}
