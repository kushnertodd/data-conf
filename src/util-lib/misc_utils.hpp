#pragma once

class Misc_utils {
 public:
  static char ascii_to_char(int n);
  static std::string char_to_string(char c);
  static int string_to_int(const std::string &int_str, Bdb_errors &errors);
  static double string_to_real(const std::string &real_str, Bdb_errors &errors);
};

