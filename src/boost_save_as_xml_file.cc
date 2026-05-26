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
#include <boost/property_tree/xml_parser.hpp>
#include <sstream>

// Helper: convert an Octave value to a ptree
static void
octave_value_to_ptree (const octave_value& ov,
                       boost::property_tree::ptree& pt,
                       const std::string& key = "")
{
  if (ov.isstruct ())
    {
      const octave_scalar_map m = ov.scalar_map_value ();
      string_vector fields = m.fieldnames ();
      for (octave_idx_type i = 0; i < fields.numel (); ++i)
        {
          std::string fname = fields(i);
          octave_value val = m.contents (fname);
          boost::property_tree::ptree child;
          octave_value_to_ptree (val, child, fname);
          pt.add_child (fname, child);
        }
    }
  else if (ov.iscell ())
    {
      const Cell c = ov.cell_value ();
      for (octave_idx_type i = 0; i < c.numel (); ++i)
        {
          boost::property_tree::ptree child;
          octave_value_to_ptree (c.elem (i), child);
          pt.push_back (std::make_pair ("", child));
        }
    }
  // is_string() check must come BEFORE is_matrix_type()/isnumeric() check
  // to prevent "invalid conversion from string to real N-D array" errors
  // when saving struct data that was parsed from JSON.
  else if (ov.is_string ())
    {
      std::string s = ov.string_value ();
      pt.put_value (s);
    }
  else if (ov.is_matrix_type () || ov.isnumeric ())
    {
      NDArray arr = ov.array_value ();
      if (arr.numel () == 1)
        {
          std::ostringstream oss;
          double d = arr(0);
          if (d == std::floor (d) && !ov.iscomplex ())
            oss << static_cast<long long> (d);
          else
            oss << d;
          pt.put_value (oss.str ());
        }
      else
        {
          std::ostringstream oss;
          for (octave_idx_type i = 0; i < arr.numel (); ++i)
            {
              if (i > 0) oss << " ";
              double d = arr(i);
              if (d == std::floor (d) && !ov.iscomplex ())
                oss << static_cast<long long> (d);
              else
                oss << d;
            }
          pt.put_value (oss.str ());
        }
    }
  else
    {
      std::string s = ov.string_value ();
      pt.put_value (s);
    }
}

DEFUN_DLD (boost_save_as_xml_file, args, nargout,
           "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{tf} =} boost_save_as_xml_file (@var{data}, @var{filename})\n\
\n\
Save Octave data (struct, numeric, string, or cell array) as an XML format file.\n\
The data is converted to a Boost.PropertyTree and written to the file.\n\
\n\
Example:\n\
@example\n\
>> data = struct ('name', \"Alice\", 'age', 30);\n\
>> boost_save_as_xml_file (data, \"data.xml\")\n\
@end example\n\
\n\
@seealso{boost_parse_from_xml_file, boost_save_as_info_file, boost_save_as_ini_file, boost_save_as_json_file}\n\
@end deftypefn")
{
  if (args.length () != 2)
    print_usage ();

  octave_value data = args(0);
  std::string filename = args(1).string_value ();

  try
    {
      boost::property_tree::ptree pt;
      octave_value_to_ptree (data, pt);
      boost::property_tree::write_xml (filename, pt);
      return octave_value (true);
    }
  catch (const std::exception& e)
    {
      error ("boost_save_as_xml_file: %s", e.what ());
      return octave_value (false);
    }
}
