#include <json-c/json.h>
#include <sstream>
#include "bdb_common.hpp"
#include "bdb_json_utils.hpp"
#include "dconf_request.hpp"

Dconf_request::Dconf_request(const std::string &request, Bdb_errors &errors) {
  request_json = json_tokener_parse(request.c_str());
  if (request_json == nullptr)
    errors.add("Dconf_request_handler::Dconf_request",
               "1",
               "invalid request json: " + request, errno);
  else {
    std::string request_class_name =
        Bdb_json_utils::get_json_string("Dconf_request_handler::Dconf_request", "1",
                                        request_json, "class_name",
                                        errors);
    if (!errors.has())
    if (request_class_name != Dconf_request::class_name())
      errors.add("Dconf_request_handler::Dconf_request", "2", "invalid request class: "
          + request_class_name);
  }
  if (!errors.has())
    from_json(request_json, errors);
}

void Dconf_request::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_name": ... `
  std::string jobj_class_name =
      Bdb_json_utils::get_json_string("Dconf_request::from_json",
                                      "1", jobj, "class_name", errors);
  if (!errors.has() && jobj_class_name != class_name())
    errors.add("Dconf_request::from_json", "2", "not class dconf_request");
  // parse: request": ...
  request = Bdb_json_utils::get_json_string("Dconf_request::from_json",
                                            "3", jobj, "request", errors);
  if (!errors.has()) {
    // parse: ' { "arguments": ... `
    json_object *arguments_json =
        Bdb_json_utils::get_json_object("Dconf_request::from_json",
                                        "4", jobj, "arguments", json_type_array,
                                        errors);
    if (!errors.has()) {
      size_t n_args = json_object_array_length(arguments_json);
      for (size_t i = 0; i < n_args && !errors.has(); i++) {
        json_object *argument_json = json_object_array_get_idx(arguments_json, i);
        std::string
            argument = Bdb_json_utils::get_json_string_type("Dconf_request::from_json",
                                                            "5", argument_json, errors);
        if (!errors.has())
          arguments.push_back(argument);
      }
    }
  }
}

json_object *Dconf_request::to_json(Bdb_errors &errors) {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Dconf_request::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_name",
                         json_object_new_string(class_name().c_str()));
  json_object_object_add(root, "request",
                         json_object_new_string(request.c_str()));

  json_object *arguments_json = json_object_new_array();
  json_object_object_add(root, "arguments", arguments_json);
  for (const auto &argument: arguments) {
    json_object *argument_json = json_object_new_string(argument.c_str());
    json_object_array_add(arguments_json, argument_json);
  }
  return root;
}

std::string Dconf_request::to_string() const {
  std::ostringstream os;
  os << "dconf_request:   " << std::endl;
  os << "request        " << request << std::endl;
  os << "arguments       " << std::endl;
  for (const auto &argument: arguments)
    os << "\t\t    " << argument << std::endl;
  return os.str();
}

// Dconf_request_response methods

Dconf_request_response::Dconf_request_response(Bdb_errors &errors) {
  jobj = json_object_new_object();
  if (!jobj) {
    errors.add("Dconf_request_response::to_json", "1", "json-c allocate error");
  } else {
    json_object_object_add(jobj, "class_name",
                           json_object_new_string(class_name().c_str()));
  }
}

void Dconf_request_response::add_array() {
  jobj_array = json_object_new_array();
  json_object_object_add(jobj, "dconf_request_responses", jobj_array);
}

void Dconf_request_response::add_request(json_object *request_json) const {
  json_object_object_add(jobj, "dconf_request", request_json);
}

void Dconf_request_response::add_response(json_object *response_json) const {
  json_object_object_add(jobj, "dconf_request_response", response_json);
}

void Dconf_request_response::add_response_array(json_object *response_json) const {
  json_object_array_add(jobj_array, response_json);
}

void Dconf_request_response::cleanup() const {
  json_object_put(jobj);
}

json_object *Dconf_request_response::to_load_response(int count, Bdb_errors &errors) {
  json_object *load_response_json = json_object_new_object();
  if (!load_response_json) {
    errors.add("Dconf_request::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(load_response_json, "class_name",
                         json_object_new_string(class_name().c_str()));
  json_object_object_add(load_response_json, "record_count",
                         json_object_new_string(std::to_string(count).c_str()));
  return load_response_json;
}

std::string Dconf_request_response::to_string() const {
  return json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PRETTY);
}
