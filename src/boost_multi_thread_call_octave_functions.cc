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
static boost::mutex octave_mutex_call_funcs;

// Structure to hold per-thread results
struct CallThreadResultFuncs
{
  std::string thread_id;
  std::string output;
  octave_value_list func_result;
  double retval_status;
  std::string error_msg;
};

void call_function_thread_funcs (const octave_value& func_handle,
                                 const octave_value_list& args_list,
                                 CallThreadResultFuncs& result)
{
  try
    {
      // Get thread ID
      std::ostringstream id_ss;
      id_ss << boost::this_thread::get_id ();
      result.thread_id = id_ss.str ();

      // Lock Octave interpreter for thread safety
      {
        boost::mutex::scoped_lock lock (octave_mutex_call_funcs);

        // Step 1: Store func_handle and args in base workspace via assignin
        {
          octave_value_list assignin_args;
          assignin_args(0) = octave_value ("base");
          assignin_args(1) = octave_value ("__octave_boost_fh_funcs__");
          assignin_args(2) = func_handle;
          BOOST_FEVAL ("assignin", assignin_args);
        }
        {
          octave_value_list assignin_args;
          assignin_args(0) = octave_value ("base");
          assignin_args(1) = octave_value ("__octave_boost_args_funcs__");
          assignin_args(2) = octave_value (Cell (args_list));
          BOOST_FEVAL ("assignin", assignin_args);
        }

        // Step 2: Capture output AND result via a SINGLE evalc call.
        // This ensures output and result come from the same evaluation,
        // avoiding inconsistency with non-deterministic functions like rand().
        octave_value_list evalc_args;
        evalc_args(0) = octave_value ("__octave_boost_fh_funcs__(__octave_boost_args_funcs__{:})");
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

DEFUN_DLD (boost_multi_thread_call_octave_functions, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn  {Loadable Function} {@var{result} =} boost_multi_thread_call_octave_functions (@var{func_handles})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_multi_thread_call_octave_functions (@var{func_handles}, @var{num_threads})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_multi_thread_call_octave_functions (@var{func_handles}, @var{num_threads}, @var{args})\n\
\n\
接收一个函数句柄元胞数组，对数组中的每个元素启动 num_threads 个 boost::thread，并行调用指定的 Octave 函数句柄。\n\
num_threads 默认为 1。\n\
func_handles 为函数句柄元胞数组（如 @@{@@sin, @@cos@}）。\n\
args 默认为空矩阵。args 中的每个元素都必须为元胞，元胞中包含传递给函数的参数。\n\
函数句柄数组和参数数组的尺寸必须相同。\n\
如果函数句柄数组对应的函数需要不带参数调用，则参数数组对应的参数必须写成空元胞。\n\
这个设计是为了参数统一从元胞中获取而来，因此，如果一个函数需要的参数恰好是空元胞，则写成@{@{@}@}。\n\
所有线程的输出将被重定向，格式为 [thread_id] 对应输出。\n\
函数返回一个二维结构体数组，尺寸为 [numel(func_handles), num_threads]，索引方式为 result(func_handles_index, thread_index)。\n\
\n\
基于 boost::thread 和 Octave 的 evalc() 函数。\n\
\n\
示例：\n\
@example\n\
result = boost_multi_thread_call_octave_functions (@{@@rand, @@randi, @@plus@}, 2, @{@{1, 2@}, @{3@}, @{4, 5@}@});\n\
result(1, :)  % @rand 在所有线程上的结果\n\
result(:, 1)  % 所有函数句柄在第一个线程上的结果\n\
@end example\n\
\n\
@seealso{boost_multi_thread_call_octave_function, boost_multi_thread_run_octave_files, boost_multi_thread_eval_octave_expressions, boost_multi_thread_feval_octave_expressions}\n\
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

      // First argument: cell array of function handles
      if (! args(0).iscell ())
        {
          error ("boost_multi_thread_call_octave_functions: FUNC_HANDLES must be a cell array of function handles");
          return octave_value ();
        }
      const Cell func_cell = args(0).cell_value ();
      int num_funcs = func_cell.numel ();

      if (num_funcs < 1)
        error ("boost_multi_thread_call_octave_functions: FUNC_HANDLES must contain at least one function handle");

      // Validate all entries are function handles
      for (int i = 0; i < num_funcs; ++i)
        {
          if (! func_cell(i).is_function_handle ())
            error ("boost_multi_thread_call_octave_functions: element %d of FUNC_HANDLES is not a function handle", i + 1);
        }

      // Second argument (optional): num_threads (default 1)
      int num_threads = 1;
      int arg_offset = 1;  // track where args begin for optional 3rd param

      if (nargin >= 2)
        {
          if (! args(1).is_scalar_type () || ! args(1).isnumeric ())
            error ("boost_multi_thread_call_octave_functions: NUM_THREADS must be a positive integer");
          num_threads = args(1).int_value ();
          if (num_threads < 1)
            error ("boost_multi_thread_call_octave_functions: NUM_THREADS must be a positive integer");
          arg_offset = 2;
        }

      // Third argument (optional): cell array of args (each element is a cell)
      // Default: each function gets empty args
      std::vector<octave_value_list> func_args_list (num_funcs);

      if (nargin >= 3)
        {
          if (! args(2).iscell ())
            {
              error ("boost_multi_thread_call_octave_functions: ARGS must be a cell array");
              return octave_value ();
            }
          const Cell args_cell = args(2).cell_value ();
          int args_len = args_cell.numel ();

          if (args_len != num_funcs)
            error ("boost_multi_thread_call_octave_functions: ARGS length (%d) must match FUNC_HANDLES length (%d)",
                   args_len, num_funcs);

          for (int i = 0; i < num_funcs; ++i)
            {
              if (! args_cell(i).iscell ())
                error ("boost_multi_thread_call_octave_functions: element %d of ARGS must be a cell array", i + 1);
              Cell inner_cell = args_cell(i).cell_value ();
              func_args_list[i] = octave_value_list (inner_cell);
            }
        }

      int total_threads = num_funcs * num_threads;

      // Create thread results (flat vector indexed by i*num_threads + t)
      std::vector<CallThreadResultFuncs> results (total_threads);
      boost::thread_group tg;

      // Launch threads - for each function handle, num_threads threads
      for (int i = 0; i < num_funcs; ++i)
        {
          for (int t = 0; t < num_threads; ++t)
            {
              int idx = i * num_threads + t;
              tg.create_thread (boost::bind (call_function_thread_funcs,
                                             func_cell(i),
                                             func_args_list[i],
                                             boost::ref (results[idx])));
            }
        }

      // Wait for all threads to complete
      tg.join_all ();

      // Build return value: struct array with dimensions [num_funcs, num_threads]
      dim_vector dims (num_funcs, num_threads);
      octave_map ret (dims);

      Cell thread_id_cell (dims);
      Cell output_cell (dims);
      Cell result_cell (dims);
      Cell status_cell (dims);
      Cell error_msg_cell (dims);

      for (int i = 0; i < num_funcs; ++i)
        {
          for (int t = 0; t < num_threads; ++t)
            {
              int idx = i * num_threads + t;
              thread_id_cell(i, t) = octave_value (results[idx].thread_id);
              output_cell(i, t) = octave_value (results[idx].output);
              result_cell(i, t) = octave_value (Cell (results[idx].func_result));
              status_cell(i, t) = octave_value (results[idx].retval_status);
              error_msg_cell(i, t) = octave_value (results[idx].error_msg);
            }
        }

      ret.assign ("thread_id", thread_id_cell);
      ret.assign ("output", output_cell);
      ret.assign ("result", result_cell);
      ret.assign ("status", status_cell);
      ret.assign ("error_msg", error_msg_cell);

      return octave_value (ret);
    }
  catch (const std::exception& e)
    {
      error ("boost_multi_thread_call_octave_functions: %s", e.what ());
      return octave_value ();
    }
}
