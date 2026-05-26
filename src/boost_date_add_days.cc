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

DEFUN_DLD (boost_date_add_days, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{date_out} =} boost_date_add_days (@var{date}, @var{days})\n\
\n\
Add a number of days to a date and return the new date.\n\
\n\
The input @var{date} can be specified as:\n\
@itemize @bullet\n\
@item A struct with fields @qcode{\"year\"}, @qcode{\"month\"}, @qcode{\"day\"}\n\
@item A string in \"YYYY-MM-DD\" format or a special value\n\
  (@qcode{\"neg_infin\"}, @qcode{\"pos_infin\"}, @qcode{\"not_a_date_time\"},\n\
   @qcode{\"max_date_time\"}, @qcode{\"min_date_time\"})\n\
@item Three numeric scalars: @var{year}, @var{month}, @var{day}\n\
@end itemize\n\
The @var{days} parameter is an integer number of days to add.\n\
\n\
Returns a struct with fields @qcode{\"year\"}, @qcode{\"month\"}, @qcode{\"day\"}.\n\
\n\
Based on boost::gregorian::date + date_duration.\n\
\n\
Example:\n\
@example\n\
boost_date_add_days (2002, 1, 1, 10)\n\
@result{} struct with year=2002, month=1, day=11\n\
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

  bg::date d = parse_date (args, 0);

  // Get the number of days to add
  long ndays = args(n1).xlong_value ("boost_date_add_days: DAYS must be an integer");

  bg::date result = d + bg::days (ndays);

  return octave_value (date_to_struct (result));
}
