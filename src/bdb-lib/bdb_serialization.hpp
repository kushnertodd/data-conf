#pragma once

#include <string>

/*!
 * @brief DTO serialization helper routines
 */
class Bdb_serialization {
 public:

  static size_t buffer_len_char_string(const char *chars_val);
  static size_t buffer_len_string(const std::string &string_val);

 // static void *deserialize_date(time_t &date_val, void *buffer);
  static void *deserialize_double(double &double_val, void *buffer);
  static void *deserialize_float(float &float_val, void *buffer);
  static void *deserialize_int(int &int_val, void *buffer);
  static void *deserialize_long(long &long_val, void *buffer);
  static void *deserialize_string(std::string &string_val, void *buffer);

  static void *serialize_char_string(const char *chars_val, void *buffer);
  //static void *serialize_date(time_t &date_val, void *buffer);
  static void *serialize_double(const double &double_val, void *buffer);
  static void *serialize_float(const float &float_val, void *buffer);
  static void *serialize_int(const int &int_val, void *buffer);
  static void *serialize_long(const long &long_val, void *buffer);
  static void *serialize_string(const std::string &string_val, void *buffer);
};
