## Copyright (C) 2024-2026 Yu Hongbo <yuhongbo@member.fsf.org>,
##                    CNOCTAVE <cnoctave@qq.com>
##
## This program is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by the
## Free Software Foundation; either version 3 of the License, or (at your
## option) any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License along
## with this program; if not, see <http://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn  {} {} demo_boost_thread
## @deftypefnx {} {} demo_boost_thread (@var{n})
##
## 演示 octave_boost boost::thread 模块的使用方法。
## 可选参数 @var{n} 指定线程数（默认 4）。
##
## 此函数创建示例文件并演示以下功能：
## @itemize
## @item @code{boost_multi_thread_run_octave_file}
## @item @code{boost_multi_thread_call_octave_function}
## @item @code{boost_multi_thread_eval_octave_expression}
## @item @code{boost_multi_thread_feval_octave_expression}
## @end itemize
##
## @seealso{boost_multi_thread_run_octave_file, boost_multi_thread_call_octave_function,
## boost_multi_thread_eval_octave_expression, boost_multi_thread_feval_octave_expression}
## @end deftypefn

function demo_boost_thread (n = 4)

  ## 0. 参数检查
  if (nargin > 1)
    print_usage ();
    return;
  endif

  if (! isnumeric (n) || n < 1 || n != fix (n))
    error ("demo_boost_thread: N must be a positive integer");
  endif

  printf ("========================================\n");
  printf ("  octave_boost boost::thread 模块演示\n");
  printf ("========================================\n");
  printf ("使用 %d 个线程\n\n", n);

  ## ============================================
  ## 1. boost_multi_thread_run_octave_file
  ## ============================================
  printf ("--- 1. boost_multi_thread_run_octave_file ---\n\n");

  ## 创建一个临时的 Octave 脚本文件
  script_file = [tempname(), ".m"];
  fid = fopen (script_file, "w");
  fprintf (fid, "%% 这是一个被多线程执行的脚本\n");
  fprintf (fid, "x = rand();\n");
  fprintf (fid, "fprintf('随机数: %.4f\\n', x);\n");
  fprintf (fid, "y = x * 100;\n");
  fprintf (fid, "fprintf('放大100倍: %.2f\\n', y);\n");
  fclose (fid);

  printf ("执行脚本文件: %s\n\n", script_file);
  result1 = boost_multi_thread_run_octave_file (script_file, n);
  disp (result1);

  ## 删除临时文件
  delete (script_file);

  printf ("\n");

  ## ============================================
  ## 2. boost_multi_thread_call_octave_function
  ## ============================================
  printf ("--- 2. boost_multi_thread_call_octave_function ---\n\n");
  printf ("并行调用函数句柄 @rand(3,3):\n");
  result2 = boost_multi_thread_call_octave_function (@rand, n, {3, 3});
  disp (result2);

  printf ("\n");

  ## ============================================
  ## 3. boost_multi_thread_eval_octave_expression
  ## ============================================
  printf ("--- 3. boost_multi_thread_eval_octave_expression ---\n\n");
  printf ("并行 eval 'sum(rand(1,5))':\n");
  result3 = boost_multi_thread_eval_octave_expression ("sum(rand(1,5))", n);
  disp (result3);

  printf ("\n");

  ## ============================================
  ## 4. boost_multi_thread_feval_octave_expression
  ## ============================================
  printf ("--- 4. boost_multi_thread_feval_octave_expression ---\n\n");
  printf ("并行 feval sin(pi/2):\n");
  result4 = boost_multi_thread_feval_octave_expression ("sin", n, {pi/2});
  disp (result4);

  printf ("\n");
  printf ("========================================\n");
  printf ("  boost::thread 模块演示结束\n");
  printf ("========================================\n");

endfunction
