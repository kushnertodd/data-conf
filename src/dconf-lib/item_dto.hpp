#pragma once

#include <json-c/json.h>
#include <list>
#include <utility>
#include "bdb_key_extractor.hpp"
#include "bdb_errors.hpp"
#include "bdb_serializable.hpp"
#include "order_item_dto.hpp"

class Item_DTO { // use Dconf_DTO as pattern but do not inherit
 public:
  static std::string class_item() { return "Item_DTO"; }
  std::string item_id{};
  std::string name{};
  std::string cost{};
  std::string score{};
  Item_DTO() = default;
  Item_DTO(std::string item_id_,
           std::string name_,
           std::string cost_);
  explicit Item_DTO(void *buffer);
  Item_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void from_json(json_object *jobj, Bdb_errors &errors);
  static int get_item_name(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey);
  static int get_item_cost(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey);
  [[nodiscard]] inline std::string get_id() const { return item_id; }
  [[nodiscard]] inline std::string get_text() const { return name; }
  void parse(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  void *serialize(void *buffer) const;
  json_object *to_json(Bdb_errors &errors) const;
  [[nodiscard]] std::string to_string() const;
};

class Item_DTO_key {
 public:
  std::string item_id{};
  Item_DTO_key() = default;
  explicit Item_DTO_key(const Item_DTO &item_dto);
  explicit Item_DTO_key(std::string item_id_);
  explicit Item_DTO_key(void *buffer);
  explicit Item_DTO_key(const Order_item_DTO &order_item_dto);
  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Item_DTO_name_key {
 public:
  std::string name;
  Item_DTO_name_key() = default;
  explicit Item_DTO_name_key(std::string name_) :
      name(std::move(name_)) {}
  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Item_DTO_cost_key {
 public:
  std::string cost;
  Item_DTO_cost_key() = default;
  explicit Item_DTO_cost_key(std::string cost_) :
      cost(std::move(cost_)) {}
  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Item_DTO_list {
 public:
  static std::string class_item() { return "Item_DTO_list"; }
  std::list<Item_DTO> list;
  void add(Item_DTO &item_dto) { list.push_back(item_dto); }
  json_object *to_json(Bdb_errors &errors) const;
};

class Item_DTO_key_list {
 public:
  std::list<Item_DTO_key> list;
  void add(Item_DTO_key &item_dto_key) { list.push_back(item_dto_key); }
};
