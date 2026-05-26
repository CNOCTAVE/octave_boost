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
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

DEFUN_DLD (boost_info_file_to_xml_file, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tf} =} boost_info_file_to_xml_file (@var{src_file}, @var{dst_file})\n\
\n\
Read an INFO format file and save its content as an XML format file.\n\
Both arguments are strings specifying file paths.\n\
\n\
This function uses Boost.PropertyTree to read the INFO file into a\n\
property tree and then write it out as an XML file.\n\
\n\
Example:\n\
@example\n\
>> boost_info_file_to_xml_file (\"config.info\", \"config.xml\")\n\
@end example\n\
\n\
@seealso{boost_xml_file_to_info_file, boost_read_info, boost_write_xml}\n\
@end deftypefn")
{
  if (args.length () != 2)
    print_usage ();

  std::string src_file = args(0).string_value ();
  std::string dst_file = args(1).string_value ();

  try
    {
      boost::property_tree::ptree pt;
      boost::property_tree::read_info (src_file, pt);
      boost::property_tree::write_xml (dst_file, pt);
      return octave_value (true);
    }
  catch (const std::exception& e)
    {
      error ("boost_info_file_to_xml_file: %s", e.what ());
      return octave_value (false);
    }
}
