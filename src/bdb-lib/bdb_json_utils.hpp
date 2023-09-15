#pragma once

#include <json-c/json.h>
#include "bdb_errors.hpp"

/*!
 * @class Bdb_json_utils json-c class wrapper
 */
class Bdb_json_utils {
 public:
  static bool check_type(const std::string &module,
                         const std::string &id,
                         json_object *jobj,
                         enum json_type expected_type,
                         Bdb_errors &errors);
  static json_object *get_json_object(const std::string &module,
                                      const std::string &id,
                                      json_object *jobj,
                                      const std::string &key,
                                      enum json_type expected_type,
                                      Bdb_errors &errors,
                                      bool optional = false);
  static std::string get_json_string(const std::string &module,
                                     const std::string &id,
                                     json_object *jobj,
                                     const std::string &key,
                                     Bdb_errors &errors,
                                     bool optional = false);
  static std::string get_json_string_type(const std::string &module,
                                          const std::string &id,
                                          json_object *jobj,
                                          Bdb_errors &errors);
};
