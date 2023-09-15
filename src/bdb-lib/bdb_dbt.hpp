#pragma once

#include <cstring>
#include <cstdlib>
#include <db_cxx.h>

/*!
 * @class Bdb_dbt
 * @brief wrapper for Bdb Dbt class with dto serialize/deserialize methods
 */
class Bdb_dbt {
 private:
  void *buffer{};   //!< serialized dto malloc'd serialized data buffer
  Dbt dbt; //!< passed/returned key/data dto

 public:
  /*!
   * @brief returns reference to internal Dbt, copying prematurely frees serialized dto data buffer
   * @return private Dbt reference
   */
  inline Dbt &get_dbt() { return dbt; }

  /*!
   * @brief deletes malloc'd serialized dto data buffer
   */
  ~Bdb_dbt() {
    if (buffer)
      std::free(buffer);
  }

  Bdb_dbt() = default;

  /*!
   * @brief populates Dbt with serialized dto data
   * @tparam T dto class
   * @param bdb_dto dto whose serialized data populates Dbt
   * @details malloc's serialized data buffer, destructor frees
   */
  template<typename T>
  explicit Bdb_dbt(T &bdb_dto) {
    size_t len = bdb_dto.buffer_size();
    buffer = std::malloc(len);
    std::memset(buffer, '\0', len);
    bdb_dto.serialize(buffer);
    dbt.set_size((u_int32_t) len);
    dbt.set_data(buffer);
  }

  /*!
   * @brief deserializes Dbt data into dto
   * @tparam T dto class
   * @param bdb_dto contains deserialized Dbt data
   */
  template<typename T>
  void set_dto(T &bdb_dto) {
    bdb_dto.deserialize(dbt.get_data());
  }

  /*!
   * @brief deserializes Dbt data and appends to dto list
   * @tparam L dto list class, .list is vector<dto>
   * @param bdb_dto_list has deserialized Dbt data appended
   */
  template<typename L>
  void set_dto_list(L &bdb_dto_list) {
    bdb_dto_list.list.emplace_back(dbt.get_data());
  }
};
