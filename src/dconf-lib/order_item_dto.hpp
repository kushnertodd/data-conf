#pragma once

#include <json-c/json.h>
#include <list>
#include "bdb_key_extractor.hpp"
#include "bdb_errors.hpp"
#include "bdb_serializable.hpp"

class Order_item_DTO { // use Dconf_DTO as pattern but do not inherit
 public:
  static std::string class_order_item() { return "Order_item_DTO"; }
  std::string order_item_id{};
  std::string order_id{};
  std::string item_id{};
  Order_item_DTO() = default;
  Order_item_DTO(std::string order_id_, std::string item_id_);
  Order_item_DTO(Order_item_DTO order_item_DTO, std::string &order_item_id_);
  explicit Order_item_DTO(void *buffer);
  Order_item_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  [[nodiscard]] size_t buffer_size() const;
  void create(const Order_item_DTO &order_item_dto, const std::string &order_item_id_);
  void *deserialize(void *buffer);
  void from_json(json_object *jobj, Bdb_errors &errors);
  static int get_order_item_order_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey);
  static int get_order_item_item_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey);
  [[nodiscard]] inline std::string get_id() const { return order_item_id; }
  void parse(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  void *serialize(void *buffer) const;
  json_object *to_json(Bdb_errors &errors) const;
  [[nodiscard]] std::string to_string() const;
};

class Order_item_DTO_key {
 public:
  std::string order_item_id{};
  Order_item_DTO_key() = default;
  explicit Order_item_DTO_key(const Order_item_DTO &order_item_dto);
  explicit Order_item_DTO_key(std::string order_item_id_);
  explicit Order_item_DTO_key(void *buffer);
  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Order_item_DTO_list {
 public:
  static std::string class_order_item() { return "Order_item_DTO_list"; }
  std::list<Order_item_DTO> list;
  void add(Order_item_DTO &order_item_dto) { list.push_back(order_item_dto); }
  json_object *to_json(Bdb_errors &errors) const;
};

class Order_item_DTO_key_list {
 public:
  std::list<Order_item_DTO_key> list;
  void add(Order_item_DTO_key &order_item_dto_key) { list.push_back(order_item_dto_key); }
};
