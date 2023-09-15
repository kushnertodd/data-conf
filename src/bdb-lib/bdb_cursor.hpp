#pragma once

#include <cstring>
#include <db_cxx.h>
#include "bdb_db.hpp"
#include "bdb_dbt.hpp"
#include "bdb_errors.hpp"

/*!
 * @class Bdb_cursor wrapper class for Bdb Dbc class
 * @details https://web.mit.edu/ghudson/dev/nokrb/third/db/docs/api_c/db_cursor.html
 */
class Bdb_cursor {
 private:
  Bdb_dbp &bdb_db;
  Dbc *cursorp{};
  bool done{};
  bool eof{};

 public:
  [[nodiscard]] inline bool is_done() const { return done; }
  [[nodiscard]] inline bool is_eof() const { return eof; }

  Bdb_cursor() = delete;

  /*!
   * @brief closes cursor
   * @post prevents exception
   * @post errors outputs to std::cerr
   */
  ~Bdb_cursor() {
    try {
      cursorp->close();
    }
    catch (DbException &e) {
      std::cerr << Bdb_error("Bdb_cursor::close",
                             "1",
                             "Error closing database cursor for " + bdb_db->to_string() + " (" + e.what() + ")",
                             e.get_errno()).to_string();
    }
    catch (std::exception &e) {
      std::cerr << Bdb_error("Bdb_cursor::close", "2",
                             "Error closing database cursor for " + bdb_db->to_string() + " (" + e.what()
                                 + ")").to_string();
    }
  }

/*!
 * @brief initializes database cursor in optional transaction
 * @param cursor_db database in which cursor is opened
 * @param errors set on cursor open errors
 * @param txnid optional transaction, in which the cursor must be both opened and closed
 */
  Bdb_cursor(Bdb_dbp &cursor_db, Bdb_errors &errors, DbTxn *txnid = nullptr) :
      bdb_db(cursor_db) {
    try {
      int ret = cursor_db->get_db().cursor(txnid, &cursorp, 0);
      if (ret)
        errors.add("Bdb_cursor::Bdb_cursor", "1", "Error opening cursor in database " + bdb_db->to_string(), ret);
    }
    catch (DbException &e) {
      errors.add("Bdb_cursor::Bdb_cursor",
                 "2",
                 "Error opening cursor in database " + bdb_db->to_string() + " (" + std::string(e.what()));
    }
    catch (std::exception &e) {
      errors.add("Bdb_cursor::Bdb_cursor",
                 "3",
                 "Error opening cursor in database " + bdb_db->to_string() + " (" + std::string(e.what()));
    }
  }

  /*!
   * @brief returns dto and key for cursor record
   * @details see https://docs.oracle.com/cd/E17275_01/html/api_reference/C/dbcget.html
   * @tparam K key dto type
   * @tparam T data dto type
   * @param bdb_key_dto dto key for record at current position
   * @param bdb_data_dto dto data for record at current position
   * @param errors cursor get errors
   * @param flags DB_FIRST, DB_NEXT, DB_NEXT_DUP, DB_NEXT_NODUP
   * @details for DB_SET, use dto_get_key()
   * @post for non-duplicate database, DB_FIRST positions cursor to first record and returns dto
   * @post for duplicate database, DB_FIRST returns dto for first record and positions cursor to first duplicate for that key
   * @post for non-duplicate database, DB_NEXT returns dto for next record
   * @post for duplicate database, DB_NEXT positions returns dto for next duplicate record if any, if so with the same key as last
   * @post for duplicate database, DB_NEXT_DUP returns dto for next duplicate record if any
   * @post for duplicate database, DB_NEXT_NODUP returns dto for next non-duplicate record after duplicate set
   * @post done is true if cursor has no more records or error occurred
   * @post eof is true if cursor has no more records
   */
  template<typename K, typename T>
  void dto_get(K &bdb_key_dto, T &bdb_data_dto, Bdb_errors &errors, int flags) {
    Bdb_dbt bdb_key_dbt{};
    Bdb_dbt bdb_data_dbt{};
    try {
      int ret = cursorp->get(&bdb_key_dbt.get_dbt(), &bdb_data_dbt.get_dbt(), flags);
      if (ret == DB_NOTFOUND) {
        done = true;
        eof = true;
      } else if (ret) {
        done = true;
        errors.add("Bdb_cursor::get", "1", "cursor get error in database " + bdb_db->to_string(), ret);
      } else {
        bdb_key_dbt.set_dto<K>(bdb_key_dto);
        bdb_data_dbt.set_dto<T>(bdb_data_dto);
      }
    }
    catch (DbException &e) {
      errors.add("Bdb_cursor::get", "2", "cursor get error in database " + bdb_db->to_string() + std::string(e.what()));
    }
    catch (std::exception &e) {
      errors.add("Bdb_cursor::get", "3", "cursor get error in database " + bdb_db->to_string() + std::string(e.what()));
    }
  }

  /*!
   * @brief gets a list of all duplicate records in database for key
   * @tparam K key dto type
   * @tparam T data dto type
   * @tparam L data dto list type
   * @param bdb_data_dto_list list of all dto data after key
   * @param errors cursor get errors
   * @details assumes secondary database or primary database with duplicates permitted
   */
  template<typename K, typename T, typename L>
  void dto_get_duplicate_list(K &bdb_key_dto, L &bdb_data_dto_list, Bdb_errors &errors, bool ignore_not_found = false) {
    T bdb_data_dto;
    K bdb_key_dto_next;
    for (dto_get_key<K, T>(bdb_key_dto, bdb_data_dto, errors, ignore_not_found);
         !done;
         dto_get_next_dup<K, T>(bdb_key_dto_next, bdb_data_dto, errors))
      bdb_data_dto_list.add(bdb_data_dto);
  }
  /*!
   * @brief gets first record for cursor, returns key and data
   * @tparam K key dto type
   * @tparam T data dto type
   * @param bdb_key_dto dto key for first record
   * @param bdb_data_dto dto for first record
   * @param errors cursor get errors
   * @post for non-duplicate database, positions cursor to first record and returns dto
   * @post for duplicate database, returns dto for first record and positions cursor to first duplicate for that key
   */
  template<typename K, typename T>
  void dto_get_first(K &bdb_key_dto, T &bdb_data_dto, Bdb_errors &errors) {
    dto_get(bdb_key_dto, bdb_data_dto, errors, DB_FIRST);
  }

  /*!
   * @brief returns dto for cursor record at key
   * @tparam K key dto type
   * @tparam T data dto type
   * @param bdb_key_dto key for cursor record to read
   * @param bdb_data_dto dto data for record at key
   * @param errors cursor get errors, e.g., key not found
   * @post for non-duplicate database, positions cursor to record with key
   * @post for duplicate database, positions cursor to first duplicate record with key
   * @post for secondary databases, the data is the primary database key
   */
  template<typename K, typename T>
  void dto_get_key(K &bdb_key_dto, T &bdb_data_dto, Bdb_errors &errors, bool ignore_not_found = false) {
    Bdb_dbt bdb_key_dbt{bdb_key_dto};
    Bdb_dbt bdb_data_dbt{};
    try {
      int ret = cursorp->get(&bdb_key_dbt.get_dbt(), &bdb_data_dbt.get_dbt(), DB_SET);
      if (ret == DB_NOTFOUND) {
        done = true;
        if (!ignore_not_found)
          errors.add("Bdb_cursor::dto_get_key", "1", " key not found in database " + bdb_db->to_string(), ret);
      } else if (ret) {
        done = true;
        errors.add("Bdb_cursor::dto_get_key", "2", "cursor set key error in database " + bdb_db->to_string(), ret);
      } else
        bdb_data_dbt.set_dto(bdb_data_dto);
    }
    catch (DbException &e) {
      errors.add("Bdb_cursor::dto_get_key",
                 "3",
                 "cursor get error in database " + bdb_db->to_string() + std::string(e.what()));
    }
    catch (std::exception &e) {
      errors.add("Bdb_cursor::dto_get_key",
                 "4",
                 "cursor get error in database " + bdb_db->to_string() + std::string(e.what()));
    }
  }

  /*!
   * @brief gets a list of all records in database
   * @tparam K key dto type
   * @tparam T data dto type
   * @tparam L data dto list type
   * @param bdb_data_dto_list list of all dto data
   * @param errors cursor get errors
   */
  template<typename K, typename T, typename L>
  void dto_get_list(L &bdb_data_dto_list, Bdb_errors &errors) {
    K bdb_key_dto;
    T bdb_data_dto;
    for (dto_get_first<K, T>(bdb_key_dto, bdb_data_dto, errors);
         !done;
         dto_get_next<K, T>(bdb_key_dto, bdb_data_dto, errors))
      bdb_data_dto_list.add(bdb_data_dto);
  }
  /*!
   * @brief gets next record for cursor, returns key and data
   * @tparam K key dto type
   * @tparam T data dto type
   * @param bdb_key_dto returned dto key
   * @param bdb_data_dto returned dto data
   * @param errors cursor get errors
   * @post for non-duplicate database, returns dto for next record
   * @post for duplicate database, positions returns dto for next duplicate record if any, if so with the same key as last
   */
  template<typename K, typename T>
  void dto_get_next(K &bdb_key_dto, T &bdb_data_dto, Bdb_errors &errors) {
    dto_get(bdb_key_dto, bdb_data_dto, errors, DB_NEXT);
  }

  /*!
   * @brief gets next duplicate record for cursor, returns key and data
   * @tparam K key dto type
   * @tparam T data dto type
   * @param bdb_key_dto returned dto key
   * @param bdb_data_dto returned dto data
   * @param errors cursor get errors
   * @post for secondary databases, key is the duplicate key and the data is the primary database key
   * @remark for secondary database, contrary to the the documentation that says the primary database data item is returned
   */
  template<typename K, typename T>
  void dto_get_next_dup(K &bdb_key_dto, T &bdb_data_dto, Bdb_errors &errors) {
    dto_get(bdb_key_dto, bdb_data_dto, errors, DB_NEXT_DUP);
  }
};
