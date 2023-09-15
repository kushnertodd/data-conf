#include <iomanip>
#include <sstream>
#include "bdb_errors.hpp"
#include "timer.hpp"


Timer::Timer() {
  start();
}

// cpu seconds to execute
double Timer::cpu_milliseconds_seconds_used(int count) const {
  return ((double) 1000.0 * (double) (c_end - c_start)) / ((double) count * CLOCKS_PER_SEC);
}

// stop clock
void Timer::end() {
  c_end = clock();
  t_end = std::chrono::high_resolution_clock::now();
}

// start clock
void Timer::start() {
  c_start = std::clock();
  c_end = std::clock();
  t_start = std::chrono::high_resolution_clock::now();
  t_end = std::chrono::high_resolution_clock::now();
}

json_object *Timer::to_json(Bdb_errors &errors, int count) {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Timer::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  double cpu_milliseconds = cpu_milliseconds_seconds_used(count);
  double wall_clock_milliseconds = wall_clock_milliseconds_used(count);
  json_object_object_add(root, "class_name", json_object_new_string(class_name().c_str()));
  json_object_object_add(root, "cpu_milliseconds",
                         json_object_new_string(std::to_string(cpu_milliseconds).c_str()));
  json_object_object_add(root,
                         "wall_clock_milliseconds",
                         json_object_new_string(std::to_string(wall_clock_milliseconds).c_str()));
  return root;
}

std::string Timer::to_string(int count) {
  std::ostringstream os;
  double cpu_milliseconds = cpu_milliseconds_seconds_used(count);
  double wall_clock_milliseconds = wall_clock_milliseconds_used(count);
  os << "cpu msecs/rec        " << std::setprecision(7) << std::fixed << cpu_milliseconds * 1000.0 << std::endl;
  os << "wall clock msecs/rec " << std::setprecision(7) << std::fixed << wall_clock_milliseconds << std::endl;
  return os.str();
}

// wall clock milliseconds to execute
double Timer::wall_clock_milliseconds_used(int count) {
  return std::chrono::duration<double, std::milli>(t_end - t_start).count() / count;
}


