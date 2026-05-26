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

DEFUN_DLD (boost_date_ge, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tf} =} boost_date_ge (@var{date1}, @var{date2})\n\
\n\
Compare two dates: is @var{date1} greater than or equal to @var{date2}?\n\
\n\
Each date can be specified as:\n\
@itemize @bullet\n\
@item A struct with fields @qcode{\"year\"}, @qcode{\"month\"}, @qcode{\"day\"}\n\
@item A string in \"YYYY-MM-DD\" format or a special value\n\
  (@qcode{\"neg_infin\"}, @qcode{\"pos_infin\"}, @qcode{\"not_a_date_time\"},\n\
   @qcode{\"max_date_time\"}, @qcode{\"min_date_time\"})\n\
@item Three numeric scalars: @var{year}, @var{month}, @var{day}\n\
@end itemize\n\
\n\
Based on boost::gregorian::date::operator>=.\n\
\n\
Example:\n\
@example\n\
boost_date_ge (2002, 1, 11, 2002, 1, 10)\n\
@result{} 1 (true)\n\
boost_date_ge (2002, 1, 10, 2002, 1, 10)\n\
@result{} 1 (true)\n\
boost_date_ge (2002, 1, 9, 2002, 1, 10)\n\
@result{} 0 (false)\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2)
    print_usage ();

  int n1 = 0;

  if (args(0).isstruct () || args(0).is_string ())
    n1 = 1;
  else
    n1 = 3;

  bg::date d1 = parse_date (args, 0);
  bg::date d2 = parse_date (args, n1);

  return octave_value (d1 >= d2);
}
