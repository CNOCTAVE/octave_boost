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
#include <sstream>

// Helper: convert a ptree to an Octave value
// Returns a cell array if the node is an array (children with empty keys),
// a struct if the node has named children, or a string leaf value.
static octave_value
ptree_to_octave_value (const boost::property_tree::ptree& pt)
{
  if (pt.empty ())
    {
      // Leaf node: return its value as a string
      return octave_value (pt.data ());
    }

  // Check if this node represents an array (all children have empty keys)
  bool is_array = true;
  for (const auto& child : pt)
    {
      if (! child.first.empty ())
        {
          is_array = false;
          break;
        }
    }

  if (is_array)
    {
      // Build a cell array for arrays (children have empty keys)
      size_t n = 0;
      for (const auto& child : pt)
        ++n;

      Cell c (n, 1);
      size_t idx = 0;
      for (const auto& child : pt)
        {
          const boost::property_tree::ptree& child_pt = child.second;
          if (child_pt.empty ())
            c(idx++) = octave_value (child_pt.data ());
          else
            c(idx++) = ptree_to_octave_value (child_pt);
        }
      return octave_value (c);
    }

  // Internal node: build a struct from named children
  octave_scalar_map m;
  for (const auto& child : pt)
    {
      const std::string& key = child.first;
      const boost::property_tree::ptree& child_pt = child.second;

      if (child_pt.empty ())
        {
          // Leaf child
          m.setfield (key, octave_value (child_pt.data ()));
        }
      else
        {
          // Non-leaf child: recurse
          m.setfield (key, ptree_to_octave_value (child_pt));
        }
    }
  return octave_value (m);
}

DEFUN_DLD (boost_parse_from_info_file, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{data} =} boost_parse_from_info_file (@var{filename})\n\
\n\
Read an INFO format file and parse its content into an Octave data structure.\n\
The file is read using Boost.PropertyTree and converted to an Octave struct.\n\
\n\
Example:\n\
@example\n\
>> data = boost_parse_from_info_file (\"data.info\")\n\
@end example\n\
\n\
@seealso{boost_save_as_info_file, boost_parse_from_ini_file, boost_parse_from_json_file, boost_parse_from_xml_file}\n\
@end deftypefn")
{
  if (args.length () != 1)
    print_usage ();

  std::string filename = args(0).string_value ();

  try
    {
      boost::property_tree::ptree pt;
      boost::property_tree::read_info (filename, pt);
      return ptree_to_octave_value (pt);
    }
  catch (const std::exception& e)
    {
      error ("boost_parse_from_info_file: %s", e.what ());
      return octave_value ();
    }
}
