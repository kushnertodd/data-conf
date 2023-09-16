#include <cstring>
#include <sstream>
#include <utility>
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "order_dto.hpp"

Order_DTO::Order_DTO(std::string order_id_,
                     std::string account_id_,
                     std::string name_) :
    order_id(std::move(order_id_)),
    account_id(std::move(account_id_)),
    name(std::move(name_)) {}

Order_DTO::Order_DTO(std::string account_id_,
                     std::string name_) :
    account_id(std::move(account_id_)),
    name(std::move(name_)) {}

Order_DTO::Order_DTO(void *buffer) {
  deserialize(buffer);
}

Order_DTO::Order_DTO(int count,
                     const std::string &line,
                     Bdb_errors &errors,
                     char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Order_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(order_id);
  len += Bdb_serialization::buffer_len_string(account_id);
  len += Bdb_serialization::buffer_len_string(name);
  return len;
}

void Order_DTO::create(const Order_DTO &order_DTO, const std::string &order_id_) {
  order_id = order_id_;
  account_id = order_DTO.account_id;
  name = order_DTO.name;
  score = order_DTO.score;
}

void *Order_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(order_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(account_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(name, p);
  return p;
}

void Order_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_order": ... `
  std::string jobj_class_order =
      Bdb_json_utils::get_json_string("Order_DTO::from_json", "1", jobj,
                                      "class_order", errors);
  if (!errors.has() && jobj_class_order != class_order())
    errors.add("Order_DTO::from_json", "2", "not class Order_DTO");
  // parse: ' { "order_id": ... `
  if (!errors.has())
    order_id = Bdb_json_utils::get_json_string("Order_DTO::from_json",
                                               "3", jobj, "order_id", errors);
  if (!errors.has())
    account_id = Bdb_json_utils::get_json_string("Order_DTO::from_json",
                                                 "4", jobj, "account_id", errors);
  if (!errors.has())
    // parse: ' { primaryOrder": ... `
    name = Bdb_json_utils::get_json_string("Order_DTO::from_json",
                                           "35", jobj, "name", errors);
}

int Order_DTO::get_order_name(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Order_DTO order_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string name = order_dto.name;
  size_t keylen = name.size() + 1;
  char *order_id_buf = (char *) malloc(keylen);
  std::strcpy(order_id_buf, name.c_str());
  skey->set_data(order_id_buf);
  skey->set_size(keylen);
  return 0;
}

int Order_DTO::get_order_account_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Order_DTO order_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string account_id = order_dto.account_id;
  size_t keylen = account_id.size() + 1;
  char *order_id_buf = (char *) malloc(keylen);
  std::strcpy(order_id_buf, account_id.c_str());
  skey->set_data(order_id_buf);
  skey->set_size(keylen);
  return 0;
}

void Order_DTO::parse(int count,
                      const std::string &line,
                      Bdb_errors &errors,
                      char delimiter) {
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, delimiter);
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        order_id = token_str;
        break;
      }
      case 1: {
        account_id = token_str;
        break;
      }
      case 2: {
        name = token_str;
        break;
      }
      default: {
        errors.add("Order_DTO::create", "1", "too many order fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  if (i < 3) {
    errors.add("Order_DTO::create", "2", "too few order fields on line "
        + Bdb_tokens::line_print(count, line));
  }
}

void *Order_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(order_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(account_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(name, p);
  return p;
}

json_object *Order_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Order_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_order",
                         json_object_new_string(class_order().c_str()));
  json_object_object_add(root, "order_id",
                         json_object_new_string(order_id.c_str()));
  json_object_object_add(root, "account_id",
                         json_object_new_string(account_id.c_str()));
  json_object_object_add(root, "name",
                         json_object_new_string(name.c_str()));
  return root;
}

std::string Order_DTO::to_string() const {
  std::ostringstream os;
  os << "order:" << std::endl;
  os << "\torder_id        " << order_id << std::endl;
  os << "\taccount_id     " << order_id << std::endl;
  os << "\tname           " << name << std::endl;
  return os.str();
}

Order_DTO_key::Order_DTO_key(const Order_DTO &order_dto) :
    order_id(order_dto.order_id) {}

Order_DTO_key::Order_DTO_key(std::string order_id_) :
    order_id(std::move(order_id_)) {}

Order_DTO_key::Order_DTO_key(void *buffer) {
  deserialize(buffer);
}

Order_DTO_key::Order_DTO_key(const Order_item_DTO &order_item_dto) :
    order_id(order_item_dto.order_id) {}

size_t Order_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(order_id);
  return len;
}

void *Order_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(order_id, p);
  return p;
}

void *Order_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(order_id, p);
  return p;
}

std::string Order_DTO_key::to_string() const {
  return "order_id: " + order_id;
}

size_t Order_DTO_account_id_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(account_id);
  return len;
}

void *Order_DTO_account_id_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(account_id, p);
  return p;
}

void *Order_DTO_account_id_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(account_id, p);
  return p;
}

std::string Order_DTO_account_id_key::to_string() const {
  return "account_id: " + account_id;
}

size_t Order_DTO_name_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(name);
  return len;
}

void *Order_DTO_name_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(name, p);
  return p;
}

void *Order_DTO_name_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(name, p);
  return p;
}

std::string Order_DTO_name_key::to_string() const {
  return "name: " + name;
}

json_object *Order_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Order_DTO_list::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_order",
                         json_object_new_string(class_order().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *order_dto_list_json = json_object_new_array();
    json_object_object_add(root, "order_dto_list", order_dto_list_json);
    for (const auto &order_dto: list) {
      json_object *order_dto_json = order_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(order_dto_list_json, order_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
