#include "bdb_databases.hpp"

// Secondary_database methods

/*!
 * @brief open secondary database
 * @param secondary_database_config defines database name, secondary db key extractor, and output filename
 * @param errors database open errors
 */
Secondary_database::Secondary_database(Secondary_database_config &secondary_database_config,
                                       const std::string &db_home,
                                       Bdb_errors &errors) :
    name(secondary_database_config.name),
    key_extractor(secondary_database_config.key_extractor) {
  bdb_db = Bdb_db(secondary_database_config.get_filename(db_home),
                  nullptr,
                  DB_CXX_NO_EXCEPTIONS) // TODO: no hardwiring DB_CXX_NO_EXCEPTIONS
      .can_create()
      .is_secondary()
      .open(errors);
}


// Primary_database methods

/*!
 * @brief open primary database
 * @param secondary_database_config defines database name, key extractor factory, and output filename
 * @param errors database open errors
 */
Primary_database::Primary_database(Primary_database_config &primary_database_config,
                                   Bdb_key_extractor *bdb_key_extractor_,
                                   const std::string &db_home, Bdb_errors &errors) :
    name(primary_database_config.name),
    bdb_key_extractor(bdb_key_extractor_) {
  bdb_db = Bdb_db(primary_database_config.get_filename(db_home),
                  nullptr,
                  DB_CXX_NO_EXCEPTIONS) // TODO: no hardwiring DB_CXX_NO_EXCEPTIONS
      .can_create()
      .has_duplicates(primary_database_config.has_duplicates)
      .open(errors);
  if (!errors.has())
    for ( auto &[db_name, secondary_database_config]:
        primary_database_config.secondary_database_config_map) {
      secondary_database_list.emplace_back(secondary_database_config, db_home, errors);
    }
  if (!errors.has())
    for (const auto &secondary_database: secondary_database_list) {
      KEY_EXTRACTOR_FCT_TYPE key_extractor =
          bdb_key_extractor->key_extractor_fct(secondary_database.key_extractor.c_str());
      if (key_extractor == nullptr)
        errors.add("Primary_database::open",
                   "1",
                   "invalid secondary database " + bdb_db->to_string() + " key_extractor: "
                       + secondary_database.key_extractor);
      if (!errors.has()) {
        int ret = bdb_db->get_db().associate(nullptr, &secondary_database.bdb_db->get_db(), key_extractor, 0);
        if (ret)
          errors.add("Primary_database::open",
                     "2",
                     "associate primary db " + bdb_db->to_string() + " with secondary db "
                         + secondary_database.bdb_db->to_string() + " error",
                     ret);
      }
      if (errors.has())
        break;
    }
}
