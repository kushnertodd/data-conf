#pragma once

#include <db_cxx.h>
#include <iostream>
#include <utility>
#include <memory>
#include "bdb_errors.hpp"

/*!
 * @class BDb_db wrapper for Bdb DB  class
 * uses builder pattern to construct
 * https://platis.solutions/blog/2023/01/03/builder-pattern-cpp/
 */

class Bdb_db {
 public:

  /*!
   * @class Bdb Db class wrapper builder class
   */
  class Bdb_db_config {
   public:
    ~Bdb_db_config();
    std::string to_string();
    inline Db &get_db() { return db_; }

   private:
    int m_cache_gbytes{4};
    int m_cache_bytes{};
    bool m_can_create{false};
    std::string m_filename{};
    bool m_has_duplicates{false};
    bool m_is_secondary{false};
    bool m_must_exist{false};
    bool m_read_only{false};
    bool m_truncate{false};
    Db db_;
    u_int32_t m_c_flags{};

    friend class Bdb_db;

    explicit Bdb_db_config(std::string filename, DbEnv *db_env, int flags) :
        db_(db_env, flags),
          m_filename(std::move(filename)) {}
    void close() noexcept;
  };

  /*!
   * @brief Bdb Db class wrapper constructor
   * @param filename database filename
   * @param db_env optional Bdb DbEnv class pointer
   * @param flags Db database open flags
   */
  explicit Bdb_db(std::string filename, DbEnv *db_env = nullptr, int flags = 0);
  void bdb_open(Bdb_errors &errors);
  Bdb_db &c_flags(int flags);
  Bdb_db &cache_bytes(int cache_bytes);
  Bdb_db &cache_gbytes(int m_cache_gbytes);
  Bdb_db &can_create(bool on = true);
  inline Db &get_db() { return m_bdb_db_config->db_; }
  Bdb_db &has_duplicates(bool on = true);
  Bdb_db &is_secondary(bool on = true);
  Bdb_db &must_exist(bool on = true);
  Bdb_db &read_only(bool on = true);
  std::unique_ptr<Bdb_db_config> open(Bdb_errors &errors);
  std::string to_string();

 private:
  std::unique_ptr<Bdb_db_config> m_bdb_db_config{};

  // https://learn.microsoft.com/en-us/cpp/standard-library/overloading-the-output-operator-for-your-own-classes?view=msvc-170
  std::ostream &operator<<(std::ostream &os) {
    return os << m_bdb_db_config->to_string();
  }
};

using Bdb_dbp = std::unique_ptr<Bdb_db::Bdb_db_config>;

//std::ostream &operator<<(std::ostream &os, const Bdb_db &bdb_db);
