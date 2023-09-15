#include "bdb_errors.hpp"
#include "misc_utils.hpp"

/*!
 * determine character for Ascii code
 * @param n to convert
 * @return C char
 */
char Misc_utils::ascii_to_char(int n) {
  return char(n);
}

/*!
 * convert C char to string
 * @param c to convert
 * @return c++ string
 */
std::string Misc_utils::char_to_string(char c) {
  std::string s(1, c);
  return s;
}

int Misc_utils::string_to_int(const std::string &int_str, Bdb_errors &errors) {
  try {
    int i = std::stoi(int_str);
    return i;
  }
  catch (std::exception &err) {
    errors.add("Misc_utils::string_to_int",
               "1",
               "invalid int '" + std::string(int_str) + "': " + std::string(err.what()));
  }
  return 0;
}

double Misc_utils::string_to_real(const std::string &real_str, Bdb_errors &errors) {
  try {
    double d = std::stod(real_str);
    return d;
  }
  catch (std::exception &err) {
    errors.add("Misc_utils::string_to_real",
               "1",
               "invalid real '" + std::string(real_str) + "': " + std::string(err.what()));
  }
  return 0;
}
