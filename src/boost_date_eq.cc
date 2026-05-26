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

DEFUN_DLD (boost_date_eq, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tf} =} boost_date_eq (@var{date1}, @var{date2})\n\
\n\
Compare two Boost.Gregorian dates for equality.\n\
\n\
Each input @var{date} can be specified in any of the following forms:\n\
@itemize\n\
@item A struct with fields @qcode{\"year\"}, @qcode{\"month\"}, @qcode{\"day\"}\n\
@item A string in the format @qcode{\"YYYY-MM-DD\"}\n\
@item A special value string: @qcode{\"neg_infin\"}, @qcode{\"pos_infin\"}, @qcode{\"not_a_date_time\"}\n\
@item Three numeric arguments: @var{year}, @var{month}, @var{day}\n\
@end itemize\n\
\n\
Based on @code{boost::gregorian::date::operator==}.\n\
\n\
@example\n\
@group\n\
tf = boost_date_eq (2002, 1, 10, 2002, 1, 10);\n\
@result{} tf = 1 (true)\n\
\n\
tf = boost_date_eq (2002, 1, 10, 2002, 1, 11);\n\
@result{} tf = 0 (false)\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2 || args.length () > 6)
    print_usage ();

  // If we have 2 args, each is a date (struct, string, or single scalar special)
  // If we have 6 args, it's two sets of (year, month, day)
  // If we have 4 args, it's potentially two structs/strings or one (year,month,day) + one struct/string
  // We need to split argument count properly.

  // Determine how many args go to each date
  int n1, n2;

  if (args.length () == 2)
    {
      // Two dates, each as struct or string
      n1 = 1;
      n2 = 1;
    }
  else if (args.length () == 3)
    {
      // First date as (year,month,day) = 3 args, second as struct/string = 0 offset for second
      // OR first as struct/string, second as 2 args? That doesn't make sense.
      // Actually for 3 args, it could be (d1_struct, y, m) or (y, m, d) = d1, d2 not possible cleanly.
      // Let's handle: first date 1 arg (struct/string), second date 2 args? No, date needs 1 or 3 args.
      // So 3 args means: first is struct/string, second needs 2 more? That doesn't work.
      // Actually with 3 args total: first date = 1 arg, second date = 2 args doesn't make sense for date.
      // So reject.
      error ("boost_date_eq: invalid number of arguments. "
             "Usage: boost_date_eq(d1, d2) with each date as struct, string, "
             "or three numeric values (year,month,day) each");
    }
  else if (args.length () == 4)
    {
      // First date 1 arg (struct/string), second date 3 args (year,month,day)
      n1 = 1;
      n2 = 3;
    }
  else if (args.length () == 5)
    {
      // first date 3 args (year,month,day), second date 2 args? No.
      // first date 2 args, second date 3 args? No, date needs 1 or 3.
      // Could be first date 1+1+3 = 5. But that's ambiguous.
      error ("boost_date_eq: invalid number of arguments. "
             "Usage: boost_date_eq(d1, d2) with each date as struct, string, "
             "or three numeric values (year,month,day) each");
    }
  else // 6 args
    {
      // Two dates, each as (year,month,day)
      n1 = 3;
      n2 = 3;
    }

  bg::date d1 = parse_date (args, 0);
  bg::date d2 = parse_date (args, n1);

  return octave_value (d1 == d2);
}
