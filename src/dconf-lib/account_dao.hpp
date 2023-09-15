#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "account_dto.hpp"
#include "bdb_tokens.hpp"

class Account_DAO {
 public:
  static int load(Bdb_dbp &account_db,
                  Bdb_dbp &account_bdb_triplet_db,
                  const std::string &text_file,
                  Bdb_errors &errors,
                  char delimiter = tab);
  static void lookup(Bdb_dbp &account_db,
                     const std::string &id,
                     Account_DTO &account_dto,
                     Bdb_errors &errors);
  static void save(Bdb_dbp &account_db,
                   Account_DTO &account_dto,
                   Bdb_errors &errors);
  static void select_all(Bdb_dbp &account_db,
                         Account_DTO_list &account_dto_list,
                         Bdb_errors &errors);
  static void select_all_for_email(Bdb_dbp &account_email_sdb,
                                   Bdb_dbp &account_db,
                                   const std::string &email,
                                   Account_DTO_list &account_dto_list,
                                   Bdb_errors &errors);
  static void select_all_for_username(Bdb_dbp &account_username_sdb,
                                      Bdb_dbp &account_db,
                                      const std::string &email,
                                      Account_DTO_list &account_dto_list,
                                      Bdb_errors &errors);
  static void update(Bdb_dbp &account_db,
                     Account_DTO &account_dto,
                     Bdb_errors &errors);
};
