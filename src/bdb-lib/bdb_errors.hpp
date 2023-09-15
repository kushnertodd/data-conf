#pragma once

#include <db_cxx.h>
#include <json-c/json.h>
#include <vector>

/*!
 * @class Bdb_error Error module
 * Includes calling module, message id, and message text
 */
class Bdb_error {
 public:
  std::string module_name{};
  std::string id{};
  std::string message{};
  int db_errno{};

  Bdb_error() = default;
  Bdb_error(std::string module_name_, std::string id_, std::string message_, int db_errno_ = 0);
  static std::string class_name() { return "Bdb_error"; }
  [[nodiscard]] bool is_db_err() const;
  [[nodiscard]] json_object *to_json() const;
  [[nodiscard]] std::string to_string() const;
};

class Bdb_errors {
 public:
  int error_ct = 0;
  std::vector<Bdb_error> errors;

  Bdb_errors() = default;
  void add(const std::string &module_name,
           const std::string &id,
           const std::string &message,
           int db_errno = 0
  );
  void check_exit(const std::string &message = "");
  static std::string class_name() { return "Bdb_errors"; }

  void clear();
  [[nodiscard]] bool has() const;
  [[nodiscard]] json_object *to_json() const;
  [[nodiscard]] std::string to_json_string() const;
  std::string to_string();
};

class Bdb_error_exception : public std::exception {
 private:
  const std::string bdb_error_str;
 public:
  Bdb_error_exception(const std::string &module_name,
                      const std::string &id,
                      const std::string &message);
  char *what();
};
