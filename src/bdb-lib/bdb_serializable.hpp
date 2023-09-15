#pragma once

#include <cstddef>

class Bdb_serializable {
 public:
  virtual size_t buffer_size() = 0;
  virtual void *deserialize(void *buffer) = 0;
  virtual void *serialize(void *buffer) = 0;
};
