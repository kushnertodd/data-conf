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
#include "item_dto.hpp"
#include "order_dto.hpp"

class Item_DAO {
 public:
  static int load(Bdb_dbp &item_db,
                  Bdb_dbp &item_bdb_triplet_db,
                  const std::string &text_file,
                  Bdb_errors &errors,
                  char delimiter = tab);
  static void lookup(Bdb_dbp &item_db,
                     const std::string &item_id,
                     Item_DTO &item_dto,
                     Bdb_errors &errors);
  static void save(Bdb_dbp &item_db,
                   Item_DTO &item_dto,
                   Bdb_errors &errors);
  static void select_all(Bdb_dbp &item_db,
                         Item_DTO_list &item_dto_list,
                         Bdb_errors &errors);
  static void select_all_for_cost(Bdb_dbp &item_cost_sdb,
                                     Bdb_dbp &item_db,
                                     const std::string &cost,
                                     Item_DTO_list &item_dto_list,
                                     Bdb_errors &errors);
  static void select_all_for_name(Bdb_dbp &item_name_sdb,
                                  Bdb_dbp &item_db,
                                  const std::string &name,
                                  Item_DTO_list &item_dto_list,
                                  Bdb_errors &errors);
  static void update(Bdb_dbp &item_db,
                     Item_DTO &item_dto,
                     Bdb_errors &errors);
};
