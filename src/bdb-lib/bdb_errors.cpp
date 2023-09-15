#include <iostream>
#include <sstream>
#include <utility>
#include "bdb_errors.hpp"

// Error methods

/*!
 * adds error to error list
 * @param module module in which error occurred
 * @param id message id within module
 * @param message descriptive message
 * @param db_errno return from Berkeley DB routine, <0 = db error, >0 = error, 0 = normal
 */
Bdb_error::Bdb_error(std::string module_, std::string id_, std::string message_, int db_errno_) :
    module_name(std::move(module_)),
    id(std::move(id_)),
    message(std::move(message_)),
    db_errno(db_errno_) {}

bool Bdb_error::is_db_err() const {
  return db_errno < 0; // else is errno if > 0
}

json_object *Bdb_error::to_json() const {
  json_object *root = json_object_new_object();
  if (!root) {
    return nullptr;
  }
  json_object_object_add(root, "module", json_object_new_string(module_name.c_str()));
  json_object_object_add(root, "id", json_object_new_string(id.c_str()));
  json_object_object_add(root, "message", json_object_new_string(message.c_str()));
  if (db_errno)
    json_object_object_add(root, "db_errno", json_object_new_string(db_strerror(db_errno)));
  return root;
}

/*!
 * Formatted error message
 * @return "module: [id] message (Bdb or system errno message)"
 */
std::string Bdb_error::to_string() const {
  std::ostringstream os;
  if (module_name.empty() && id.empty())
    os << message;
  else if (id.empty())
    os << module_name + ": " + message;
  else
    os << module_name + "[" + id + "]: " + message;
  if (db_errno)
    os << " (" << db_strerror(db_errno) << ")";
  return os.str();
}

// Bdb_errors methods

/*!
 * adds error to error list
 * @param module module in which error occurred
 * @param id message id within module
 * @param message descriptive message
 */
void Bdb_errors::add(const std::string &module_name,
                     const std::string &id,
                     const std::string &message,
                     int db_errno) {
  error_ct++;
  errors.emplace_back(module_name, id, message, db_errno);
}

/*!
 * checks for error and exits
 * @param message
 */
void Bdb_errors::check_exit(const std::string &message) {
  if (error_ct > 0) {
    if (!message.empty())
      std::cerr << message << std::endl;
    std::cerr << to_string() << std::endl;
    exit(1);
  }
}

void Bdb_errors::clear() {
  error_ct = 0;
  errors.clear();
}

/*!
 * checks whether error has been seen
 * @return non-zero error count
 */
bool Bdb_errors::has() const {
  return error_ct != 0;
}

/*!
 * json formatted error messages
 * @return json-c object handle
 */
json_object *Bdb_errors::to_json() const {
  json_object *root = json_object_new_object();
  if (!root) {
    return nullptr;
  }
  if (!has()) {
    json_object_object_add(root, "class_name", json_object_new_string(Bdb_errors::class_name().c_str()));
    json_object *names_json = json_object_new_array();
    json_object_object_add(root, "errors", names_json);
    for (const Bdb_error &error: errors) {
      json_object *error_jobj = error.to_json();
      if (error_jobj != nullptr)
        json_object_array_add(names_json, error_jobj);
    }
  }
  if (!has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}

std::string Bdb_errors::to_json_string() const {
  return json_object_to_json_string_ext(to_json(),
                                        JSON_C_TO_STRING_PRETTY);
}

/*!
 * format error list
 * @return text containing errors one per line
 */
std::string Bdb_errors::to_string() {
  std::ostringstream os;
  for (const Bdb_error &error: errors) {
    os << error.to_string() << std::endl;
  }
  return os.str();
}

/*!
 * @brief error exception
 * @param module module in which error occurred
 * @param id message id within module
 * @param message descriptive message
 */
Bdb_error_exception::Bdb_error_exception(const std::string &module_name,
                                         const std::string &id,
                                         const std::string &message) :
    bdb_error_str((char *) Bdb_error(module_name, id, message).to_string().c_str()) {}

char *Bdb_error_exception::what() {
  return (char *) bdb_error_str.c_str();
}
