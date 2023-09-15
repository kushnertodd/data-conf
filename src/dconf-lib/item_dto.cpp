#include <cstring>
#include <sstream>
#include <utility>
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "item_dto.hpp"
#include "order_item_dto.hpp"

Item_DTO::Item_DTO(std::string item_id_,
                   std::string name_,
                   std::string cost_) :
    item_id(std::move(item_id_)),
    name(std::move(name_)),
    cost(std::move(cost_)) {}

Item_DTO::Item_DTO(void *buffer) {
  deserialize(buffer);
}

Item_DTO::Item_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Item_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(item_id);
  len += Bdb_serialization::buffer_len_string(name);
  len += Bdb_serialization::buffer_len_string(cost);
  return len;
}

void *Item_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(item_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(name, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(cost, p);
  return p;
}

void Item_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_item": ... `
  std::string jobj_class_item =
      Bdb_json_utils::get_json_string("Item_DTO::from_json", "1", jobj,
                                      "class_item", errors);
  if (!errors.has() && jobj_class_item != class_item())
    errors.add("Item_DTO::from_json", "2", "not class Item_DTO");
  // parse: ' { "item_id": ... `
  item_id = Bdb_json_utils::get_json_string("Item_DTO::from_json", "3", jobj, "item_id", errors);
  if (!errors.has())
    // parse: ' { name": ... `
    name = Bdb_json_utils::get_json_string("Item_DTO::from_json", "4", jobj, "name", errors);
  if (!errors.has())
    // parse: ' { "cost": ... `
    cost = Bdb_json_utils::get_json_string("Item_DTO::cost", "5", jobj, "cost", errors);
}

int Item_DTO::get_item_name(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Item_DTO item_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string name = item_dto.name;
  size_t keylen = name.size() + 1;
  char *item_id_buf = (char *) malloc(keylen);
  std::strcpy(item_id_buf, name.c_str());
  skey->set_data(item_id_buf);
  skey->set_size(keylen);
  return 0;
}

int Item_DTO::get_item_cost(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Item_DTO item_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string cost = item_dto.cost;
  size_t keylen = cost.size() + 1;
  char *item_id_buf = (char *) malloc(keylen);
  std::strcpy(item_id_buf, cost.c_str());
  skey->set_data(item_id_buf);
  skey->set_size(keylen);
  return 0;
}

void Item_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  // nconst	primaryPrincipals	birthYear	deathYear	primaryProfession	knownForTitle
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, delimiter);
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        item_id = token_str;
        break;
      }
      case 1: {
        name = token_str;
        break;
      }
      case 2: {
        cost = token_str;
        break;
      }
      default: {
        errors.add("Item_DTO::create", "1", "too many item fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  // Store the tokens as per structure members , where (i==0) is first member and so on..
  if (i < 3) {
    errors.add("Item_DTO::parse", "2", "too few item fields on line "
        + Bdb_tokens::line_print(count, line));
  }
}

void *Item_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(item_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(name, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(cost, p);
  return p;
}

json_object *Item_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Item_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_item", json_object_new_string(class_item().c_str()));
  json_object_object_add(root, "item_id", json_object_new_string(item_id.c_str()));
  json_object_object_add(root, "name", json_object_new_string(name.c_str()));
  json_object_object_add(root, "cost", json_object_new_string(cost.c_str()));
  //json_object_object_add(root, "score", json_object_new_string(score.c_str()));
  return root;
}

std::string Item_DTO::to_string() const {
  std::ostringstream os;
  os << "item:" << std::endl;
  os << "\titem_id        " << item_id << std::endl;
  os << "\tname    " << name << std::endl;
  os << "\tcost      " << cost << std::endl;
  return os.str();
}

Item_DTO_key::Item_DTO_key(const Item_DTO &item_DTO) :
    item_id(item_DTO.item_id) {}

Item_DTO_key::Item_DTO_key(std::string item_id_) :
    item_id(std::move(item_id_)) {}

Item_DTO_key::Item_DTO_key(void *buffer) {
  deserialize(buffer);
}

Item_DTO_key::Item_DTO_key(const Order_item_DTO &order_item_dto) :
    item_id(order_item_dto.item_id) {}

size_t Item_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(item_id);
  return len;
}

void *Item_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(item_id, p);
  return p;
}

void *Item_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(item_id, p);
  return p;
}

std::string Item_DTO_key::to_string() const {
  return "item_id: " + item_id;
}

size_t Item_DTO_name_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(name);
  return len;
}

void *Item_DTO_name_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(name, p);
  return p;
}

void *Item_DTO_name_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(name, p);
  return p;
}

std::string Item_DTO_name_key::to_string() const {
  return "name: " + name;
}

size_t Item_DTO_cost_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(cost);
  return len;
}

void *Item_DTO_cost_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(cost, p);
  return p;
}

void *Item_DTO_cost_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(cost, p);
  return p;
}

std::string Item_DTO_cost_key::to_string() const {
  return "cost: " + cost;
}

json_object *Item_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Item_DTO_list::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_item", json_object_new_string(class_item().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *item_dto_list_json = json_object_new_array();
    json_object_object_add(root, "item_dto_list", item_dto_list_json);
    for (const auto &item_dto: list) {
      json_object *item_dto_json = item_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(item_dto_list_json, item_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
