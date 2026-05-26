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

//

// Structure to hold per-thread results
struct ThreadResult
{
  std::string thread_id;
  std::string output;
  double retval_status;
  std::string error_msg;
};

void run_file_thread (const std::string& filename, ThreadResult& result)
{
  try
    {
      // Get thread ID
      std::ostringstream id_ss;
      id_ss << boost::this_thread::get_id ();
      result.thread_id = id_ss.str ();

      // Build evalc command to run the file and capture output
      std::string escaped_filename = filename;
      // Simple escape for single quotes in filename
      std::size_t pos = 0;
      while ((pos = escaped_filename.find ("'", pos)) != std::string::npos)
        {
          escaped_filename.replace (pos, 1, "''");
          pos += 2;
        }

      std::string eval_code = "source ('" + escaped_filename + "');";

      // Lock Octave interpreter for thread safety
      {
        boost::mutex::scoped_lock lock (octave_mutex);

        // Use evalc to capture output
        octave_value_list evalc_args;
        evalc_args(0) = eval_code;
        octave_value_list evalc_result = BOOST_FEVAL ("evalc", evalc_args, 2);

        // Get captured output
        result.output = evalc_result(0).string_value ();

        // Get return value status (0 = success)
        result.retval_status = 0.0;
      }
    }
  catch (const std::exception& e)
    {
      result.error_msg = e.what ();
      result.retval_status = -1.0;
    }
}

DEFUN_DLD (boost_multi_thread_run_octave_file, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn  {Loadable Function} {@var{result} =} boost_multi_thread_run_octave_file (@var{filename})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_multi_thread_run_octave_file (@var{filename}, @var{num_threads})\n\
\n\
启动多个 boost::thread，并行执行指定的 Octave 文件。\n\
所有线程的输出将被重定向，格式为 [thread_id] 对应输出。\n\
函数返回一个结构体数组，包含每个线程的 thread_id、output 和 status。\n\
\n\
基于 boost::thread 和 Octave 的 source() 函数。\n\
\n\
示例：\n\
@example\n\
result = boost_multi_thread_run_octave_file (\"myscript.m\", 4);\n\
@end example\n\
\n\
@seealso{boost_multi_thread_call_octave_function, boost_multi_thread_eval_octave_expression, boost_multi_thread_feval_octave_expression}\n\
@end deftypefn")
{
  try
    {
      // Parse arguments
      int nargin = args.length ();

      if (nargin < 1 || nargin > 2)
        {
          print_usage ();
          return octave_value ();
        }

      // First argument: filename
      if (! args(0).is_string ())
        error ("boost_multi_thread_run_octave_file: FILENAME must be a string");
      std::string filename = args(0).string_value ();

      // Second argument (optional): num_threads (default 1)
      int num_threads = 1;
      if (nargin >= 2)
        {
          if (! args(1).is_scalar_type () || ! args(1).isnumeric ())
            error ("boost_multi_thread_run_octave_file: NUM_THREADS must be a positive integer");
          num_threads = args(1).int_value ();
          if (num_threads < 1)
            error ("boost_multi_thread_run_octave_file: NUM_THREADS must be a positive integer");
        }

      // Create thread results
      std::vector<ThreadResult> results (num_threads);
      boost::thread_group tg;

      // Launch threads
      for (int i = 0; i < num_threads; ++i)
        {
          tg.create_thread (boost::bind (run_file_thread,
                                         filename,
                                         boost::ref (results[i])));
        }

      // Wait for all threads to complete
      tg.join_all ();

      // Build return value: struct array
      octave_map ret;
      Cell thread_id_cell (dim_vector (num_threads, 1));
      Cell output_cell (dim_vector (num_threads, 1));
      Cell status_cell (dim_vector (num_threads, 1));

      for (int i = 0; i < num_threads; ++i)
        {
          thread_id_cell(i) = octave_value (results[i].thread_id);
          output_cell(i) = octave_value (results[i].output);
          status_cell(i) = octave_value (results[i].retval_status);
        }

      ret.assign ("thread_id", thread_id_cell);
      ret.assign ("output", output_cell);
      ret.assign ("status", status_cell);

      return octave_value (ret);
    }
  catch (const std::exception& e)
    {
      error ("boost_multi_thread_run_octave_file: %s", e.what ());
      return octave_value ();
    }
}
