#pragma once

#include <json-c/json.h>
#include <list>
#include <utility>
#include "bdb_tokens.hpp"
#include "bdb_serializable.hpp"

class Bdb_text_id_occurence {
 public:
  std::string id{};
  int triplets_count{};
  int occurrence_count{};
  Bdb_text_id_occurence() = default;
  Bdb_text_id_occurence(std::string id_, int triplets_count_, int occurrence_count_)
      : id(std::move(id_)),
        triplets_count(triplets_count_),
        occurrence_count(occurrence_count_) {}
  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
};

class Bdb_text_id_occurrence_list {
 public:
  std::vector<Bdb_text_id_occurence> list;
  void add(const Bdb_text_id_occurence &bdb_text_id_occurence);
};

class Bdb_text_triplet_occurence {
 public:
  std::string triplet{};
  int occurrence_count{};
  Bdb_text_triplet_occurence() = default;
  Bdb_text_triplet_occurence(std::string triplet_, int occurrence_count_)
      : triplet(std::move(triplet_)), occurrence_count(occurrence_count_) {}
  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
};

class Bdb_text_triplets {
 public:
  int triplets_count{};
  std::list<Bdb_text_triplet_occurence> list;
  Bdb_text_triplets() = default;
  explicit Bdb_text_triplets(const std::string &text);
};

class Bdb_triplets_DTO { // use Imdb_DTO as pattern but do not inherit
 public:
  std::string triplet{};
  std::list<Bdb_text_id_occurence> list;

  Bdb_triplets_DTO() = default;
  explicit Bdb_triplets_DTO(std::string triplet_) :
      triplet(std::move(triplet_)) {}
  explicit Bdb_triplets_DTO(void *buffer);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
};

class Bdb_triplets_DTO_key {
 public:
  std::string triplet{};
  Bdb_triplets_DTO_key() = default;

  explicit Bdb_triplets_DTO_key(const Bdb_triplets_DTO &bdb_triplets_dto);
  explicit Bdb_triplets_DTO_key(std::string bdb_triplet_);
  explicit Bdb_triplets_DTO_key(void *buffer);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
};
