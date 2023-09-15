#include <sstream>
#include <utility>
#include "bdb_serialization.hpp"
#include "bdb_triplets_dto.hpp"

// Bdb_text_id_occurence methods

[[nodiscard]] size_t Bdb_text_id_occurence::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(id);
  len += sizeof(triplets_count);
  len += sizeof(occurrence_count);
  return len;
}
void *Bdb_text_id_occurence::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_int(triplets_count, p);
  p = (unsigned char *) Bdb_serialization::deserialize_int(occurrence_count, p);
  return p;
}
void *Bdb_text_id_occurence::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(id, p);
  p = (unsigned char *) Bdb_serialization::serialize_int(triplets_count, p);
  p = (unsigned char *) Bdb_serialization::serialize_int(occurrence_count, p);
  return p;
}

// Bdb_text_id_occurrence_list methods

void Bdb_text_id_occurrence_list::add(const Bdb_text_id_occurence &bdb_text_id_occurence) {
  list.push_back(bdb_text_id_occurence);
}

// Bdb_text_triplet_occurence methods

size_t Bdb_text_triplet_occurence::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(triplet);
  len += sizeof(occurrence_count);
  return len;
}
void *Bdb_text_triplet_occurence::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(triplet, p);
  p = (unsigned char *) Bdb_serialization::deserialize_int(occurrence_count, p);
  return p;
}
void *Bdb_text_triplet_occurence::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(triplet, p);
  p = (unsigned char *) Bdb_serialization::serialize_int(occurrence_count, p);
  return p;
}

// Bdb_text_triplets methods

Bdb_text_triplets::Bdb_text_triplets(const std::string &text) {
  auto triplet_list = Bdb_tokens::tokenize_triplets(text, ' ');
  triplets_count = static_cast<int>(triplet_list.size());
  std::map<std::string, int> triplet_count_map;
  for (auto const &triplet: triplet_list)
    triplet_count_map[triplet]++;
  for (auto const &triplet_occurence_entry: triplet_count_map)
    list.emplace_back(triplet_occurence_entry.first,
                      triplet_occurence_entry.second);
}

// Bdb_triplets_DTO methods

Bdb_triplets_DTO::Bdb_triplets_DTO(void *buffer) {
  deserialize(buffer);
}

size_t Bdb_triplets_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(triplet);
  len += sizeof(int);
  for (const auto &bdb_id_occurrence: list) {
    len += Bdb_serialization::buffer_len_string(bdb_id_occurrence.id);
    len += sizeof(bdb_id_occurrence.triplets_count);
    len += sizeof(bdb_id_occurrence.occurrence_count);
  }
  return len;
}

void *Bdb_triplets_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(triplet, p);
  int n_id_triplets = static_cast<int>(list.size());
  p = (unsigned char *) Bdb_serialization::deserialize_int(n_id_triplets, p);
  for (int i = 0; i < n_id_triplets; i++) {
    Bdb_text_id_occurence bdb_text_id_occurence;
    p = (unsigned char *) bdb_text_id_occurence.deserialize(p);
    list.push_back(bdb_text_id_occurence);
  }
  return p;
}

void *Bdb_triplets_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(triplet, p);
  int n_id_triplets = static_cast<int>(list.size());
  p = (unsigned char *) Bdb_serialization::serialize_int(n_id_triplets, p);
  for (const auto &id_triplet_occurrence: list) {
    p = (unsigned char *) id_triplet_occurrence.serialize(p);
  }
  return p;
}

// Bdb_triplets_DTO_key methods

Bdb_triplets_DTO_key::Bdb_triplets_DTO_key(const Bdb_triplets_DTO &bdb_triplets_dto) :
    triplet(bdb_triplets_dto.triplet) {
}

Bdb_triplets_DTO_key::Bdb_triplets_DTO_key(std::string triplet_) :
    triplet(std::move(triplet_)) {
}

Bdb_triplets_DTO_key::Bdb_triplets_DTO_key(void *buffer) {
  deserialize(buffer);
}

size_t Bdb_triplets_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(triplet);
  return len;
}

void *Bdb_triplets_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(triplet, p);
  return p;
}

void *Bdb_triplets_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(triplet, p);
  return p;
}
