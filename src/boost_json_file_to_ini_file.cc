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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>

DEFUN_DLD (boost_json_file_to_ini_file, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tf} =} boost_json_file_to_ini_file (@var{src_file}, @var{dst_file})\n\
\n\
Read a JSON format file and save its content as an INI format file.\n\
Both arguments are strings specifying file paths.\n\
\n\
This function uses Boost.PropertyTree to read the JSON file into a\n\
property tree and then write it out as an INI file.\n\
\n\
Example:\n\
@example\n\
>> boost_json_file_to_ini_file (\"config.json\", \"config.ini\")\n\
@end example\n\
\n\
@seealso{boost_ini_file_to_json_file, boost_read_json, boost_write_ini}\n\
@end deftypefn")
{
  if (args.length () != 2)
    print_usage ();

  std::string src_file = args(0).string_value ();
  std::string dst_file = args(1).string_value ();

  try
    {
      boost::property_tree::ptree pt;
      boost::property_tree::read_json (src_file, pt);
      boost::property_tree::write_ini (dst_file, pt);
      return octave_value (true);
    }
  catch (const std::exception& e)
    {
      error ("boost_json_file_to_ini_file: %s", e.what ());
      return octave_value (false);
    }
}
