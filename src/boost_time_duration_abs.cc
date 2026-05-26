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

// Helper to convert time_duration to Octave struct
static inline octave_scalar_map
time_duration_abs_to_struct (const bp::time_duration& td)
{
  octave_scalar_map ret;

  if (td.is_special ())
    {
      if (td.is_neg_infinity ())
        ret.setfield ("value", octave_value ("neg_infin"));
      else if (td.is_pos_infinity ())
        ret.setfield ("value", octave_value ("pos_infin"));
      else
        ret.setfield ("value", octave_value ("not_a_date_time"));
      return ret;
    }

  ret.setfield ("hours", octave_value (static_cast<double> (td.hours ())));
  ret.setfield ("minutes", octave_value (static_cast<double> (td.minutes ())));
  ret.setfield ("seconds", octave_value (static_cast<double> (td.seconds ())));
  ret.setfield ("fractional_seconds", octave_value (static_cast<double> (td.fractional_seconds ())));
  return ret;
}

DEFUN_DLD (boost_time_duration_abs, args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{td_out} =} boost_time_duration_abs (@var{td})\n\
\n\
Generate a new time duration with the absolute value of the input time duration.\n\
\n\
The input @var{td} can be specified as:\n\
@itemize\n\
@item A struct with fields @qcode{\"hours\"}, @qcode{\"minutes\"}, @qcode{\"seconds\"}, and optionally @qcode{\"fractional_seconds\"}\n\
@item A string in @qcode{\"HH:MM:SS.fff\"} format\n\
@item 3 or 4 numeric arguments: @var{hours}, @var{minutes}, @var{seconds}[, @var{fractional_seconds}]\n\
@end itemize\n\
\n\
Returns a struct with fields @qcode{\"hours\"}, @qcode{\"minutes\"}, @qcode{\"seconds\"}, @qcode{\"fractional_seconds\"}.\n\
\n\
Based on @code{boost::posix_time::time_duration::abs()}.\n\
\n\
@example\n\
@group\n\
boost_time_duration_abs (-1, 0, 0)\n\
@result{} struct with hours=1, minutes=0, seconds=0, fractional_seconds=0\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1)
    print_usage ();

  bp::time_duration td = parse_time_duration (args, 0);
  bp::time_duration result = td.abs ();

  return octave_value (time_duration_abs_to_struct (result));
}
