#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "bdb_tokens.hpp"
#include "order_dto.hpp"

class Order_DAO {
 public:
  static int load(Bdb_dbp &order_db,
                  Bdb_dbp &order_bdb_triplet_db,
                  const std::string &text_file,
                  Bdb_errors &errors,
                  char delimiter = tab);
  static void lookup(Bdb_dbp &order_db,
                     const std::string &order_id,
                     Order_DTO &order_dto,
                     Bdb_errors &errors);
  static void save(Bdb_dbp &order_db,
                   Order_DTO &order_dto,
                   Bdb_errors &errors);
  static void save_key(Bdb_dbp &order_db,
                       Order_DTO &order_dto,
                       Order_DTO &order_dto_with_key,
                   Bdb_errors &errors);
  static void select_all(Bdb_dbp &order_db,
                         Order_DTO_list &order_dto_list,
                         Bdb_errors &errors);
  static void select_all_for_account_id(Bdb_dbp &order_account_id_sdb,
                                        Bdb_dbp &order_db,
                                        const std::string &account_id,
                                        Order_DTO_list &order_dto_list,
                                        Bdb_errors &errors);
  static void select_all_for_name(Bdb_dbp &order_name_sdb,
                                  Bdb_dbp &order_db,
                                  const std::string &name,
                                  Order_DTO_list &order_dto_list,
                                  Bdb_errors &errors);
  static void update(Bdb_dbp &order_db,
                     Order_DTO &order_dto,
                     Bdb_errors &errors);
};
