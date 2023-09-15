#pragma once

#include <db.h>
#include "bdb_db.hpp"

using KEY_EXTRACTOR_FCT_TYPE = int (*)(Db *, const Dbt *, const Dbt *, Dbt *);

/*!
 * wrapper class for Bdb secondary database key extractor callback
 */
class Bdb_key_extractor {
 public:
  virtual ~Bdb_key_extractor() = default;
  virtual KEY_EXTRACTOR_FCT_TYPE key_extractor_fct(const char *key_extractor_name) = 0;
};
