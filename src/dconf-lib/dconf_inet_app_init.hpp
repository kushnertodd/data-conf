#pragma once

#include "bdb_databases_config.hpp"
#include "bdb_errors.hpp"

class Dconf_inet_app_init {
 public:
  static std::string app_name;
  const int PORT = 8000;
  bool load_request{};
  int cache_bytes{0};
  int cache_gbytes{4};
  std::string db_config_filename{};
  std::string db_home{"."};
  std::string tsv_home{"."};
  int port{PORT};
  bool have_db_config_filename{false};
  bool have_db_home{false};
  Bdb_databases_config bdb_databases_config;
  Dconf_inet_app_init(int argc,
                    const char **argv,
                    Bdb_errors &errors);
  void init(Bdb_errors &errors);
  static void usage(const std::string &message = "");
};

