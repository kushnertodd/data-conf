#pragma once

#include <chrono>
#include <ctime>
#include <json-c/json.h>
#include "bdb_errors.hpp"

// https://en.cppreference.com/w/cpp/chrono/c/clock
// https://en.cppreference.com/w/c/chrono/clock_t
// https://en.cppreference.com/w/cpp/chrono/high_resolution_clock
// https://en.cppreference.com/w/cpp/chrono/duration
// https://en.cppreference.com/w/cpp/chrono/time_point

class Timer {
 private:
  static std::string class_name(){ return "Timer"; }
  std::clock_t c_start{};
  std::clock_t c_end{};
  std::chrono::time_point<std::chrono::high_resolution_clock> t_start{};
  std::chrono::time_point<std::chrono::high_resolution_clock> t_end{};
 public:
  Timer();
  [[nodiscard]] double cpu_milliseconds_seconds_used(int count = 1) const;
  void end();
  void start();
  json_object *to_json(Bdb_errors &errors, int count = 1);
  std::string to_string(int count = 1);
  double wall_clock_milliseconds_used(int count = 1);
};
