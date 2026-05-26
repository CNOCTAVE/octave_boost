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

#if !defined (octave_boost_date_common_h)
#define octave_boost_date_common_h 1

#include <octave/oct.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <sstream>
#include <iomanip>

namespace bg = boost::gregorian;
namespace bp = boost::posix_time;

// Parse a date from Octave arguments.
// Supports the following forms:
//   struct with fields: year, month, day
//   string: "YYYY-MM-DD", "neg_infin", "pos_infin", "not_a_date_time"
//   three scalars: year, month, day
inline bg::date
parse_date (const octave_value_list& args, int offset = 0)
{
  // Case 1: struct with year, month, day fields
  if (args (offset).isstruct ())
    {
      octave_scalar_map m = args (offset).scalar_map_value ();

      if (! m.isfield ("year") || ! m.isfield ("month") || ! m.isfield ("day"))
        error ("parse_date: date struct must have fields 'year', 'month', 'day'");

      double y = m.getfield ("year").double_value ();
      double mo = m.getfield ("month").double_value ();
      double d = m.getfield ("day").double_value ();

      return bg::date (static_cast<unsigned short> (y),
                       static_cast<unsigned short> (mo),
                       static_cast<unsigned short> (d));
    }

  // Case 2: string
  if (args (offset).is_string ())
    {
      std::string s = args (offset).string_value ();

      if (s == "neg_infin")
        return bg::date (boost::date_time::neg_infin);
      if (s == "pos_infin")
        return bg::date (boost::date_time::pos_infin);
      if (s == "not_a_date_time")
        return bg::date (boost::date_time::not_a_date_time);
      if (s == "max_date_time")
        return bg::date (boost::date_time::max_date_time);
      if (s == "min_date_time")
        return bg::date (boost::date_time::min_date_time);

      // Try "YYYY-MM-DD" format
      return bg::from_string (s);
    }

  // Case 3: three numeric arguments (year, month, day)
  if ((args.length () - offset) >= 3)
    {
      double y = args (offset).double_value ();
      double mo = args (offset + 1).double_value ();
      double d = args (offset + 2).double_value ();

      return bg::date (static_cast<unsigned short> (y),
                       static_cast<unsigned short> (mo),
                       static_cast<unsigned short> (d));
    }

  // Case 4: default constructor -> not_a_date_time
  return bg::date (boost::date_time::not_a_date_time);
}

// Convert a boost::gregorian::date to an Octave struct with year, month, day
inline octave_scalar_map
date_to_struct (const bg::date& d)
{
  octave_scalar_map ret;
  ret.setfield ("year", octave_value (static_cast<double> (d.year ())));
  ret.setfield ("month", octave_value (static_cast<double> (d.month ())));
  ret.setfield ("day", octave_value (static_cast<double> (d.day ())));
  return ret;
}

// Convert a boost::gregorian::date to a string "YYYY-MM-DD"
inline std::string
date_to_string (const bg::date& d)
{
  if (d.is_not_a_date ())
    return "not_a_date_time";
  if (d.is_infinity ())
    {
      if (d.is_neg_infinity ())
        return "-infinity";
      else
        return "+infinity";
    }

  std::ostringstream oss;
  oss << d.year () << "-"
      << std::setw (2) << std::setfill ('0') << d.month ()
      << "-"
      << std::setw (2) << std::setfill ('0') << d.day ();
  return oss.str ();
}

// Parse a date_period from Octave arguments.
// Supports the following forms:
//   struct with fields: begin (date struct) and end (date struct) or begin and length
//   two dates: each as struct, string, or three scalars
//   date + days: date (struct/string) + integer days count
struct date_period_result {
  bg::date_period period;
  int consumed;  // number of Octave args consumed
};

inline date_period_result
parse_date_period (const octave_value_list& args, int offset = 0)
{
  // Case 1: struct with begin/end or begin/length fields
  if (args (offset).isstruct ())
    {
      octave_scalar_map m = args (offset).scalar_map_value ();

      if (m.isfield ("begin"))
        {
          octave_scalar_map begin_map = m.getfield ("begin").scalar_map_value ();

          double by = begin_map.getfield ("year").double_value ();
          double bmo = begin_map.getfield ("month").double_value ();
          double bd = begin_map.getfield ("day").double_value ();
          bg::date begin (static_cast<unsigned short> (by),
                           static_cast<unsigned short> (bmo),
                           static_cast<unsigned short> (bd));

          if (m.isfield ("end"))
            {
              octave_scalar_map end_map = m.getfield ("end").scalar_map_value ();

              double ey = end_map.getfield ("year").double_value ();
              double emo = end_map.getfield ("month").double_value ();
              double ed = end_map.getfield ("day").double_value ();
              bg::date end (static_cast<unsigned short> (ey),
                             static_cast<unsigned short> (emo),
                             static_cast<unsigned short> (ed));
              return {bg::date_period (begin, end), 1};
            }
          else if (m.isfield ("length"))
            {
              long len = m.getfield ("length").long_value ();
              return {bg::date_period (begin, bg::days (len)), 1};
            }
          else
            error ("parse_date_period: struct with 'begin' must also have 'end' or 'length'");
        }
    }

  // Case 2 & 3: individual arguments (two dates, or date + days)
  int n1;
  if (args (offset).isstruct () || args (offset).is_string ())
    n1 = 1;
  else
    n1 = 3;

  bg::date begin = parse_date (args, offset);
  int next = offset + n1;

  if (next >= args.length ())
    error ("parse_date_period: insufficient arguments for end date or duration");

  // Check if next arg is a date (struct/string) → two dates form
  if (args (next).isstruct () || args (next).is_string ())
    {
      bg::date end = parse_date (args, next);
      return {bg::date_period (begin, end), next + 1 - offset};
    }

  // Numeric: could be 3-scalar date or single days count
  if (args (next).isnumeric ())
    {
      int remaining = args.length () - next;
      if (remaining >= 3)
        {
          // remaining >= 3 numerics → 3-scalar date (for both n1=1 and n1=3 cases)
          bg::date end = parse_date (args, next);
          return {bg::date_period (begin, end), next + 3 - offset};
        }
      else if (remaining >= 1)
        {
          // Single days value
          long ndays = args (next).long_value ();
          return {bg::date_period (begin, bg::days (ndays)), next + 1 - offset};
        }
    }

  error ("parse_date_period: unable to parse date_period from arguments");
  return {bg::date_period (bg::date (boost::date_time::not_a_date_time),
                            bg::days (0)), 0};
}

// Convert a boost::gregorian::date_period to an Octave struct
inline octave_scalar_map
date_period_to_struct (const bg::date_period& dp)
{
  octave_scalar_map ret;
  ret.setfield ("begin", octave_value (date_to_struct (dp.begin ())));
  ret.setfield ("last", octave_value (date_to_struct (dp.last ())));
  ret.setfield ("end", octave_value (date_to_struct (dp.end ())));
  ret.setfield ("length", octave_value (static_cast<double> (dp.length ().days ())));
  return ret;
}

// ========== ptime helper functions ==========

// Parse a ptime from Octave arguments.
// Supports the following forms:
//   struct with fields: date (struct with year/month/day) and
//                       time_of_day (struct with hours/minutes/seconds/fractional_seconds)
//   string: ISO format "YYYY-MM-DD HH:MM:SS", or special values
//   default -> not_a_date_time
inline bp::ptime
parse_ptime (const octave_value_list& args, int offset = 0)
{
  // Case 1: struct with date and optional time_of_day fields
  if (args (offset).isstruct ())
    {
      octave_scalar_map m = args (offset).scalar_map_value ();

      if (! m.isfield ("date"))
        error ("parse_ptime: struct must have a 'date' field");

      octave_scalar_map date_map = m.getfield ("date").scalar_map_value ();

      double y = date_map.getfield ("year").double_value ();
      double mo = date_map.getfield ("month").double_value ();
      double d = date_map.getfield ("day").double_value ();
      bg::date dt (static_cast<unsigned short> (y),
                    static_cast<unsigned short> (mo),
                    static_cast<unsigned short> (d));

      if (m.isfield ("time_of_day"))
        {
          octave_scalar_map tod_map = m.getfield ("time_of_day").scalar_map_value ();

          double hr = tod_map.isfield ("hours") ? tod_map.getfield ("hours").double_value () : 0.0;
          double min = tod_map.isfield ("minutes") ? tod_map.getfield ("minutes").double_value () : 0.0;
          double sec = tod_map.isfield ("seconds") ? tod_map.getfield ("seconds").double_value () : 0.0;
          double fs = tod_map.isfield ("fractional_seconds") ? tod_map.getfield ("fractional_seconds").double_value () : 0.0;


          return bp::ptime (dt, bp::time_duration (static_cast<long> (hr),
                                                    static_cast<long> (min),
                                                    static_cast<long> (sec),
                                                    static_cast<long> (fs)));
        }

      // Date only, time defaults to midnight
      return bp::ptime (dt, bp::time_duration (0, 0, 0, 0));
    }

  // Case 2: string
  if (args (offset).is_string ())
    {
      std::string s = args (offset).string_value ();

      if (s == "neg_infin")
        return bp::ptime (boost::date_time::neg_infin);
      if (s == "pos_infin")
        return bp::ptime (boost::date_time::pos_infin);
      if (s == "not_a_date_time")
        return bp::ptime (boost::date_time::not_a_date_time);
      if (s == "max_date_time")
        return bp::ptime (boost::date_time::max_date_time);
      if (s == "min_date_time")
        return bp::ptime (boost::date_time::min_date_time);

      // Try from delimited string "YYYY-MM-DD HH:MM:SS"
      try
        {
          return bp::time_from_string (s);
        }
      catch (...)
        {
          // Try ISO extended string "YYYY-MM-DDTHH:MM:SS"
          try
            {
              return bp::from_iso_extended_string (s);
            }
          catch (...)
            {
              error ("parse_ptime: unable to parse string '%s' as ptime", s.c_str ());
            }
        }
    }

  // Case 3: default constructor -> not_a_date_time
  return bp::ptime (boost::date_time::not_a_date_time);
}

// Parse a time_duration from Octave arguments.
// Supports the following forms:
//   struct with fields: hours, minutes, seconds, fractional_seconds
//   string: "HH:MM:SS.fff" (from delimited string)
//   3 or 4 numeric args: (hours, minutes, seconds[, fractional_seconds])
inline bp::time_duration
parse_time_duration (const octave_value_list& args, int offset = 0)
{
  // Case 1: struct with time_duration fields
  if (args (offset).isstruct ())
    {
      octave_scalar_map m = args (offset).scalar_map_value ();

      long hr = m.isfield ("hours") ? m.getfield ("hours").long_value () : 0;
      long min = m.isfield ("minutes") ? m.getfield ("minutes").long_value () : 0;
      long sec = m.isfield ("seconds") ? m.getfield ("seconds").long_value () : 0;
      long fs = m.isfield ("fractional_seconds") ? m.getfield ("fractional_seconds").long_value () : 0;


      return bp::time_duration (hr, min, sec, fs);
    }

  // Case 2: string
  if (args (offset).is_string ())
    {
      std::string s = args (offset).string_value ();

      if (s == "neg_infin")
        return bp::time_duration (boost::date_time::neg_infin);
      if (s == "pos_infin")
        return bp::time_duration (boost::date_time::pos_infin);
      if (s == "not_a_date_time")
        return bp::time_duration (boost::date_time::not_a_date_time);
      if (s == "max_date_time")
        return bp::time_duration (boost::date_time::max_date_time);
      if (s == "min_date_time")
        return bp::time_duration (boost::date_time::min_date_time);

      return bp::duration_from_string (s);
    }

  // Case 3: numeric arguments (hours, minutes, seconds[, fractional_seconds])
  long hr = args (offset).long_value ();
  long min = args (offset + 1).long_value ();
  long sec = args (offset + 2).long_value ();
  long fs = (args.length () > (offset + 3)) ? args (offset + 3).long_value () : 0;


  return bp::time_duration (hr, min, sec, fs);
}

// Convert a boost::posix_time::ptime to an Octave struct with date and time_of_day
inline octave_scalar_map
ptime_to_struct (const bp::ptime& t)
{
  octave_scalar_map ret;

  if (t.is_not_a_date_time ())
    {
      ret.setfield ("date", octave_value ("not_a_date_time"));
      return ret;
    }
  if (t.is_infinity ())
    {
      if (t.is_neg_infinity ())
        ret.setfield ("date", octave_value ("-infinity"));
      else
        ret.setfield ("date", octave_value ("+infinity"));
      return ret;
    }

  bg::date d = t.date ();
  octave_scalar_map date_s;
  date_s.setfield ("year", octave_value (static_cast<double> (d.year ())));
  date_s.setfield ("month", octave_value (static_cast<double> (d.month ())));
  date_s.setfield ("day", octave_value (static_cast<double> (d.day ())));
  ret.setfield ("date", octave_value (date_s));

  bp::time_duration td = t.time_of_day ();
  octave_scalar_map tod_s;
  tod_s.setfield ("hours", octave_value (static_cast<double> (td.hours ())));
  tod_s.setfield ("minutes", octave_value (static_cast<double> (td.minutes ())));
  tod_s.setfield ("seconds", octave_value (static_cast<double> (td.seconds ())));
  tod_s.setfield ("fractional_seconds", octave_value (static_cast<double> (td.fractional_seconds ())));
  tod_s.setfield ("total_seconds", octave_value (static_cast<double> (td.total_seconds ())));
  ret.setfield ("time_of_day", octave_value (tod_s));

  return ret;
}

// ========== time_period helper functions ==========

// Parse a time_period from Octave arguments.
// Supports the following forms:
//   struct with fields: begin (ptime struct) and end (ptime struct) or begin and length (time_duration)
//   two ptimes (each as struct or string)
//   ptime + time_duration
struct time_period_result {
  bp::time_period period;
  int consumed;  // number of Octave args consumed
};

inline time_period_result
parse_time_period (const octave_value_list& args, int offset = 0)
{
  // Case 1: struct with begin/end or begin/length fields
  if (args (offset).isstruct ())
    {
      octave_scalar_map m = args (offset).scalar_map_value ();

      if (m.isfield ("begin"))
        {
          bp::ptime begin;
          octave_value begin_ov = m.getfield ("begin");
          if (begin_ov.is_string ())
            {
              octave_value_list tmp_args;
              tmp_args(0) = begin_ov;
              begin = parse_ptime (tmp_args, 0);
            }
          else if (begin_ov.isstruct ())
            {
              octave_scalar_map begin_map = begin_ov.scalar_map_value ();

              double by = begin_map.getfield ("year").double_value ();
              double bmo = begin_map.getfield ("month").double_value ();
              double bd = begin_map.getfield ("day").double_value ();
              bg::date bdate (static_cast<unsigned short> (by),
                               static_cast<unsigned short> (bmo),
                               static_cast<unsigned short> (bd));

              bp::time_duration btd (0, 0, 0, 0);
              if (begin_map.isfield ("hours"))
                btd = bp::time_duration (
                  begin_map.getfield ("hours").long_value (),
                  begin_map.isfield ("minutes") ? begin_map.getfield ("minutes").long_value () : 0,
                  begin_map.isfield ("seconds") ? begin_map.getfield ("seconds").long_value () : 0,
                  begin_map.isfield ("fractional_seconds") ? begin_map.getfield ("fractional_seconds").long_value () : 0);
              else if (begin_map.isfield ("time_of_day"))
                {
                  octave_scalar_map tod = begin_map.getfield ("time_of_day").scalar_map_value ();
                  btd = bp::time_duration (
                    tod.isfield ("hours") ? tod.getfield ("hours").long_value () : 0,
                    tod.isfield ("minutes") ? tod.getfield ("minutes").long_value () : 0,
                    tod.isfield ("seconds") ? tod.getfield ("seconds").long_value () : 0,
                    tod.isfield ("fractional_seconds") ? tod.getfield ("fractional_seconds").long_value () : 0);
                }
              begin = bp::ptime (bdate, btd);
            }
          else
            error ("parse_time_period: 'begin' field must be a struct or string");

          if (m.isfield ("end"))
            {
              bp::ptime end;
              octave_value end_ov = m.getfield ("end");
              if (end_ov.is_string ())
                {
                  octave_value_list tmp_args;
                  tmp_args(0) = end_ov;
                  end = parse_ptime (tmp_args, 0);
                }
              else if (end_ov.isstruct ())
                {
                  octave_scalar_map end_map = end_ov.scalar_map_value ();
                  double ey = end_map.getfield ("year").double_value ();
                  double emo = end_map.getfield ("month").double_value ();
                  double ed = end_map.getfield ("day").double_value ();
                  bg::date edate (static_cast<unsigned short> (ey),
                                   static_cast<unsigned short> (emo),
                                   static_cast<unsigned short> (ed));

                  bp::time_duration etd (0, 0, 0, 0);
                  if (end_map.isfield ("hours"))
                    etd = bp::time_duration (
                      end_map.getfield ("hours").long_value (),
                      end_map.isfield ("minutes") ? end_map.getfield ("minutes").long_value () : 0,
                      end_map.isfield ("seconds") ? end_map.getfield ("seconds").long_value () : 0,
                      end_map.isfield ("fractional_seconds") ? end_map.getfield ("fractional_seconds").long_value () : 0);
                  else if (end_map.isfield ("time_of_day"))
                    {
                      octave_scalar_map tod = end_map.getfield ("time_of_day").scalar_map_value ();
                      etd = bp::time_duration (
                        tod.isfield ("hours") ? tod.getfield ("hours").long_value () : 0,
                        tod.isfield ("minutes") ? tod.getfield ("minutes").long_value () : 0,
                        tod.isfield ("seconds") ? tod.getfield ("seconds").long_value () : 0,
                        tod.isfield ("fractional_seconds") ? tod.getfield ("fractional_seconds").long_value () : 0);
                    }
                  end = bp::ptime (edate, etd);
                }
              else
                error ("parse_time_period: 'end' field must be a struct or string");

              return {bp::time_period (begin, end), 1};
            }
          else if (m.isfield ("length"))
            {
              bp::time_duration len;
              octave_value len_ov = m.getfield ("length");
              if (len_ov.is_string ())
                {
                  octave_value_list tmp_args;
                  tmp_args(0) = len_ov;
                  len = parse_time_duration (tmp_args, 0);
                }
              else if (len_ov.isstruct ())
                {
                  octave_scalar_map len_map = len_ov.scalar_map_value ();
                  long lhr = len_map.isfield ("hours") ? len_map.getfield ("hours").long_value () : 0;
                  long lmin = len_map.isfield ("minutes") ? len_map.getfield ("minutes").long_value () : 0;
                  long lsec = len_map.isfield ("seconds") ? len_map.getfield ("seconds").long_value () : 0;
                  long lfs = len_map.isfield ("fractional_seconds") ? len_map.getfield ("fractional_seconds").long_value () : 0;
                  len = bp::time_duration (lhr, lmin, lsec, lfs);
                }
              else
                error ("parse_time_period: 'length' field must be a struct or string");

              return {bp::time_period (begin, len), 1};
            }
          else
            error ("parse_time_period: struct with 'begin' must also have 'end' or 'length'");
        }
    }

  // Case 2 & 3: individual arguments (two ptimes, or ptime + time_duration)
  bp::ptime begin = parse_ptime (args, offset);
  // parse_ptime consumes 1 arg for struct/string; for other types it defaults to not_a_date_time
  int n1 = 1;  // always 1 since parse_ptime only accepts struct/string for valid ptime

  int next = offset + n1;

  if (next >= args.length ())
    error ("parse_time_period: insufficient arguments for end time or duration");

  // Check if next arg is a ptime (struct/string) → two ptimes form
  if (args (next).isstruct () || args (next).is_string ())
    {
      bp::ptime end = parse_ptime (args, next);
      return {bp::time_period (begin, end), next + 1 - offset};
    }

  // Numeric: could be a time_duration
  bp::time_duration len = parse_time_duration (args, next);
  return {bp::time_period (begin, len), next + 3 - offset};  // time_duration consumes 3 or 4 args
}

// Convert a boost::posix_time::time_period to an Octave struct
inline octave_scalar_map
time_period_to_struct (const bp::time_period& tp)
{
  octave_scalar_map ret;
  ret.setfield ("begin", octave_value (ptime_to_struct (tp.begin ())));
  ret.setfield ("last", octave_value (ptime_to_struct (tp.last ())));
  ret.setfield ("end", octave_value (ptime_to_struct (tp.end ())));
  ret.setfield ("length", octave_value (ptime_to_struct (
    bp::ptime (bg::date (2000, 1, 1), tp.length ()))));
  return ret;
}

// Convert a boost::posix_time::time_duration to an Octave struct
inline octave_scalar_map
time_duration_to_struct (const bp::time_duration& td)
{
  octave_scalar_map ret;
  ret.setfield ("hours", octave_value (static_cast<double> (td.hours ())));
  ret.setfield ("minutes", octave_value (static_cast<double> (td.minutes ())));
  ret.setfield ("seconds", octave_value (static_cast<double> (td.seconds ())));
  ret.setfield ("fractional_seconds", octave_value (static_cast<double> (td.fractional_seconds ())));
  ret.setfield ("total_seconds", octave_value (static_cast<double> (td.total_seconds ())));
  return ret;
}

#endif
