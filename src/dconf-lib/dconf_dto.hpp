#pragma once

#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_errors.hpp"
#include "bdb_serializable.hpp"
#include "bdb_tokens.hpp"

/**
 * Interface for DTOs. Do not actually inherit because constructors
 * must call from_json() and parse(), and calling virtual functions
 * is not safe/advisable.
 */
class Dconf_DTO : public Bdb_serializable {
 public:
  virtual void from_json(json_object *jobj, Bdb_errors &errors) = 0;
  virtual void parse(int count, const std::string &line, Bdb_errors &errors) = 0;
  virtual json_object *to_json(Bdb_errors &errors) = 0;
  [[nodiscard]] virtual std::string to_string() = 0;
};

class Dconf_DTO_key : public Bdb_serializable {
 public:
  virtual void from_json(json_object *jobj, Bdb_errors &errors) = 0;
  virtual void parse(int count, const std::string &line, Bdb_errors &errors) = 0;

  virtual json_object *to_json(Bdb_errors &errors) = 0;
  [[nodiscard]] virtual std::string to_string() = 0;
};
