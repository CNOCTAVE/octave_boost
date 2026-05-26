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

DEFUN_DLD (boost_date_period_intersection, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{period_out} =} boost_date_period_intersection (@var{period1}, @var{period2})\n\
\n\
Calculate the intersection of two date_periods. Returns null if there is no intersection.\n\
\n\
Each @var{period} can be specified as:\n\
@itemize @bullet\n\
@item A struct with fields @qcode{\"begin\"} (date struct) and @qcode{\"end\"} (date struct)\n\
  or @qcode{\"begin\"} and @qcode{\"length\"}\n\
@item Two dates (each as struct, string, or three scalars)\n\
@item A date (struct/string/three scalars) and an integer days count\n\
@end itemize\n\
\n\
Returns a struct with fields @qcode{\"begin\"}, @qcode{\"last\"}, @qcode{\"end\"}, @qcode{\"length\"}.\n\
If the intersection is null, returns a struct with @qcode{\"length\"} = 0.\n\
\n\
Based on boost::gregorian::date_period::intersection().\n\
\n\
Example:\n\
@example\n\
dp = boost_date_period_intersection (2002, 1, 1, 2002, 1, 10, 2002, 1, 2, 2002, 1, 3)\n\
@result{} period: 2002-Jan-02 to 2002-Jan-03\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2)
    print_usage ();

  auto res1 = parse_date_period (args, 0);
  bg::date_period dp1 = res1.period;
  int consumed1 = res1.consumed;

  if (consumed1 >= args.length ())
    error ("boost_date_period_intersection: insufficient arguments for second period");

  auto res2 = parse_date_period (args, consumed1);
  bg::date_period dp2 = res2.period;

  bg::date_period result = dp1.intersection (dp2);

  return octave_value (date_period_to_struct (result));
}
