#include <sstream>
#include "bdb_json_utils.hpp"

/*!
 * @brief verify expected json-c object type
 * @return object type match
 */
bool Bdb_json_utils::check_type(const std::string &module,
                            const std::string &id,
                            json_object *jobj,
                            enum json_type expected_type,
                            Bdb_errors &errors) {
  enum json_type actual_type = json_object_get_type(jobj);
  if (expected_type != actual_type) {
    std::ostringstream os;
    os << "expected type " << json_type_to_name(expected_type)
       << " not " << json_type_to_name(actual_type);
    errors.add(module, id, os.str());
    return false;
  }
  return true;
}

/*!
 * @brief return json-c object at json key with expected type
 * @return json object handle
 */
json_object *Bdb_json_utils::get_json_object(const std::string &module,
                                         const std::string &id,
                                         json_object *jobj,
                                         const std::string &key,
                                         enum json_type expected_type,
                                         Bdb_errors &errors,
                                         bool optional) {
  json_object *jobj_from_key = json_object_object_get(jobj, key.c_str());
  if (jobj_from_key == nullptr) {
    if (!optional) {
      std::string message = "missing expected key " + key;
      errors.add(module, id, message);
    }
  } else {
    if (!check_type(module, id, jobj_from_key, expected_type, errors)) {
      return nullptr;
    }
  }
  return jobj_from_key;
}

/*!
 * @brief return json-c string at json key
 * @return json object string value
 * @details any optional field will treat a value of "" as the field being missing
 */
std::string Bdb_json_utils::get_json_string(const std::string &module,
                                        const std::string &id,
                                        json_object *jobj,
                                        const std::string &key,
                                        Bdb_errors &errors,
                                        bool optional) {
  json_object *jobj_string = get_json_object(module, id, jobj, key,
                                             json_type_string, errors, optional);
  if (!errors.has() && jobj_string != nullptr)
    return json_object_get_string(jobj_string);
  else
    return "";
}

/*!
 * @brief return json-c object string if string type
 * @return json object string value or "" if not string type
 */
std::string Bdb_json_utils::get_json_string_type(const std::string &module,
                                             const std::string &id,
                                             json_object *jobj,
                                             Bdb_errors &errors) {

  if (check_type(module, id, jobj, json_type_string, errors))
    return json_object_get_string(jobj);
  else
    return "";
}
