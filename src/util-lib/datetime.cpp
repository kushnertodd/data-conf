#include <ctime>
#include <iomanip>
#include <sstream>
#include "bdb_serialization.hpp"
#include "datetime.hpp"

Datetime::Datetime() :
    epoch_time(now()) {
}

Datetime::Datetime(std::tm *tm) {
  from_tm(tm);
}

Datetime::Datetime(time_t epoch_time_) : epoch_time(epoch_time_) {}

// default format is "mm/dd/yyyy"
Datetime::Datetime(const std::string &date_time_str, Bdb_errors &errors, const std::string &format) {
  from_string(date_time_str, errors, format);
}

size_t Datetime::buffer_size() {
  return sizeof(epoch_time);
}

std::string Datetime::day_to_string(bool zero_prefix) {
  if (zero_prefix) {
    char buf[3];
    sprintf(buf, "%02d", day());
    return buf;
  } else
    return std::to_string(day());
}

double Datetime::days_before(time_t start_time) const {
  return (double) (start_time - epoch_time) / (60.0 * 60.0 * 24.0);
}

double Datetime::days_since(time_t start_time) const {
  return (double) (epoch_time - start_time) / (60.0 * 60.0 * 24.0);
}

void *Datetime::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_long(epoch_time, p);
  return p;
}

// default format is "mm/dd/yyyy"
void Datetime::from_string(const std::string &date_time_str, Bdb_errors &errors, const std::string &format) {
  struct std::tm when{};
  std::stringstream s(date_time_str);
  s >> std::get_time(&when, format.c_str());
  if (s.fail())
    errors.add("Datetime::Datetime", "1", "Date/time format error: '" + date_time_str + "' format '" + format + "'");
  else
    from_tm(&when);
}

void Datetime::from_time(time_t epoch_time_) {
  epoch_time = epoch_time_;
}

void Datetime::from_tm(std::tm *tm) {
  epoch_time = std::mktime(tm);
}

std::string Datetime::hour_to_string(bool zero_prefix) {
  if (zero_prefix) {
    char buf[3];
    sprintf(buf, "%02d", hour());
    return buf;
  } else
    return std::to_string(hour());
}

std::string Datetime::minute_to_string(bool zero_prefix) {
  if (zero_prefix) {
    char buf[3];
    sprintf(buf, "%02d", minute());
    return buf;
  } else
    return std::to_string(minute());
}

std::string Datetime::month_to_string(bool zero_prefix) {
  if (zero_prefix) {
    char buf[3];
    sprintf(buf, "%02d", month());
    return buf;
  } else
    return std::to_string(month());
}

std::string Datetime::second_to_string(bool zero_prefix) {
  if (zero_prefix) {
    char buf[3];
    sprintf(buf, "%02d", second());
    return buf;
  } else
    return std::to_string(second());
}

void *Datetime::serialize(void *buffer) {
  return Bdb_serialization::serialize_long(epoch_time, buffer);
}

// formats as "mm/dd/yyyy"
std::string Datetime::to_string() {
  std::ostringstream os;
  // we'll optimistically assume this succeeds
  os << std::put_time(to_tm(), "%m/%d/%Y");
  return os.str();
}

std::string Datetime::to_string(const std::string &format, Bdb_errors &errors) {
  std::ostringstream os;
  os << std::put_time(to_tm(), format.c_str());
  if (os.fail()) {
    errors.add("Datetime::to_string", "1", "Date/time format error: '" + format + "'");
    return "";
  } else
    return os.str();
}

std::tm *Datetime::to_tm() {
  return std::localtime(&epoch_time);
}

std::string Datetime::week_day_to_string(bool zero_prefix) {
  if (zero_prefix) {
    char buf[3];
    sprintf(buf, "%02d", week_day());
    return buf;
  } else
    return std::to_string(week_day());
}

// no two-digit format
std::string Datetime::year_to_string() {
  return std::to_string(year());
}

std::string Datetime::year_day_to_string() {
  return std::to_string(year_day());
}

