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

DEFUN_DLD (boost_date_period_length, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{len} =} boost_date_period_length (@var{period})\n\
\n\
Return the length (number of days) of a date_period.\n\
\n\
The input @var{period} can be specified as:\n\
@itemize @bullet\n\
@item A struct with fields @qcode{\"begin\"} (date struct) and @qcode{\"end\"} (date struct)\n\
  or @qcode{\"begin\"} and @qcode{\"length\"}\n\
@item Two dates (each as struct, string, or three scalars)\n\
@item A date (struct/string/three scalars) and an integer days count\n\
@end itemize\n\
\n\
Returns a scalar integer (number of days).\n\
\n\
Based on boost::gregorian::date_period::length().\n\
\n\
Example:\n\
@example\n\
len = boost_date_period_length (2002, 1, 1, 2)\n\
@result{} 2\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1)
    print_usage ();

  auto res = parse_date_period (args, 0);
  bg::date_period dp = res.period;

  return octave_value (static_cast<double> (dp.length ().days ()));
}
