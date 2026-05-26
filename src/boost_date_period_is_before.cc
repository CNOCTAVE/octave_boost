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

DEFUN_DLD (boost_date_period_is_before, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tf} =} boost_date_period_is_before (@var{period}, @var{date})\n\
\n\
Determine if the date_period is before a given date.\n\
\n\
The input @var{period} can be specified as:\n\
@itemize @bullet\n\
@item A struct with fields @qcode{\"begin\"} (date struct) and @qcode{\"end\"} (date struct)\n\
  or @qcode{\"begin\"} and @qcode{\"length\"}\n\
@item Two dates (each as struct, string, or three scalars)\n\
@item A date (struct/string/three scalars) and an integer days count\n\
@end itemize\n\
\n\
The @var{date} argument can be a struct (with year, month, day fields), a string (\"YYYY-MM-DD\",\n\
\"neg_infin\", \"pos_infin\", \"not_a_date_time\", \"max_date_time\", \"min_date_time\"),\n\
or three numeric scalars (year, month, day).\n\
\n\
Returns a logical value (1 if period is before the date, 0 otherwise).\n\
\n\
Based on boost::gregorian::date_period::is_before().\n\
\n\
Example:\n\
@example\n\
tf = boost_date_period_is_before (2002, 1, 1, 2002, 1, 3, 2002, 1, 10)\n\
@result{} 1 (true)\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2)
    print_usage ();

  auto res = parse_date_period (args, 0);
  bg::date_period dp = res.period;
  int consumed = res.consumed;

  if (consumed >= args.length ())
    error ("boost_date_period_is_before: missing date argument");

  bg::date d = parse_date (args, consumed);

  return octave_value (dp.is_before (d));
}
