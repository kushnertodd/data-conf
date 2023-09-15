#include <utility>
#include "bdb_db.hpp"

// Bdb_db_config methods

/*!
 * @brief database destructor closes database
 */
Bdb_db::Bdb_db_config::~Bdb_db_config() {
  close();
}

/*!
 * @brief close database
 */
void Bdb_db::Bdb_db_config::close() noexcept {
  try {
    // https://docs.oracle.com/cd/E17076_05/html/api_reference/C/dbclose.html
    db_.close(0);
  }
  catch (DbException &e) {
    std::cerr << Bdb_error("Bdb_db::Bdb_db_config::close", "1",
                           "Error closing database: " + m_filename + " (" + e.what() + ")", e.get_errno()).to_string();
  }
  catch (std::exception &e) {
    std::cerr << Bdb_error("Bdb_db::Bdb_db_config::close", "1",
                           "Error closing database: " + m_filename + " (" + e.what() + ")").to_string();
  }
}

/*!
 * @brief stringify database configuration
 * @return database filename
 */
std::string Bdb_db::Bdb_db_config::to_string() {
  return "filename " + m_filename;
}

// Bdb_db methods

/*!
 * @brief construct database descriptor
 * @param filename database filename
 * @param db_env bdb environment (default nullptr)
 * @param flags database open flags
 */
Bdb_db::Bdb_db(std::string filename, DbEnv *db_env, int flags)
    : m_bdb_db_config{new Bdb_db_config{std::move(filename), db_env, flags}} {

}

/*!
 * @brief set database cache size in gigabytes
 * @param cache_gbytes database cache size
 * @return
 */
Bdb_db &Bdb_db::cache_gbytes(int cache_gbytes) {
  m_bdb_db_config->m_cache_gbytes = cache_gbytes;
  return *this;
}

/*!
 * @brief set database cache size in bytes
 * @param cache_gbytes database cache size
 * @return
 */
Bdb_db &Bdb_db::cache_bytes(int cache_bytes) {
  m_bdb_db_config->m_cache_bytes = cache_bytes;
  return *this;
}

/*!
 * @brief allow creating database
 * @param on default to allow
 * @return reference for builder
 */
Bdb_db &Bdb_db::can_create(bool on) {
  if (on)
    m_bdb_db_config->m_c_flags |= DB_CREATE;
  return *this;
}

/*!
 * @brief flags for opening database
 * @param flags optional flags for open -- should use before other options
 * @return reference for builder
 */
Bdb_db &Bdb_db::c_flags(int flags) {
  m_bdb_db_config->m_c_flags = flags;
  return *this;
}

/*!
 * @brief require database to exist
 * @param on default to allow
 * @return reference for builder
 */
Bdb_db &Bdb_db::must_exist(bool on) {
  if (on)
    m_bdb_db_config->m_c_flags |= DB_EXCL;
  return *this;
}

/*!
 * @brief make database read-only
 * @param on default to allow
 * @return reference for builder
 */
Bdb_db &Bdb_db::read_only(bool on) {
  if (on)
    m_bdb_db_config->m_c_flags |= DB_RDONLY;
  return *this;
}

/*!
 * @brief allow database duplicate keys
 * @param on default to allow
 * @return reference for builder
 */
Bdb_db &Bdb_db::has_duplicates(bool on) {
  if (on)
    m_bdb_db_config->db_.set_flags(DB_DUP);
  return *this;
}

/*!
 * @brief create secondary database (duplicate keys)
 * @param on default to allow
 * @return reference for builder
 */
// If this is a secondary database, support sorted duplicates
Bdb_db &Bdb_db::is_secondary(bool on) {
  if (on)
    m_bdb_db_config->db_.set_flags(DB_DUPSORT);
  return *this;
}

/*!
 * @brief open database with configured flags
 * @param errors database open errors
 */
void Bdb_db::bdb_open(Bdb_errors &errors) {
  try {
    int ret = m_bdb_db_config->db_.set_cachesize(m_bdb_db_config->m_cache_gbytes, m_bdb_db_config->m_cache_bytes, 1);
    if (ret)
      errors.add("BDB_db::open", "1", " set_cachesize error ", ret);
    else {
      // Open the database
      // https://docs.oracle.com/cd/E17076_05/html/api_reference/C/dbopen.html
      ret = m_bdb_db_config->db_.open(nullptr,
                                      m_bdb_db_config->m_filename.c_str(),
                                      nullptr,
                                      DB_BTREE,
                                      m_bdb_db_config->m_c_flags,
                                      0);
      if (ret)
        errors.add("BDB_db::open", "2", "Error opening database: " + m_bdb_db_config->m_filename, ret);
    }
  }
    // DbException is not a subclass of std::exception, so we need to catch them both.
  catch (DbException &e) {
    errors.add("BDB_db::open",
               "3",
               "Error opening database: " + m_bdb_db_config->m_filename + " (" + e.what() + ")",
               e.get_errno());
  }
  catch (std::exception &e) {
    errors.add("BDB_db::open", "4", "Error opening database: " + m_bdb_db_config->m_filename + " (" + e.what() + ")");
  }
}

/*!
 * @brief stringify database configuration
 * @return database configuration descriptive text
 */
std::string Bdb_db::to_string() {
  return m_bdb_db_config->to_string();
}

/*!
 * @brief open database
 * @param errors open errors
 * @return pointer to database configuration
 */
std::unique_ptr<Bdb_db::Bdb_db_config> Bdb_db::open(Bdb_errors &errors) {
  bdb_open(errors);
  return std::move(m_bdb_db_config);
}
