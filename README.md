# octave_boost

 A comprehensive open-source high performance
 processing package for GNU Octave based on Boost C++
 Libraries. Built with C++17 standard.
 Provides direct access to Boost.Accumulators statistical
 accumulators for online computation of count, covariance,
 density, error of mean, extended P^2 quantiles, kurtosis,
 max/min, mean, median, moments, P^2 cumulative distribution,
 P^2 quantile, peaks over threshold, POT quantile, POT tail mean,
 skewness, sum, tail statistics, coherent/non-coherent tail mean,
 tail quantile, tail variate, tail variate means, variance,
 weighted covariance, weighted density, weighted extended P^2 quantiles,
 and weighted kurtosis.
 Also provides Boost.Chrono duration operations including 
 count, add, subtract, multiply, divide, remainder, equality
 and inequality comparison, clock elapsed time measurement 
 (system_clock, steady_clock, high_resolution_clock, 
 process_cpu_clock, thread_clock), and time formatting
 (time_fmt for duration and system_clock).
 Also provides Boost.Date_Time Gregorian date handling: get year/month/day,
 year_month_day, day_of_week, day_of_year, end_of_month (get_end_of_month
 and end_of_month), is_infinity, is_neg_infinity, is_pos_infinity,
 is_not_a_date, is_special, modjulian_day, julian_day, week_number,
 to_simple_string, to_iso_string, to_iso_extended_string, date_eq,
 date_ne, date_gt, date_lt, date_ge, date_le, add_days, minus_days,
 minus_date, to_tm, and date_from_tm.
 Also provides Boost.Thread multi-threading: run Octave files, call
 functions, eval expressions, and feval expressions in parallel using
 boost::thread, with per-thread output capture and thread ID labeling.
 The plural variants (boost_multi_thread_run_octave_files, 
 boost_multi_thread_call_octave_functions,
 boost_multi_thread_eval_octave_expressions,
 boost_multi_thread_feval_octave_expressions)
 accept cell arrays and launch one thread per element.
 Also provides Boost.PropertyTree config format conversion, including
 XML, JSON, INI and INFO(INF) formats.

## Documentation

Check out document: [octave_boost Document](https://cnoctave.github.io/octave_boost/index.html)

## Package Structure
```
octave_boost/
├── src/          # C++ source files (.cc) — Octave loadable functions (DEFUN_DLD)
├── inst/         # Octave function files (.m)
├── docs/         # Documentation (zh-CN, multi-language support)
├── .github/      # GitHub Actions CI workflow
├── DESCRIPTION   # Package metadata
├── INDEX         # Function categories
└── NEWS          # Version history
```

## License
GNU General Public License v3.0 or later (GPLv3+)

## Citation
If you use octave_boost in academic research, please cite:

```
@misc{CNOCTAVE2024,
  author = {Yu Hongbo},
  title = {octave_boost},
  year = {2024},
  howpublished = {\url{https://github.com/CNOCTAVE/octave_boost}}
}
```

## How to translate octave_boost Document into another language
In ./docs directory, index.html is zh-CN simplified Chinese document.
For example, if you want to translate document into English.
1. Copy index.html as another document with different language code as filename,
   for example, en-US.html.
2. Translate en-US.html into English.
3. Add dropdown like the picture below to every *.html.
   For example, add dropdown "en-US English".
   ![the dropdown looking](./docs/translate_dropdown.png)
   The code for adding dropdown is like the picture below.
   ![the dropdown code](./docs/translate_dropdown_code.png)
4. PR to octave_boost.
