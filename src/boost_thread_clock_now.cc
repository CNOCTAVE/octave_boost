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

DEFUN_DLD (boost_thread_clock_now, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{time} =} boost_thread_clock_now ()\n\
\n\
Return the current thread CPU clock time as a struct with fields\n\
@qcode{\"count\"} and @qcode{\"unit\"}. The thread clock measures CPU time\n\
consumed by the current thread. The count is in nanoseconds.\n\
Based on boost::chrono::thread_clock::now().\n\
\n\
@noindent Note: if the platform does not support thread clock, an error\n\
will be raised.\n\
\n\
@example\n\
@group\n\
t = boost_thread_clock_now ();\n\
@result{} t.count (nanoseconds since thread clock epoch)\n\
@result{} t.unit = 'nanoseconds'\n\
@end group\n\
@end example\n\
@end deftypefn")
{
  if (args.length () != 0)
    print_usage ();

  try
    {
      auto tp = boost::chrono::thread_clock::now ();
      auto duration = tp.time_since_epoch ();
      auto ns = boost::chrono::duration_cast<boost::chrono::nanoseconds> (duration);

      octave_scalar_map ret;
      ret.setfield ("count", octave_value (static_cast<double> (ns.count ())));
      ret.setfield ("unit", octave_value ("nanoseconds"));

      return octave_value (ret);
    }
  catch (const std::exception& e)
    {
      error ("boost_thread_clock_now: thread clock not supported on this platform: %s",
             e.what ());
      return octave_value ();
    }
}
