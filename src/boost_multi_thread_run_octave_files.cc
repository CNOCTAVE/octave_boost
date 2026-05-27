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
static boost::mutex octave_mutex_files;

// Structure to hold per-thread results
struct ThreadResultFiles
{
  std::string thread_id;
  std::string output;
  double retval_status;
  std::string error_msg;
};

void run_file_thread_files (const std::string& filename, ThreadResultFiles& result)
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
        boost::mutex::scoped_lock lock (octave_mutex_files);

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

DEFUN_DLD (boost_multi_thread_run_octave_files, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn  {Loadable Function} {@var{result} =} boost_multi_thread_run_octave_files (@var{filenames})\n\
@deftypefnx {Loadable Function} {@var{result} =} boost_multi_thread_run_octave_files (@var{filenames}, @var{num_threads})\n\
\n\
接收一个字符串数组（元胞数组），对数组中的每个元素启动 num_threads 个 boost::thread，并行执行指定的 Octave 脚本文件。\n\
num_threads 默认为 1。\n\
所有线程的输出将被重定向，格式为 [thread_id] 对应输出。\n\
函数返回一个二维结构体数组，尺寸为 [numel(filenames), num_threads]，索引方式为 result(filenames_index, thread_index)。\n\
\n\
基于 boost::thread 和 Octave 的 source() 函数。\n\
\n\
示例：\n\
@example\n\
result = boost_multi_thread_run_octave_files ({\"script1.m\", \"script2.m\"}, 3);\n\
result(1, :)  % 第一个脚本在所有线程上的结果\n\
result(:, 1)  % 所有脚本在第一个线程上的结果\n\
@end example\n\
\n\
@seealso{boost_multi_thread_run_octave_file, boost_multi_thread_call_octave_functions, boost_multi_thread_eval_octave_expressions, boost_multi_thread_feval_octave_expressions}\n\
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

      // First argument: cell array of filenames (strings)
      if (! args(0).iscell ())
        error ("boost_multi_thread_run_octave_files: FILENAMES must be a cell array of strings");

      const Cell file_cell = args(0).cell_value ();
      int num_files = file_cell.numel ();

      if (num_files < 1)
        error ("boost_multi_thread_run_octave_files: FILENAMES must contain at least one filename");

      // Validate all entries are strings
      std::vector<std::string> filenames (num_files);
      for (int i = 0; i < num_files; ++i)
        {
          if (! file_cell(i).is_string ())
            error ("boost_multi_thread_run_octave_files: element %d of FILENAMES is not a string", i + 1);
          filenames[i] = file_cell(i).string_value ();
        }

      // Second argument (optional): num_threads (default 1)
      int num_threads = 1;
      if (nargin >= 2)
        {
          if (! args(1).is_scalar_type () || ! args(1).isnumeric ())
            error ("boost_multi_thread_run_octave_files: NUM_THREADS must be a positive integer");
          num_threads = args(1).int_value ();
          if (num_threads < 1)
            error ("boost_multi_thread_run_octave_files: NUM_THREADS must be a positive integer");
        }

      int total_threads = num_files * num_threads;

      // Create thread results (flat vector indexed by i*num_threads + t)
      std::vector<ThreadResultFiles> results (total_threads);
      boost::thread_group tg;

      // Launch threads - for each file, num_threads threads
      for (int i = 0; i < num_files; ++i)
        {
          for (int t = 0; t < num_threads; ++t)
            {
              int idx = i * num_threads + t;
              tg.create_thread (boost::bind (run_file_thread_files,
                                             filenames[i],
                                             boost::ref (results[idx])));
            }
        }

      // Wait for all threads to complete
      tg.join_all ();

      // Build return value: struct array with dimensions [num_files, num_threads]
      dim_vector dims (num_files, num_threads);
      octave_map ret (dims);

      Cell thread_id_cell (dims);
      Cell output_cell (dims);
      Cell status_cell (dims);
      Cell error_msg_cell (dims);

      for (int i = 0; i < num_files; ++i)
        {
          for (int t = 0; t < num_threads; ++t)
            {
              int idx = i * num_threads + t;
              thread_id_cell(i, t) = octave_value (results[idx].thread_id);
              output_cell(i, t) = octave_value (results[idx].output);
              status_cell(i, t) = octave_value (results[idx].retval_status);
              error_msg_cell(i, t) = octave_value (results[idx].error_msg);
            }
        }

      ret.assign ("thread_id", thread_id_cell);
      ret.assign ("output", output_cell);
      ret.assign ("status", status_cell);
      ret.assign ("error_msg", error_msg_cell);

      return octave_value (ret);
    }
  catch (const std::exception& e)
    {
      error ("boost_multi_thread_run_octave_files: %s", e.what ());
      return octave_value ();
    }
}
