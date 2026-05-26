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

DEFUN_DLD (boost_date_span_to_simple_string, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{str} =} boost_date_span_to_simple_string (@var{period})\n\
\n\
Convert a date_period to a [YYYY-mmm-DD/YYYY-mmm-DD] string where mmm is the 3-character month name.\n\
\n\
The input @var{period} can be specified as:\n\
@itemize @bullet\n\
@item A struct with fields @qcode{\"begin\"} (date struct) and @qcode{\"end\"} (date struct)\n\
  or @qcode{\"begin\"} and @qcode{\"length\"}\n\
@item Two dates (each as struct, string, or three scalars)\n\
@item A date (struct/string/three scalars) and an integer days count\n\
@end itemize\n\
\n\
Returns a string in the format [YYYY-mmm-DD/YYYY-mmm-DD].\n\
\n\
Based on boost::gregorian::to_simple_string(date_period).\n\
\n\
Example:\n\
@example\n\
str = boost_date_span_to_simple_string (2002, 1, 1, 2002, 1, 31)\n\
@result{} \"[2002-Jan-01/2002-Jan-31]\"\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1)
    print_usage ();

  auto res = parse_date_period (args, 0);
  bg::date_period dp = res.period;

  return octave_value (bg::to_simple_string (dp));
}
