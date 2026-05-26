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

DEFUN_DLD (boost_date_period_expand, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{period_out} =} boost_date_period_expand (@var{period}, @var{days})\n\
\n\
Subtract duration from begin and add duration to end of a date_period, returning a new period.\n\
\n\
The input @var{period} can be specified as:\n\
@itemize @bullet\n\
@item A struct with fields @qcode{\"begin\"} (date struct) and @qcode{\"end\"} (date struct)\n\
  or @qcode{\"begin\"} and @qcode{\"length\"}\n\
@item Two dates (each as struct, string, or three scalars)\n\
@item A date (struct/string/three scalars) and an integer days count\n\
@end itemize\n\
The @var{days} parameter is an integer number of days to expand.\n\
\n\
Returns a struct with fields @qcode{\"begin\"}, @qcode{\"last\"}, @qcode{\"end\"}, @qcode{\"length\"}.\n\
\n\
Based on boost::gregorian::date_period::expand().\n\
\n\
Example:\n\
@example\n\
dp = boost_date_period_expand (2005, 1, 2, 2, 1)\n\
@result{} period: 2005-Jan-01 to 2005-Jan-04\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 2)
    print_usage ();

  auto res = parse_date_period (args, 0);
  bg::date_period dp = res.period;
  int consumed = res.consumed;

  if (consumed >= args.length ())
    error ("boost_date_period_expand: missing days argument");

  long ndays = args (consumed).long_value ();

  dp.expand (bg::days (ndays));

  return octave_value (date_period_to_struct (dp));
}
