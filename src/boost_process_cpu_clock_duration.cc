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

DEFUN_DLD (boost_process_cpu_clock_duration, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{dur} =} boost_process_cpu_clock_duration ()\n\
\n\
Measure the elapsed process CPU time between entering and exiting this\n\
function using boost::chrono::process_cpu_clock.  The function enters an\n\
infinite loop; press Ctrl+C to exit.  The elapsed time is returned as a\n\
struct with fields @qcode{\"real\"} (wall clock nanoseconds),\n\
@qcode{\"user\"} (user CPU nanoseconds),\n\
@qcode{\"system\"} (kernel CPU nanoseconds), and @qcode{\"total\"}\n\
(total CPU nanoseconds).\n\
\n\
Based on boost::chrono::process_cpu_clock::now().\n\
\n\
@example\n\
@group\n\
dur = boost_process_cpu_clock_duration ();\n\
@result{} dur.real (wall clock nanoseconds)\n\
@result{} dur.user (user CPU nanoseconds)\n\
@result{} dur.system (kernel CPU nanoseconds)\n\
@result{} dur.total (total CPU nanoseconds)\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 0)
    print_usage ();

  auto start = boost::chrono::process_cpu_clock::now ();

  octave_stdout << "Press Ctrl+C to stop..." << std::endl;

  try
    {
      while (true)
        {
          octave_quit ();
        }
    }
  catch (const std::exception&)
    {
      // Ctrl+C was pressed; measure elapsed time
    }

  auto end = boost::chrono::process_cpu_clock::now ();
  auto elapsed = end - start;
  auto rep = elapsed.count ();  // process_times<long>

  octave_scalar_map ret;
  ret.setfield ("real",   octave_value (static_cast<double> (rep.real)));
  ret.setfield ("user",   octave_value (static_cast<double> (rep.user)));
  ret.setfield ("system", octave_value (static_cast<double> (rep.system)));
  ret.setfield ("total",  octave_value (static_cast<double> (rep.user + rep.system)));

  return octave_value (ret);
}
