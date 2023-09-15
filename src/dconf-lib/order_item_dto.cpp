#include <cstring>
#include <sstream>
#include <utility>
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "order_item_dto.hpp"

Order_item_DTO::Order_item_DTO(std::string order_id_, std::string item_id_) :
    order_id(std::move(order_id_)),
    item_id(std::move(item_id_)) {}

Order_item_DTO::Order_item_DTO(Order_item_DTO order_item_DTO, std::string &order_item_id_) :
    order_item_id(std::move(order_item_id_)),
    order_id(std::move(order_item_DTO.order_id)),
    item_id(std::move(order_item_DTO.item_id)) {}

Order_item_DTO::Order_item_DTO(void *buffer) {
  deserialize(buffer);
}

Order_item_DTO::Order_item_DTO(int count,
                               const std::string &line,
                               Bdb_errors &errors,
                               char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Order_item_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(order_item_id);
  len += Bdb_serialization::buffer_len_string(order_id);
  len += Bdb_serialization::buffer_len_string(item_id);
  return len;
}

void Order_item_DTO::create(const Order_item_DTO &order_item_dto,
                            const std::string &order_item_id_) {
  order_item_id = order_item_id_;
  order_id = order_item_dto.order_id;
  item_id = order_item_dto.item_id;
}

void *Order_item_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(order_item_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(order_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(item_id, p);
  return p;
}

void Order_item_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_order_item": ... `
  std::string jobj_class_order_item =
      Bdb_json_utils::get_json_string("Order_item_DTO::from_json", "1", jobj,
                                      "class_order_item", errors);
  if (!errors.has() && jobj_class_order_item != class_order_item())
    errors.add("Order_item_DTO::from_json", "2", "not class Order_item_DTO");
  // parse: ' { "order_item_id": ... `
  order_item_id = Bdb_json_utils::get_json_string("Order_item_DTO::from_json", "3", jobj, "order_item_id", errors);
  if (!errors.has())
    // parse: ' { order_id": ... `
    order_id = Bdb_json_utils::get_json_string("Order_item_DTO::from_json", "4", jobj, "order_id", errors);
  if (!errors.has())
    // parse: ' { "item_id": ... `
    item_id = Bdb_json_utils::get_json_string("Order_item_DTO::from_json", "5", jobj, "item_id", errors);
}

int Order_item_DTO::get_order_item_order_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Order_item_DTO order_item_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string order_id = order_item_dto.order_id;
  size_t keylen = order_id.size() + 1;
  char *order_id_buf = (char *) malloc(keylen);
  std::strcpy(order_id_buf, order_id.c_str());
  skey->set_data(order_id_buf);
  skey->set_size(keylen);
  return 0;
}

int Order_item_DTO::get_order_item_item_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Order_item_DTO order_item_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string item_id = order_item_dto.item_id;
  size_t keylen = item_id.size() + 1;
  char *item_id_buf = (char *) malloc(keylen);
  std::strcpy(item_id_buf, item_id.c_str());
  skey->set_data(item_id_buf);
  skey->set_size(keylen);
  return 0;
}

void Order_item_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, delimiter);
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        order_item_id = token_str;
        break;
      }
      case 1: {
        order_id = token_str;
        break;
      }
      case 2: {
        item_id = token_str;
        break;
      }
      default: {
        errors.add("Order_item_DTO::create", "1", "too many item fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  // Store the tokens as per structure members , where (i==0) is first member and so on..
  if (i < 3) {
    errors.add("Order_item_DTO::create", "2", "too few item fields on line "
        + Bdb_tokens::line_print(count, line));
  }
}

void *Order_item_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(order_item_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(order_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(item_id, p);
  return p;
}

json_object *Order_item_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Order_item_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_order_item",
                         json_object_new_string(class_order_item().c_str()));
  json_object_object_add(root, "order_item_id",
                         json_object_new_string(order_item_id.c_str()));
  json_object_object_add(root, "order_id",
                         json_object_new_string(order_id.c_str()));
  return root;
}

std::string Order_item_DTO::to_string() const {
  std::ostringstream os;
  os << "order_item:" << std::endl;
  os << "\torder_item_id " << order_item_id << std::endl;
  os << "\torder_id      " << order_id << std::endl;
  os << "\titem_id      " << item_id << std::endl;
  return os.str();
}

Order_item_DTO_key::Order_item_DTO_key(const Order_item_DTO &order_item_dto) :
    order_item_id(order_item_dto.order_item_id) {}

Order_item_DTO_key::Order_item_DTO_key(std::string order_item_id_) :
    order_item_id(std::move(order_item_id_)) {}

Order_item_DTO_key::Order_item_DTO_key(void *buffer) {
  deserialize(buffer);
}

size_t Order_item_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(order_item_id);
  return len;
}

void *Order_item_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(order_item_id, p);
  return p;
}

void *Order_item_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(order_item_id, p);
  return p;
}

std::string Order_item_DTO_key::to_string() const {
  return "order_item_id: " + order_item_id;
}

json_object *Order_item_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Order_item_DTO_list::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_order_item",
                         json_object_new_string(class_order_item().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *order_item_dto_list_json = json_object_new_array();
    json_object_object_add(root, "order_item_dto_list", order_item_dto_list_json);
    for (const auto &order_item_dto: list) {
      json_object *order_item_dto_json = order_item_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(order_item_dto_list_json, order_item_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
