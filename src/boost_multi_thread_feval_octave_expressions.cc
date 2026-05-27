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
static boost::mutex octave_mutex_feval_exprs;

// Structure to hold per-thread results
struct FevalThreadResultExprs
{
  std::string thread_id;
  std::string output;
  octave_value_list func_result;
  double retval_status;
  std::string error_msg;
};

void feval_expression_thread_exprs (const std::string& funcname,
                                    const octave_value_list& args_list,
                                    FevalThreadResultExprs& result)
{
  try
    {
      // Get thread ID
      std::ostringstream id_ss;
      id_ss << boost::this_thread::get_id ();
      result.thread_id = id_ss.str ();

      // Lock Octave interpreter for thread safety
      {
        boost::mutex::scoped_lock lock (octave_mutex_feval_exprs);

        // Step 1: Store arguments in base workspace via assignin
        octave_value_list assignin_args;
        assignin_args(0) = octave_value ("base");
        assignin_args(1) = octave_value ("__octave_boost_feval_args_exprs__");
        assignin_args(2) = octave_value (Cell (args_list));
        BOOST_FEVAL ("assignin", assignin_args);

        // Step 2: Use a SINGLE evalc call with 2 outputs to capture
        // both output AND result from the same evaluation.
        // This ensures consistency for non-deterministic functions like rand().
        std::string feval_expr = funcname + "(__octave_boost_feval_args_exprs__{:})";
        octave_value_list evalc_args;
        evalc_args(0) = octave_value (feval_expr);
        octave_value_list evalc_out = BOOST_FEVAL ("evalc", evalc_args, 2);

        if (evalc_out.length () > 0)
          result.output = evalc_out(0).string_value ();

        // Get the actual return value from the same evalc call (no second feval)
        if (evalc_out.length () > 1)
          {
            octave_value_list tmp;
            tmp(0) = evalc_out(1);
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

DEFUN_DLD (boost_multi_thread_feval_octave_expressions, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn  {Loadable Function} {@var{result} =} boost_multi_thread_feval_octave_expressions (@var{funcnames})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_multi_thread_feval_octave_expressions (@var{funcnames}, @var{num_threads})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_multi_thread_feval_octave_expressions (@var{funcnames}, @var{num_threads}, @var{args})\n\
\n\
接收一个函数名称字符串元胞数组，对数组中的每个元素启动 num_threads 个 boost::thread，并行 feval() 指定的 Octave 函数。\n\
num_threads 默认为 1。\n\
funcnames 为函数名称字符串的元胞数组。\n\
args 默认为空矩阵。args 中的每个元素都必须为元胞，元胞中包含传递给 feval 的参数。\n\
函数名称数组和参数数组的尺寸必须相同。\n\
如果函数名称数组对应的函数需要不带参数调用，则参数数组对应的参数必须写成空元胞。\n\
这个设计是为了参数统一从元胞中获取而来，因此，如果一个函数需要的参数恰好是空元胞，则写成@{@{@}@}。\n\
所有线程的输出将被重定向，格式为 [thread_id] 对应输出。\n\
函数返回一个二维结构体数组，尺寸为 [numel(funcnames), num_threads]，索引方式为 result(funcnames_index, thread_index)。\n\
\n\
基于 boost::thread 和 Octave 的 evalc() 函数。\n\
\n\
示例：\n\
@example\n\
result = boost_multi_thread_feval_octave_expressions (@{\"rand\", \"sin\"@}, 2, @{@{3, 3@}, @{pi/2@}@});\n\
result(1, :)  % \"rand\" 在所有线程上的结果\n\
result(:, 1)  % 所有函数在第一个线程上的结果\n\
@end example\n\
\n\
@seealso{boost_multi_thread_feval_octave_expression, boost_multi_thread_run_octave_files, boost_multi_thread_call_octave_functions, boost_multi_thread_eval_octave_expressions}\n\
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

      // First argument: cell array of function names (strings)
      if (! args(0).iscell ())
        error ("boost_multi_thread_feval_octave_expressions: FUNCNAMES must be a cell array of strings");

      const Cell funcname_cell = args(0).cell_value ();
      int num_funcs = funcname_cell.numel ();

      if (num_funcs < 1)
        error ("boost_multi_thread_feval_octave_expressions: FUNCNAMES must contain at least one function name");

      // Validate all entries are strings
      std::vector<std::string> funcnames (num_funcs);
      for (int i = 0; i < num_funcs; ++i)
        {
          if (! funcname_cell(i).is_string ())
            error ("boost_multi_thread_feval_octave_expressions: element %d of FUNCNAMES is not a string", i + 1);
          funcnames[i] = funcname_cell(i).string_value ();
        }

      // Second argument (optional): num_threads (default 1)
      int num_threads = 1;
      if (nargin >= 2)
        {
          if (! args(1).is_scalar_type () || ! args(1).isnumeric ())
            error ("boost_multi_thread_feval_octave_expressions: NUM_THREADS must be a positive integer");
          num_threads = args(1).int_value ();
          if (num_threads < 1)
            error ("boost_multi_thread_feval_octave_expressions: NUM_THREADS must be a positive integer");
        }

      // Third argument (optional): cell array of args (each element is a cell)
      std::vector<octave_value_list> func_args_list (num_funcs);

      if (nargin >= 3)
        {
          if (! args(2).iscell ())
            {
              error ("boost_multi_thread_feval_octave_expressions: ARGS must be a cell array");
              return octave_value ();
            }
          const Cell args_cell = args(2).cell_value ();
          int args_len = args_cell.numel ();

          if (args_len != num_funcs)
            error ("boost_multi_thread_feval_octave_expressions: ARGS length (%d) must match FUNCNAMES length (%d)",
                   args_len, num_funcs);

          for (int i = 0; i < num_funcs; ++i)
            {
              if (! args_cell(i).iscell ())
                error ("boost_multi_thread_feval_octave_expressions: element %d of ARGS must be a cell array", i + 1);
              Cell inner_cell = args_cell(i).cell_value ();
              func_args_list[i] = octave_value_list (inner_cell);
            }
        }

      int total_threads = num_funcs * num_threads;

      // Create thread results (flat vector indexed by i*num_threads + t)
      std::vector<FevalThreadResultExprs> results (total_threads);
      boost::thread_group tg;

      // Launch threads - for each function name, num_threads threads
      for (int i = 0; i < num_funcs; ++i)
        {
          for (int t = 0; t < num_threads; ++t)
            {
              int idx = i * num_threads + t;
              tg.create_thread (boost::bind (feval_expression_thread_exprs,
                                             funcnames[i],
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
      error ("boost_multi_thread_feval_octave_expressions: %s", e.what ());
      return octave_value ();
    }
}
