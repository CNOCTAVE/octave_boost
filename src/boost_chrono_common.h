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

#if !defined (octave_boost_chrono_common_h)
#define octave_boost_chrono_common_h 1

#include <octave/oct.h>
#include <boost/chrono/chrono.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#include <boost/chrono/thread_clock.hpp>
#include <string>
#include <map>

// Supported time units for Boost.Chrono duration operations
enum class ChronoUnit
{
  HOURS,
  MINUTES,
  SECONDS,
  MILLISECONDS,
  MICROSECONDS,
  NANOSECONDS
};

// Map string to ChronoUnit
static inline ChronoUnit
parse_chrono_unit (const std::string& unit_str)
{
  static std::map<std::string, ChronoUnit> unit_map =
  {
    {"hours", ChronoUnit::HOURS},
    {"hour", ChronoUnit::HOURS},
    {"h", ChronoUnit::HOURS},
    {"minutes", ChronoUnit::MINUTES},
    {"minute", ChronoUnit::MINUTES},
    {"min", ChronoUnit::MINUTES},
    {"seconds", ChronoUnit::SECONDS},
    {"second", ChronoUnit::SECONDS},
    {"s", ChronoUnit::SECONDS},
    {"milliseconds", ChronoUnit::MILLISECONDS},
    {"millisecond", ChronoUnit::MILLISECONDS},
    {"ms", ChronoUnit::MILLISECONDS},
    {"microseconds", ChronoUnit::MICROSECONDS},
    {"microsecond", ChronoUnit::MICROSECONDS},
    {"us", ChronoUnit::MICROSECONDS},
    {"nanoseconds", ChronoUnit::NANOSECONDS},
    {"nanosecond", ChronoUnit::NANOSECONDS},
    {"ns", ChronoUnit::NANOSECONDS}
  };

  std::string lower = unit_str;
  for (auto& c : lower)
    c = std::tolower (c);

  auto it = unit_map.find (lower);
  if (it == unit_map.end ())
    error ("unknown time unit: '%s'. Valid units: hours, minutes, seconds, "
           "milliseconds, microseconds, nanoseconds", unit_str.c_str ());

  return it->second;
}

// Convert a value and ChronoUnit to nanoseconds (for internal computation)
static inline boost::chrono::nanoseconds
to_nanoseconds (double value, ChronoUnit unit)
{
  switch (unit)
    {
    case ChronoUnit::HOURS:
      return boost::chrono::duration_cast<boost::chrono::nanoseconds>
               (boost::chrono::hours (static_cast<long> (value)));
    case ChronoUnit::MINUTES:
      return boost::chrono::duration_cast<boost::chrono::nanoseconds>
               (boost::chrono::minutes (static_cast<long> (value)));
    case ChronoUnit::SECONDS:
      return boost::chrono::duration_cast<boost::chrono::nanoseconds>
               (boost::chrono::seconds (static_cast<long> (value)));
    case ChronoUnit::MILLISECONDS:
      return boost::chrono::duration_cast<boost::chrono::nanoseconds>
               (boost::chrono::milliseconds (static_cast<long> (value)));
    case ChronoUnit::MICROSECONDS:
      return boost::chrono::duration_cast<boost::chrono::nanoseconds>
               (boost::chrono::microseconds (static_cast<long> (value)));
    case ChronoUnit::NANOSECONDS:
      return boost::chrono::nanoseconds (static_cast<long> (value));
    }
  return boost::chrono::nanoseconds (0);
}

// Unit string for the result of duration arithmetic
static inline const char*
chrono_unit_string (ChronoUnit unit)
{
  switch (unit)
    {
    case ChronoUnit::HOURS:        return "hours";
    case ChronoUnit::MINUTES:      return "minutes";
    case ChronoUnit::SECONDS:      return "seconds";
    case ChronoUnit::MILLISECONDS: return "milliseconds";
    case ChronoUnit::MICROSECONDS: return "microseconds";
    case ChronoUnit::NANOSECONDS:  return "nanoseconds";
    }
  return "unknown";
}

#endif
