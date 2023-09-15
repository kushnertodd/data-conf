#include <cstring>
#include "bdb_serialization.hpp"

/*!
 * @brief length of serialized C style string (null terminated)
 * @param chars_val null-terminated string to serialize
 * @return string length including null
 */
size_t Bdb_serialization::buffer_len_char_string(const char *chars_val) {
  int chars_len = (int) std::strlen(chars_val) + 1;
  return chars_len;
}

/*!
 * @brief length of serialized C++ string as C style string (null terminated)
 * @param string_val string to serialize
 * @return string length including null
 */
size_t Bdb_serialization::buffer_len_string(const std::string &string_val) {
  return buffer_len_char_string(string_val.c_str());
}

//void *Bdb_serialization::deserialize_date(time_t &date_val, void *buffer) {
//  auto *p = (unsigned char *) buffer;
//  std::memcpy(&date_val, p, sizeof(struct tm));
//  p += sizeof(long);
//  return p;
//}

/*!
 * @brief deserialize double value
 * @param double_val double value to deserialize
 */
void *Bdb_serialization::deserialize_double(double &double_val, void *buffer) {
  auto *p = (unsigned char *) buffer;
  std::memcpy(&double_val, p, sizeof(double));
  p += sizeof(double);
  return p;
}

/*!
 * @brief deserialize float value
 * @param float_val float value to deserialize
 */
void *Bdb_serialization::deserialize_float(float &float_val, void *buffer) {
  auto *p = (unsigned char *) buffer;
  std::memcpy(&float_val, p, sizeof(float));
  p += sizeof(float);
  return p;
}

/*!
 * @brief deserialize int value
 * @param int_val int value to deserialize
 */
void *Bdb_serialization::deserialize_int(int &int_val, void *buffer) {
  auto *p = (unsigned char *) buffer;
  std::memcpy(&int_val, p, sizeof(int));
  p += sizeof(int);
  return p;
}

/*!
 * @brief deserialize long value
 * @param long_val long value to deserialize
 */
void *Bdb_serialization::deserialize_long(long &long_val, void *buffer) {
  auto *p = (unsigned char *) buffer;
  std::memcpy(&long_val, p, sizeof(long));
  p += sizeof(long);
  return p;
}

/*!
 * @brief deserialize C++ string as C style string (null terminated)
 * @param string_val string to deserialize
 */
void *Bdb_serialization::deserialize_string(std::string &string_val, void *buffer) {
  auto *p = (unsigned char *) buffer;
  size_t chars_len = std::strlen((char *) p) + 1;
  string_val = std::string((const char *) p);
  p += chars_len;
  return p;
}

/*!
 * @brief serialize C style string (null terminated)
 * @param chars_val null-terminated string to serialize
 */
 void *Bdb_serialization::serialize_char_string(const char *chars_val, void *buffer) {
  auto *p = (unsigned char *) buffer;
  int chars_len = (int) std::strlen(chars_val) + 1;
  strcpy((char *) p, chars_val);
  p += chars_len;
  return p;
}

/*!
 * @brief serialize long value
 * @param long_val long value to serialize
 */
/*
void *Bdb_serialization::serialize_date(time_t &date_val, void *buffer) {
  auto *p = (unsigned char *) buffer;
  std::memcpy(p, &date_val, sizeof(struct tm));
  p += sizeof(long);
  return p;
}
*/

/*!
 * @brief serialize double value
 * @param double_val double value to serialize
 */
void *Bdb_serialization::serialize_double(const double &double_val, void *buffer) {
  auto *p = (unsigned char *) buffer;
  std::memcpy(p, &double_val, sizeof(double));
  p += sizeof(double);
  return p;
}

/*!
 * @brief serialize float value
 * @param float_val float value to serialize
 */
void *Bdb_serialization::serialize_float(const float &float_val, void *buffer) {
  auto *p = (unsigned char *) buffer;
  std::memcpy(p, &float_val, sizeof(float));
  p += sizeof(float);
  return p;
}

/*!
 * @brief serialize int value
 * @param int_val int value to serialize
 */
void *Bdb_serialization::serialize_int(const int &int_val, void *buffer)  {
  auto *p = (unsigned char *) buffer;
  std::memcpy(p, &int_val, sizeof(int));
  p += sizeof(int);
  return p;
}

/*!
 * @brief serialize long value
 * @param long_val long value to serialize
 */
void *Bdb_serialization::serialize_long(const long &long_val, void *buffer) {
  auto *p = (unsigned char *) buffer;
  std::memcpy(p, &long_val, sizeof(long));
  p += sizeof(long);
  return p;
}

/*!
 * @brief deseriserializealize C++ string as C style string (null terminated)
 * @param string_val string to serialize
 */
void *Bdb_serialization::serialize_string(const std::string &string_val, void *buffer) {
  return serialize_char_string(string_val.c_str(), buffer);
}

