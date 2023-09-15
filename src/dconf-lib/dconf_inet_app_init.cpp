#include <json-c/json.h>
#include "bdb_errors.hpp"
#include "bdb_file_io.hpp"
#include "misc_utils.hpp"
#include "dconf_inet_app_init.hpp"

std::string Dconf_inet_app_init::app_name{"bdb_unix_lookup"};

Dconf_inet_app_init::Dconf_inet_app_init(int argc,
                                         const char **argv,
                                         Bdb_errors &errors) {
  // https://man7.org/linux/man-pages/man3/getopt.3.html
  int ch;
  while ((ch = getopt(argc, (char **) argv, ":b:d:g:h:p:t:")) != EOF)
    switch (ch) {
      case 'b':
        cache_bytes = Misc_utils::string_to_int(optarg, errors);
        break;
      case 'd':
        if (optarg != nullptr) {
          db_config_filename = optarg;
          have_db_config_filename = true;
        }
        break;
      case 'g':
        cache_gbytes = Misc_utils::string_to_int(optarg, errors);
        break;
      case 'h':
        if (optarg != nullptr) {
          db_home = optarg;
          have_db_home = true;
        }
        break;
      case 'p':
        port = Misc_utils::string_to_int(optarg, errors);
        break;
      case 't':
        if (optarg != nullptr) {
          tsv_home = optarg;
        }
        break;
      case ':':
        errors.add("Dconf_inet_app_init::Dconf_inet_app_init",
                   "1",
                   "Option -" + Misc_utils::char_to_string((char) ch) + " requires an argument");
        break;
      case '?':
        errors.add("Dconf_inet_app_init::Dconf_inet_app_init",
                   "2",
                   "Invalid option -" + Misc_utils::char_to_string((char) ch));
        break;
      default:
        errors.add("Dconf_inet_app_init::init", "1", "Invalid option '-" + Misc_utils::char_to_string((char) ch));
    }
  if (!have_db_config_filename)
    errors.add("Dconf_inet_app_init::init", "2", "-d required");
  if (!have_db_home)
    errors.add("Dconf_inet_app_init::init", "3", "-h required");
  if (!errors.has())
    init(errors);
  else
    usage("");
}

void Dconf_inet_app_init::init(Bdb_errors &errors) {
  std::string directory = Bdb_file_IO::get_current_directory();
  json_object *bdb_json = json_object_from_file(db_config_filename.c_str());
  if (bdb_json != nullptr)
    bdb_databases_config.from_json(bdb_json, errors);
  else
    errors.add("Dconf_inet_app_init::init", "4",
               "invalid json file "
                   + db_config_filename
                   + "(error: " + db_strerror(errno) + ")");
}

void Dconf_inet_app_init::usage(const std::string &message) {
  std::cout << (message.empty() ? app_name + " usage: " : message) << std::endl
            << " -d <request db config json file>" << std::endl
            << " -g <cache gbytes>" << std::endl
            << " -h <database home directory>" << std::endl
            << " -p <port>" << std::endl
            << " -t <tsv file home directory>" << std::endl
            << std::endl;
}
