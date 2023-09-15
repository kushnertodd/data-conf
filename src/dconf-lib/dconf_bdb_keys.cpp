#include <cstring>
#include "account_dto.hpp"
#include "item_dto.hpp"
#include "order_dto.hpp"
#include "order_item_dto.hpp"
#include "dconf_bdb_keys.hpp"

KEY_EXTRACTOR_FCT_TYPE Dconf_bdb_key_extractor::key_extractor_fct(const char *key_extractor_name) {
  if (!strcmp(key_extractor_name, "get_account_email"))
    return Account_DTO::get_account_email;
  if (!strcmp(key_extractor_name, "get_account_username"))
    return Account_DTO::get_account_username;
  if (!strcmp(key_extractor_name, "get_item_name"))
    return Item_DTO::get_item_name;
  if (!strcmp(key_extractor_name, "get_item_cost"))
    return Item_DTO::get_item_cost;
  if (!strcmp(key_extractor_name, "get_order_account_id"))
    return Order_DTO::get_order_account_id;
  if (!strcmp(key_extractor_name, "get_order_name"))
    return Order_DTO::get_order_name;
  if (!strcmp(key_extractor_name, "get_order_item_order_id"))
    return Order_item_DTO::get_order_item_order_id;
  if (!strcmp(key_extractor_name, "get_order_item_item_id"))
    return Order_item_DTO::get_order_item_item_id;
  else return nullptr;
}


