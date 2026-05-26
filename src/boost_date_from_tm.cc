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
#include <ctime>

DEFUN_DLD (boost_date_from_tm, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{date} =} boost_date_from_tm (@var{tm_struct})\n\
\n\
Convert a @code{tm} struct back to a Boost.Gregorian date.\n\
\n\
The input @var{tm_struct} must be a struct with at least these fields:\n\
@itemize @bullet\n\
@item @qcode{\"tm_year\"} - years since 1900\n\
@item @qcode{\"tm_mon\"} - month (0-11, 0=January)\n\
@item @qcode{\"tm_mday\"} - day of month (1-31)\n\
@end itemize\n\
\n\
Returns a struct with fields @qcode{\"year\"}, @qcode{\"month\"}, @qcode{\"day\"}.\n\
\n\
Based on boost::gregorian::date_from_tm().\n\
\n\
Example:\n\
@example\n\
tm.tm_year = 105;\n\
tm.tm_mon = 0;\n\
tm.tm_mday = 1;\n\
d = boost_date_from_tm (tm);\n\
@result{} struct with year=2005, month=1, day=1\n\
@end example\n\
@end deftypefn")
{
  if (args.length () < 1)
    print_usage ();

  if (! args(0).isstruct ())
    error ("boost_date_from_tm: argument must be a struct with fields 'tm_year', 'tm_mon', 'tm_mday'");

  octave_scalar_map m = args(0).scalar_map_value ();

  if (! m.isfield ("tm_year") || ! m.isfield ("tm_mon") || ! m.isfield ("tm_mday"))
    error ("boost_date_from_tm: struct must have fields 'tm_year', 'tm_mon', 'tm_mday'");

  std::tm d_tm;
  d_tm.tm_year = static_cast<int> (m.getfield ("tm_year").double_value ());
  d_tm.tm_mon  = static_cast<int> (m.getfield ("tm_mon").double_value ());
  d_tm.tm_mday = static_cast<int> (m.getfield ("tm_mday").double_value ());
  // Default remaining fields
  d_tm.tm_wday = 0;
  d_tm.tm_yday = 0;
  d_tm.tm_hour = 0;
  d_tm.tm_min  = 0;
  d_tm.tm_sec  = 0;
  d_tm.tm_isdst = -1;

  bg::date d = bg::date_from_tm (d_tm);

  return octave_value (date_to_struct (d));
}
