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

DEFUN_DLD (boost_chrono_steady_clock_duration, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{dur} =} boost_chrono_steady_clock_duration ()\n\
\n\
Measure the elapsed monotonic time between entering and exiting this function\n\
using boost::chrono::steady_clock.  The steady clock is monotonic and not\n\
affected by system time adjustments.  The function enters an infinite loop;\n\
press Ctrl+C to exit.  The elapsed time is returned as a struct with\n\
fields @qcode{\"count\"} (nanoseconds) and @qcode{\"unit\"}.\n\
\n\
Based on boost::chrono::steady_clock::now().\n\
\n\
@example\n\
@group\n\
dur = boost_chrono_steady_clock_duration ();\n\
@result{} dur.count (elapsed nanoseconds)\n\
@result{} dur.unit = 'nanoseconds'\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 0)
    print_usage ();

  auto start = boost::chrono::steady_clock::now ();

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

  auto end = boost::chrono::steady_clock::now ();
  auto elapsed = end - start;
  auto ns = boost::chrono::duration_cast<boost::chrono::nanoseconds> (elapsed);

  octave_scalar_map ret;
  ret.setfield ("count", octave_value (static_cast<double> (ns.count ())));
  ret.setfield ("unit", octave_value ("nanoseconds"));

  return octave_value (ret);
}
