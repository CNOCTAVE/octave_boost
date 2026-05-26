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

#include "boost_chrono_common.h"

DEFUN_DLD (boost_process_cpu_clock_now, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{time} =} boost_process_cpu_clock_now ()\n\
\n\
Return the current process CPU clock time as a struct with fields\n\
@qcode{\"real\"} (wall clock), @qcode{\"user\"}, @qcode{\"system\"}, and\n\
@qcode{\"total\"} (= user + system), each in nanoseconds.\n\
The process CPU clock measures CPU time consumed by the current process.\n\
Based on boost::chrono::process_cpu_clock::now().\n\
\n\
@example\n\
@group\n\
t = boost_process_cpu_clock_now ();\n\
@result{} t.real (wall clock time in nanoseconds)\n\
@result{} t.user (user CPU time in nanoseconds)\n\
@result{} t.system (system CPU time in nanoseconds)\n\
@result{} t.total (total CPU time in nanoseconds)\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 0)
    print_usage ();

  auto tp = boost::chrono::process_cpu_clock::now ();
  auto duration = tp.time_since_epoch ();
  auto rep = duration.count ();

  octave_scalar_map ret;
  // process_times members: real (wall), user, system — all are plain Rep (long int)
  ret.setfield ("real",   octave_value (static_cast<double> (rep.real)));
  ret.setfield ("user",   octave_value (static_cast<double> (rep.user)));
  ret.setfield ("system", octave_value (static_cast<double> (rep.system)));
  ret.setfield ("total",  octave_value (static_cast<double> (rep.user + rep.system)));

  return octave_value (ret);
}
