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

#include <octave/oct.h>
#include <octave/parse.h>
#include "config.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <string>
#include <sstream>
#include <vector>

// Use octave::feval (Octave 4.0+ namespace)
#define BOOST_FEVAL octave::feval

// Shared mutex for serializing access to Octave interpreter
static boost::mutex octave_mutex;

// Structure to hold per-thread results
struct CallThreadResult
{
  std::string thread_id;
  std::string output;
  octave_value_list func_result;
  double retval_status;
  std::string error_msg;
};

void call_function_thread (const octave_value& func_handle,
                           const octave_value_list& args_list,
                           CallThreadResult& result)
{
  try
    {
      // Get thread ID
      std::ostringstream id_ss;
      id_ss << boost::this_thread::get_id ();
      result.thread_id = id_ss.str ();

      // Lock Octave interpreter for thread safety
      {
        boost::mutex::scoped_lock lock (octave_mutex);

        // Step 1: Store func_handle and args in base workspace via assignin
        {
          octave_value_list assignin_args;
          assignin_args(0) = octave_value ("base");
          assignin_args(1) = octave_value ("__octave_boost_fh__");
          assignin_args(2) = func_handle;
          BOOST_FEVAL ("assignin", assignin_args);
        }
        {
          octave_value_list assignin_args;
          assignin_args(0) = octave_value ("base");
          assignin_args(1) = octave_value ("__octave_boost_args__");
          assignin_args(2) = octave_value (Cell (args_list));
          BOOST_FEVAL ("assignin", assignin_args);
        }

        // Step 2: Capture output AND result via a SINGLE evalc call.
        // This ensures output and result come from the same evaluation,
        // avoiding inconsistency with non-deterministic functions like rand().
        octave_value_list evalc_args;
        evalc_args(0) = octave_value ("__octave_boost_fh__(__octave_boost_args__{:})");
        octave_value_list evalc_result = BOOST_FEVAL ("evalc", evalc_args, 2);

        if (evalc_result.length () > 0)
          result.output = evalc_result(0).string_value ();

        // Get the actual return value from the same evalc call (no second feval)
        if (evalc_result.length () > 1)
          {
            octave_value_list tmp;
            tmp(0) = evalc_result(1);
            result.func_result = tmp;
          }

        result.retval_status = 0.0;
      }
    }
  catch (const std::exception& e)
    {
      result.error_msg = e.what ();
      result.retval_status = -1.0;
    }
}

DEFUN_DLD (boost_multi_thread_call_octave_function, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn  {Loadable Function} {@var{result} =} boost_multi_thread_call_octave_function (@var{func_handle})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_multi_thread_call_octave_function (@var{func_handle}, @var{num_threads})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_multi_thread_call_octave_function (@var{func_handle}, @var{num_threads}, @var{args})\n\
\n\
启动多个 boost::thread，并行调用指定的 Octave 函数句柄。\n\
func_handle 为函数句柄（如 @sin）。\n\
所有线程的输出将被重定向，格式为 [thread_id] 对应输出。\n\
函数返回一个结构体数组，包含每个线程的 thread_id、output、result 和 status。\n\
\n\
基于 boost::thread 和 Octave 的 feval() 函数。\n\
\n\
示例：\n\
@example\n\
result = boost_multi_thread_call_octave_function (@@rand, 4, {5, 5});\n\
@end example\n\
\n\
@seealso{boost_multi_thread_run_octave_file, boost_multi_thread_eval_octave_expression, boost_multi_thread_feval_octave_expression}\n\
@end deftypefn")
{
  try
    {
      int nargin = args.length ();

      if (nargin < 1 || nargin > 3)
        {
          print_usage ();
          return octave_value ();
        }

      // First argument: function handle (not string)
      if (! args(0).is_function_handle ())
        {
          error ("boost_multi_thread_call_octave_function: FUNC_HANDLE must be a function handle (e.g. @sin)");
          return octave_value ();
        }
      octave_value func_handle = args(0);

      // Second argument (optional): num_threads (default 1)
      int num_threads = 1;
      if (nargin >= 2)
        {
          if (! args(1).is_scalar_type () || ! args(1).isnumeric ())
            error ("boost_multi_thread_call_octave_function: NUM_THREADS must be a positive integer");
          num_threads = args(1).int_value ();
          if (num_threads < 1)
            error ("boost_multi_thread_call_octave_function: NUM_THREADS must be a positive integer");
        }

      // Third argument (optional): function arguments (cell array, default empty)
      octave_value_list func_args;
      if (nargin >= 3)
        {
          if (! args(2).iscell ())
            {
              error ("boost_multi_thread_call_octave_function: ARGS must be a cell array");
              return octave_value ();
            }
          const Cell arg_cell = args(2).cell_value ();
          func_args = octave_value_list (arg_cell);
        }

      // Create thread results
      std::vector<CallThreadResult> results (num_threads);
      boost::thread_group tg;

      // Launch threads
      for (int i = 0; i < num_threads; ++i)
        {
          tg.create_thread (boost::bind (call_function_thread,
                                         func_handle,
                                         func_args,
                                         boost::ref (results[i])));
        }

      // Wait for all threads to complete
      tg.join_all ();

      // Build return value
      octave_map ret;
      Cell thread_id_cell (dim_vector (num_threads, 1));
      Cell output_cell (dim_vector (num_threads, 1));
      Cell result_cell (dim_vector (num_threads, 1));
      Cell status_cell (dim_vector (num_threads, 1));

      for (int i = 0; i < num_threads; ++i)
        {
          thread_id_cell(i) = octave_value (results[i].thread_id);
          output_cell(i) = octave_value (results[i].output);
          result_cell(i) = octave_value (Cell (results[i].func_result));
          status_cell(i) = octave_value (results[i].retval_status);
        }

      ret.assign ("thread_id", thread_id_cell);
      ret.assign ("output", output_cell);
      ret.assign ("result", result_cell);
      ret.assign ("status", status_cell);

      return octave_value (ret);
    }
  catch (const std::exception& e)
    {
      error ("boost_multi_thread_call_octave_function: %s", e.what ());
      return octave_value ();
    }
}
