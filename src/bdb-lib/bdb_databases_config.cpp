#include "bdb_json_utils.hpp"
#include "bdb_databases_config.hpp"

// Secondary_database_config methods


/*!
 * @brief secondary database configuration move constructor, including name, filename, and key extractor function name
 * @param other copied object
 */
Secondary_database_config::Secondary_database_config(Secondary_database_config &&other) noexcept:
    name(std::move(other.name)),
    filename(std::move(other.filename)),
    key_extractor(std::move(other.key_extractor)) {
}

/*!
 * @brief construct secondary database configuration from json file
 * @param jobj json file object handle
 * @param errors unexpected configuration format
 */
Secondary_database_config::Secondary_database_config(json_object *jobj, Bdb_errors &errors) {
  from_json(jobj, errors);
}

/*!
 * @brief secondary database assignment
 * @param other copied object
 * @return object reference
 */
Secondary_database_config &Secondary_database_config::operator=(const Secondary_database_config &other) {
  if (this != &other) {
    name = other.name;
    filename = other.filename;
    key_extractor = other.key_extractor;
  }
  return *this;
}

/*!
 * @brief secondary database move configuration assignment
 * @param other copied object
 * @return object reference
 */
Secondary_database_config &Secondary_database_config::operator=(Secondary_database_config &&other) noexcept {
  if (this != &other) {
    name = std::move(other.name);
    filename = std::move(other.filename);
    key_extractor = std::move(other.key_extractor);
  }
  return *this;
}

/*!
 * @brief create secondary database configuration from json file
 * @param jobj json file object handle
 * @param errors unexpected configuration format
 */
void Secondary_database_config::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_name": ... `
  std::string jobj_class_name =
      Bdb_json_utils::get_json_string("Secondary_database_config::from_json", "1", jobj, "class_name", errors);
  if (!errors.has()) {
    if (jobj_class_name != class_name())
      errors.add("Secondary_database_config::from_json", "2", "not class Secondary_database_config");
  }
  // parse: ' { "name": ... `
  if (!errors.has())
    name = Bdb_json_utils::get_json_string("Secondary_database_config::from_json", "3", jobj, "name", errors);
  // parse: ' { "filename": ... `
  if (!errors.has())
    filename = Bdb_json_utils::get_json_string("Secondary_database_config::from_json", "4", jobj, "filename", errors);
  // parse: ' { "key_extractor": ... `
  if (!errors.has())
    key_extractor =
        Bdb_json_utils::get_json_string("Secondary_database_config::from_json", "5", jobj, "key_extractor", errors);
}

/*!
 * @brief format secondary database filename in database home directory
 * @return secondary database filename
 */
std::string Secondary_database_config::get_filename(const std::string &db_home) const {
  return db_home + "/" + filename;
}

/*!
 * @brief convert secondary database configuration to json
 * @param errors unable to allocate json handle
 * @return json handle
 */
json_object *Secondary_database_config::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Secondary_database_config::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_name", json_object_new_string(class_name().c_str()));
  json_object_object_add(root, "name", json_object_new_string(name.c_str()));
  json_object_object_add(root, "filename", json_object_new_string(filename.c_str()));
  json_object_object_add(root, "key_extractor", json_object_new_string(key_extractor.c_str()));
  return root;
}

// Primary_database_config methods

/*!
 * @brief primary database configuration move constructor, including name, filename, and key extractor function name
 * @param other copied object
 */

Primary_database_config::Primary_database_config(Primary_database_config &&other) noexcept:
    name(std::move(other.name)),
    filename(std::move(other.filename)) {
}

/*!
 * @brief construct primary database configuration from json file
 * @param jobj json file object handle
 * @param errors unexpected configuration format
 */
Primary_database_config::Primary_database_config(json_object *jobj, Bdb_errors &errors) {
  from_json(jobj, errors);
}

/*!
 * @brief primary database assignment
 * @param other copied object
 * @return object reference
 */
Primary_database_config &Primary_database_config::operator=(Primary_database_config &&other) noexcept {
  if (this != &other) {
    name = std::move(other.name);
    filename = std::move(other.filename);
  }
  return *this;
}

/*!
 * @brief primary database move configuration assignment
 * @param other copied object
 * @return object reference
 * @details https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/
 */
void Primary_database_config::add(const Secondary_database_config &secondary_database_config) {
  secondary_database_config_map[secondary_database_config.name] = secondary_database_config;
}

/*!
 * @brief create primary database configuration from json file
 * @param jobj json file object handle
 * @param errors unexpected configuration format
 */
void Primary_database_config::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_name": ... `
  std::string jobj_class_name =
      Bdb_json_utils::get_json_string("Primary_database_config::from_json", "1", jobj, "class_name", errors);
  if (!errors.has()) {
    if (jobj_class_name != class_name())
      errors.add("Primary_database_config::from_json", "2", "not class Primary_database_config");
  }
  // parse: ' { "name": ... `
  if (!errors.has())
    name = Bdb_json_utils::get_json_string("Primary_database_config::from_json", "3", jobj, "name", errors);
  // parse: ' { "filename": ... `
  if (!errors.has())
    filename = Bdb_json_utils::get_json_string("Primary_database_config::from_json", "4", jobj, "filename", errors);
  // parse: ' { "has_duplicates": ... `
  std::string has_duplicates_str;
  if (!errors.has())
    has_duplicates_str =
        Bdb_json_utils::get_json_string("Primary_database_config::from_json",
                                        "5",
                                        jobj,
                                        "has_duplicates",
                                        errors,
                                        true);
  if (!errors.has() && has_duplicates_str == "true")
    has_duplicates = true;
  // parse: ' { "variables": ... '
  json_object *variables_json = nullptr;
  if (!errors.has())
    variables_json =
        Bdb_json_utils::get_json_object("Primary_database_config::from_json",
                                        "1",
                                        jobj,
                                        "variables",
                                        json_type_array,
                                        errors,
                                        true);
  // parse: ' { "secondary_databases": ... `
  json_object *secondary_databases_json = nullptr;
  if (!errors.has())
    secondary_databases_json =
        Bdb_json_utils::get_json_object("Primary_database_config::from_json",
                                        "1",
                                        jobj,
                                        "secondary_databases",
                                        json_type_array,
                                        errors,
                                        true);
  if (!errors.has() && secondary_databases_json != nullptr) {
    size_t n_dbs = json_object_array_length(secondary_databases_json);
    for (size_t i = 0; i < n_dbs && !errors.has(); i++) {
      json_object *secondary_database_json = json_object_array_get_idx(secondary_databases_json, i);
      Secondary_database_config secondary_database_config(secondary_database_json, errors);
      if (!errors.has())
        add(secondary_database_config);
    }
  }
}

/*!
 * @brief format primary database filename in database home directory
 * @return primary database filename
 */
std::string Primary_database_config::get_filename(const std::string &db_home) const {
  return db_home + "/" + filename;
}

/*!
 * @brief select secondary database configuration
 * @param db_name secondary database name
 * @param secondary_database_config secondary database configuration
 * @param errors secondary database name not found
 */
void Primary_database_config::select(const std::string &db_name,
                                     Secondary_database_config &secondary_database_config,
                                     Bdb_errors &errors) {
  for (const auto &it: secondary_database_config_map)
    if (it.first == db_name) {
      secondary_database_config = it.second;
      return;
    }
  errors.add("Primary_database_config::select", "1", "secondary database " + db_name + "not found");
}

/*!
 * @brief convert primary database configuration to json
 * @param errors unable to allocate json handle
 * @return json handle
 */
json_object *Primary_database_config::to_json(Bdb_errors &errors) {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Primary_database_config::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_name", json_object_new_string(class_name().c_str()));
  json_object_object_add(root, "name", json_object_new_string(name.c_str()));
  json_object_object_add(root, "filename", json_object_new_string(filename.c_str()));
  if (!errors.has() && !secondary_database_config_map.empty()) {
    json_object *secondary_databases_json = json_object_new_array();
    json_object_object_add(root, "secondary_databases", secondary_databases_json);
    for (const auto &[db_name, secondary_database_config]: secondary_database_config_map) {
      json_object *secondary_database_json = secondary_database_config.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(secondary_databases_json, secondary_database_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}

// Bdb_databases_config methods

/*!
 * @brief database configurations constructor
 * @param jobj json handle
 * @param errors unexpected configuration format
 */
Bdb_databases_config::Bdb_databases_config(json_object *jobj, Bdb_errors &errors) {
  from_json(jobj, errors);
}

/*!
 * @brief create database configuration from json file
 * @param jobj json file object handle
 * @param errors unexpected configuration format
 */
void Bdb_databases_config::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_name": ... `
  std::string jobj_class_name =
      Bdb_json_utils::get_json_string("Bdb_databases_config::from_json", "1", jobj,
                                      "class_name", errors);
  if (!errors.has()) {
    if (jobj_class_name != class_name())
      errors.add("Bdb_databases_config::from_json", "2", "not class Bdb_databases_config");
  }
  json_object *primary_databases_json;
  if (!errors.has())
    // parse: ' { "bdb_databases_config": ... `
    primary_databases_json =
        Bdb_json_utils::get_json_object("Bdb_databases_config::from_json",
                                        "1",
                                        jobj,
                                        "primary_databases",
                                        json_type_array,
                                        errors);
  if (!errors.has()) {
    size_t n_dbs = json_object_array_length(primary_databases_json);
    for (size_t i = 0; i < n_dbs && !errors.has(); i++) {
      json_object *database_json = json_object_array_get_idx(primary_databases_json, i);
      Primary_database_config primary_database_config(database_json, errors);
      if (!errors.has())
        primary_database_config_map[primary_database_config.name] = primary_database_config;
    }
  }
}

/*!
 * @brief select primary database configuration
 * @param db_name primary database name
 * @param secondary_database_config primary database configuration
 * @param errors primary database name not found
 */
void Bdb_databases_config::select(const std::string &db_name,
                                  Primary_database_config &primary_database_config,
                                  Bdb_errors &errors) {
  for (const auto &it: primary_database_config_map)
    if (it.first == db_name) {
      primary_database_config = it.second;
      return;
    }
  errors.add("", "1", "primary database config name " + db_name + " not found");
}

/*!
 * @brief convert database configuration to json
 * @param errors unable to allocate json handle
 * @return json handle
 */
json_object *Bdb_databases_config::to_json(Bdb_errors &errors) {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Bdb_databases_config::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  if (!errors.has()) {
    json_object_object_add(root, "class_name", json_object_new_string(Bdb_databases_config::class_name().c_str()));
    json_object *primary_databases_json = json_object_new_array();
    json_object_object_add(root, "primary_databases", primary_databases_json);
    for (auto &[db_name, primary_database_config]: primary_database_config_map) {
      json_object *primary_database_json = primary_database_config.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(primary_databases_json, primary_database_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
