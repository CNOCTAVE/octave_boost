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
static boost::mutex octave_mutex_exprs;

// Structure to hold per-thread results
struct EvalThreadResultExprs
{
  std::string thread_id;
  std::string output;
  octave_value func_result;
  double retval_status;
  std::string error_msg;
};

void eval_expression_thread_exprs (const std::string& expression,
                                   EvalThreadResultExprs& result)
{
  try
    {
      // Get thread ID
      std::ostringstream id_ss;
      id_ss << boost::this_thread::get_id ();
      result.thread_id = id_ss.str ();

      // Lock Octave interpreter for thread safety
      {
        boost::mutex::scoped_lock lock (octave_mutex_exprs);

        // Escape single quotes in the expression for evalc
        std::string escaped_expr = expression;
        std::size_t qpos = 0;
        while ((qpos = escaped_expr.find ("'", qpos)) != std::string::npos)
          {
            escaped_expr.replace (qpos, 1, "''");
            qpos += 2;
          }

        std::string eval_code = escaped_expr;

        octave_value_list evalc_args;
        evalc_args(0) = eval_code;

        // Call evalc to capture output and result
        octave_value_list evalc_result = BOOST_FEVAL ("evalc", evalc_args, 2);

        // Captured output (from evalc first return value)
        if (evalc_result.length () > 0)
          result.output = evalc_result(0).string_value ();

        // Result value from evalc second return value
        if (evalc_result.length () > 1)
          result.func_result = evalc_result(1);

        result.retval_status = 0.0;
      }
    }
  catch (const std::exception& e)
    {
      result.error_msg = e.what ();
      result.retval_status = -1.0;
    }
}

DEFUN_DLD (boost_multi_thread_eval_octave_expressions, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn  {Loadable Function} {@var{result} =} boost_multi_thread_eval_octave_expressions (@var{expressions})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_multi_thread_eval_octave_expressions (@var{expressions}, @var{num_threads})\n\
\n\
接收一个表达式字符串数组（元胞数组），对数组中的每个元素启动 num_threads 个 boost::thread，并行 eval() 指定的 Octave 表达式。\n\
num_threads 默认为 1。\n\
所有线程的输出将被重定向，格式为 [thread_id] 对应输出。\n\
函数返回一个二维结构体数组，尺寸为 [numel(expressions), num_threads]，索引方式为 result(expressions_index, thread_index)。\n\
\n\
基于 boost::thread 和 Octave 的 eval() 函数。\n\
\n\
示例：\n\
@example\n\
result = boost_multi_thread_eval_octave_expressions (@{\"rand()\", \"sum(rand(1,10))\"@}, 2);\n\
result(1, :)  % 第一个表达式在所有线程上的结果\n\
result(:, 1)  % 所有表达式在第一个线程上的结果\n\
@end example\n\
\n\
@seealso{boost_multi_thread_eval_octave_expression, boost_multi_thread_run_octave_files, boost_multi_thread_call_octave_functions, boost_multi_thread_feval_octave_expressions}\n\
@end deftypefn")
{
  try
    {
      int nargin = args.length ();

      if (nargin < 1 || nargin > 2)
        {
          print_usage ();
          return octave_value ();
        }

      // First argument: cell array of expression strings
      if (! args(0).iscell ())
        error ("boost_multi_thread_eval_octave_expressions: EXPRESSIONS must be a cell array of strings");

      const Cell expr_cell = args(0).cell_value ();
      int num_exprs = expr_cell.numel ();

      if (num_exprs < 1)
        error ("boost_multi_thread_eval_octave_expressions: EXPRESSIONS must contain at least one expression");

      // Validate all entries are strings
      std::vector<std::string> expressions (num_exprs);
      for (int i = 0; i < num_exprs; ++i)
        {
          if (! expr_cell(i).is_string ())
            error ("boost_multi_thread_eval_octave_expressions: element %d of EXPRESSIONS is not a string", i + 1);
          expressions[i] = expr_cell(i).string_value ();
        }

      // Second argument (optional): num_threads (default 1)
      int num_threads = 1;
      if (nargin >= 2)
        {
          if (! args(1).is_scalar_type () || ! args(1).isnumeric ())
            error ("boost_multi_thread_eval_octave_expressions: NUM_THREADS must be a positive integer");
          num_threads = args(1).int_value ();
          if (num_threads < 1)
            error ("boost_multi_thread_eval_octave_expressions: NUM_THREADS must be a positive integer");
        }

      int total_threads = num_exprs * num_threads;

      // Create thread results (flat vector indexed by i*num_threads + t)
      std::vector<EvalThreadResultExprs> results (total_threads);
      boost::thread_group tg;

      // Launch threads - for each expression, num_threads threads
      for (int i = 0; i < num_exprs; ++i)
        {
          for (int t = 0; t < num_threads; ++t)
            {
              int idx = i * num_threads + t;
              tg.create_thread (boost::bind (eval_expression_thread_exprs,
                                             expressions[i],
                                             boost::ref (results[idx])));
            }
        }

      // Wait for all threads to complete
      tg.join_all ();

      // Build return value: struct array with dimensions [num_exprs, num_threads]
      dim_vector dims (num_exprs, num_threads);
      octave_map ret (dims);

      Cell thread_id_cell (dims);
      Cell output_cell (dims);
      Cell result_cell (dims);
      Cell status_cell (dims);
      Cell error_msg_cell (dims);

      for (int i = 0; i < num_exprs; ++i)
        {
          for (int t = 0; t < num_threads; ++t)
            {
              int idx = i * num_threads + t;
              thread_id_cell(i, t) = octave_value (results[idx].thread_id);
              output_cell(i, t) = octave_value (results[idx].output);
              result_cell(i, t) = results[idx].func_result;
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
      error ("boost_multi_thread_eval_octave_expressions: %s", e.what ());
      return octave_value ();
    }
}
