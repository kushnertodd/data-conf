#include <cstring>
#include <sstream>
#include <utility>
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "account_dto.hpp"

Account_DTO::Account_DTO(std::string account_id_,
                         std::string username_,
                         std::string email_,
                         std::string created_) :
    account_id(std::move(account_id_)),
    username(std::move(username_)),
    email(std::move(email_)),
    created(std::move(created_)) {}

Account_DTO::Account_DTO(void *buffer) {
  deserialize(buffer);
}

Account_DTO::Account_DTO(int count,
                         const std::string &line,
                         Bdb_errors &errors,
                         char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Account_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(account_id);
  len += Bdb_serialization::buffer_len_string(username);
  len += Bdb_serialization::buffer_len_string(email);
  len += Bdb_serialization::buffer_len_string(created);
  return len;
}

void *Account_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(account_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(username, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(email, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(created, p);
  return p;
}

void Account_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_account": ... `
  std::string jobj_class_account =
      Bdb_json_utils::get_json_string("Account_DTO::from_json", "1", jobj,
                                      "class_account", errors);
  if (!errors.has() && jobj_class_account != class_account())
    errors.add("Account_DTO::from_json", "2", "not class Account_DTO");
  // parse: ' { "account_id": ... `
  account_id = Bdb_json_utils::get_json_string("Account_DTO::from_json",
                                               "3", jobj, "account_id", errors);
  if (!errors.has())
    // parse: ' { username": ... `
    username = Bdb_json_utils::get_json_string("Account_DTO::from_json",
                                               "4", jobj, "username", errors);
  if (!errors.has())
    // parse: ' { "email": ... `
    email = Bdb_json_utils::get_json_string("Account_DTO::from_json",
                                            "5", jobj, "email", errors);
  if (!errors.has())
    // parse: ' { "created": ... `
    created = Bdb_json_utils::get_json_string("Account_DTO::from_json",
                                              "6", jobj, "created", errors);
}

int Account_DTO::get_account_email(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Account_DTO account_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string email = account_dto.email;
  size_t keylen = email.size() + 1;
  char *account_id_buf = (char *) malloc(keylen);
  std::strcpy(account_id_buf, email.c_str());
  skey->set_data(account_id_buf);
  skey->set_size(keylen);
  return 0;
}

int Account_DTO::get_account_username(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Account_DTO account_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string username = account_dto.username;
  size_t keylen = username.size() + 1;
  char *account_id_buf = (char *) malloc(keylen);
  std::strcpy(account_id_buf, username.c_str());
  skey->set_data(account_id_buf);
  skey->set_size(keylen);
  return 0;
}

void Account_DTO::parse(int count,
                        const std::string &line,
                        Bdb_errors &errors,
                        char delimiter) {
  std::vector<std::string> token_list =
      Bdb_tokens::tokenize(line, delimiter);
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        account_id = token_str;
        break;
      }
      case 1: {
        username = token_str;
        break;
      }
      case 2: {
        email = token_str;
        break;
      }
      case 3: {
        created = token_str;
        break;
      }
      default: {
        errors.add("Account_DTO::parse", "1", "too many account fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  if (i < 4) {
    errors.add("Account_DTO::parse", "2", "too few account fields on line "
        + Bdb_tokens::line_print(count, line));
  }
}

void *Account_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(account_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(username, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(email, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(created, p);
  return p;
}

json_object *Account_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Account_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_account", json_object_new_string(class_account().c_str()));
  json_object_object_add(root, "account_id", json_object_new_string(account_id.c_str()));
  json_object_object_add(root, "username", json_object_new_string(username.c_str()));
  json_object_object_add(root, "email", json_object_new_string(email.c_str()));
  json_object_object_add(root, "created", json_object_new_string(created.c_str()));
  //json_object_object_add(root, "score", json_object_new_string(score.c_str()));
  return root;
}

std::string Account_DTO::to_string() const {
  std::ostringstream os;
  os << "account:" << std::endl;
  os << "\taccount_id  " << account_id << std::endl;
  os << "\tusername    " << username << std::endl;
  os << "\temail      " << email << std::endl;
  os << "\tcreated     " << created << std::endl;
  return os.str();
}

Account_DTO_key::Account_DTO_key(const Account_DTO &account_dto) :
    account_id(account_dto.account_id) {}

Account_DTO_key::Account_DTO_key(std::string account_id_) :
    account_id(std::move(account_id_)) {}

Account_DTO_key::Account_DTO_key(void *buffer) {
  deserialize(buffer);
}

size_t Account_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(account_id);
  return len;
}

void *Account_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(account_id, p);
  return p;
}

void *Account_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(account_id, p);
  return p;
}

std::string Account_DTO_key::to_string() const {
  return "account_id: " + account_id;
}

size_t Account_email_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(email);
  return len;
}

void *Account_email_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(email, p);
  return p;
}

void *Account_email_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(email, p);
  return p;
}

std::string Account_email_DTO_key::to_string() const {
  return "email: " + email;
}

size_t Account_username_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(username);
  return len;
}

void *Account_username_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(username, p);
  return p;
}

void *Account_username_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(username, p);
  return p;
}

std::string Account_username_DTO_key::to_string() const {
  return "username: " + username;
}

json_object *Account_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Account_DTO_list::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_account", json_object_new_string(class_account().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *account_dto_list_json = json_object_new_array();
    json_object_object_add(root, "account_dto_list", account_dto_list_json);
    for (const auto &account_dto: list) {
      json_object *account_dto_json = account_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(account_dto_list_json, account_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
