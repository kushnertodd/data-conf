#pragma once

#include <json-c/json.h>
#include <filesystem>
#include <list>
#include "bdb_databases_config.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_key_extractor.hpp"

/*!
 * @brief secondary database with duplicate keys associated with primary database records
 */
class Secondary_database {
 public:
  std::unique_ptr<Bdb_db::Bdb_db_config> bdb_db;
  std::string name{};
  std::string key_extractor{};

  Secondary_database() = delete;
  Secondary_database(Secondary_database_config &secondary_database_config,
                     const std::string &db_home, Bdb_errors &errors);

  Secondary_database(const Secondary_database &other) = delete;
  Secondary_database &operator=(const Secondary_database &other) = delete;
  Secondary_database(Secondary_database &&other) noexcept = delete;
  Secondary_database &operator=(Secondary_database &&other) noexcept = delete;

};

using Secondary_database_list = std::list<Secondary_database>;
using Secondary_database_list_iter = Secondary_database_list::iterator;

/*!
 * @brief primary database with unique key records
 */
class Primary_database {
 public:
  std::unique_ptr<Bdb_db::Bdb_db_config> bdb_db;
  std::string name{};
  Bdb_key_extractor *bdb_key_extractor{};

  Secondary_database_list secondary_database_list;

  Primary_database() = delete;
  Primary_database(Primary_database_config &primary_database_config,
                   Bdb_key_extractor *bdb_key_extractor_,
                   const std::string &db_home, Bdb_errors &errors);

  Primary_database(const Primary_database &other) = delete;
  Primary_database &operator=(const Primary_database &other) = delete;
  Primary_database(Primary_database &&other) = delete;
  Primary_database &operator=(Primary_database &&other) = delete;
};
