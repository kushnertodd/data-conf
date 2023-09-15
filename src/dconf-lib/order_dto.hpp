#pragma once

#include <json-c/json.h>
#include <list>
#include "bdb_key_extractor.hpp"
#include "bdb_errors.hpp"
#include "bdb_serializable.hpp"
#include "order_item_dto.hpp"

class Order_DTO { // use Dconf_DTO as pattern but do not inherit
 public:
  static std::string class_order() { return "Order_DTO"; }
  std::string order_id{};
  std::string account_id{};
  std::string name{};
  std::string score{};
  Order_DTO() = default;
  Order_DTO(std::string order_id_, std::string account_id_, std::string name_);
  explicit Order_DTO(void *buffer);
  Order_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void from_json(json_object *jobj, Bdb_errors &errors);
  [[nodiscard]] inline std::string get_id() const { return order_id; }
  [[nodiscard]] inline std::string get_text() const { return name; }
  static int get_order_account_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey);
  static int get_order_name(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey);
  void parse(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  void *serialize(void *buffer) const;
  json_object *to_json(Bdb_errors &errors) const;
  [[nodiscard]] std::string to_string() const;
};

class Order_DTO_key {
 public:
  std::string order_id{};
  Order_DTO_key() = default;
  explicit Order_DTO_key(const Order_DTO &order_dto);
  explicit Order_DTO_key(std::string order_id_);
  explicit Order_DTO_key(void *buffer);
  explicit Order_DTO_key(const Order_item_DTO &order_item_dto);
  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Order_DTO_account_id_key {
 public:
  std::string account_id;
  Order_DTO_account_id_key() = default;
  explicit Order_DTO_account_id_key(std::string account_id_) :
      account_id(std::move(account_id_)) {}
  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Order_DTO_name_key {
 public:
  std::string name;
  Order_DTO_name_key() = default;
  explicit Order_DTO_name_key(std::string name_) :
      name(std::move(name_)) {}
  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Order_DTO_list {
 public:
  static std::string class_order() { return "Order_DTO_list"; }
  std::list<Order_DTO> list;
  void add(Order_DTO &order_dto) { list.push_back(order_dto); }
  json_object *to_json(Bdb_errors &errors) const;
};

class Order_DTO_key_list {
 public:
  std::list<Order_DTO_key> list;
  void add(Order_DTO_key &order_dto_key) { list.push_back(order_dto_key); }
};
