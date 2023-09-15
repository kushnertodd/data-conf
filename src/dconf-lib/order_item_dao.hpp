#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"

#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "item_dto.hpp"
#include "order_dto.hpp"
#include "order_item_dto.hpp"
#include "bdb_tokens.hpp"

class Order_item_DAO {
 public:
  static int load(Bdb_dbp &order_item_db,
                  const std::string &text_file,
                  Bdb_errors &errors,
                  char delimiter = tab);
  static void lookup(Bdb_dbp &order_item_db,
                     const std::string &order_item_id,
                     Order_item_DTO &order_item_dto,
                     Bdb_errors &errors);
  static void delete_order(Bdb_dbp &order_db,
                           Bdb_dbp &order_item_db,
                           Bdb_dbp &order_item_order_id_sdb,
                           const std::string &order_id,
                           Order_DTO &order_dto,
                           Bdb_errors &errors);
  static void delete_order_item(Bdb_dbp &order_item_db,
                                Bdb_dbp &order_item_order_id_sdb,
                                Bdb_dbp &order_item_item_id_sdb,
                                const std::string &order_id,
                                const std::string &item_id,
                                Order_item_DTO &order_item_dto_with_key,
                                Bdb_errors &errors);
  static void save(Bdb_dbp &order_item_db,
                   Order_item_DTO &order_item_dto,
                   Order_item_DTO &order_item_dto_with_key,
                   Bdb_errors &errors);
  static void select_by_item_sdb(Bdb_dbp &order_item_item_id_sdb,
                                 const std::string &item_id,
                                 Order_item_DTO_key_list &order_item_dto_key_list,
                                 Bdb_errors &errors);
  static void select_by_order_sdb(Bdb_dbp &order_item_order_id_sdb,
                                  const std::string &order_id,
                                  Order_item_DTO_key_list &order_item_dto_key_list,
                                  Bdb_errors &errors);
  static void select_by_key_list(Bdb_dbp &order_item_db,
                                 Order_item_DTO_key_list &order_item_dto_key_list,
                                 Order_item_DTO_list &order_item_dto_list,
                                 Bdb_errors &errors);
  static void select_items_for_order(Bdb_dbp &order_item_db,
                                     Bdb_dbp &item_db,
                                     Bdb_dbp &order_item_order_id_sdb,
                                     const std::string &order_id,
                                     Item_DTO_list &item_dto_list,
                                     Bdb_errors &errors);
  static void select_item_list(Bdb_dbp &item_db,
                               Order_item_DTO_list &order_item_dto_list,
                               Item_DTO_list &item_dto_list,
                               Bdb_errors &errors);
  static void select_orders_for_item(Bdb_dbp &order_item_item_id_sdb,
                                     Bdb_dbp &order_item_db,
                                     Bdb_dbp &order_db,
                                     const std::string &item_id,
                                     Order_DTO_list &order_dto_list,
                                     Bdb_errors &errors);
  static void select_order_list(Bdb_dbp &order_db,
                                Order_item_DTO_list &order_item_key_list,
                                Order_DTO_list &order_dto_list,
                                Bdb_errors &errors);
  static void select_other_items(Bdb_dbp &order_item_order_id_sdb,
                                 Bdb_dbp &order_item_db,
                                 Bdb_dbp &item_db,
                                 const std::string &order_id,
                                 Item_DTO_list &item_dto_list,
                                 Bdb_errors &errors);
};
