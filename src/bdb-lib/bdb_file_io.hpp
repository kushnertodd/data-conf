#pragma once

#include <fstream>      // std::ifstream
#include <iostream>
#include <sstream>
#include "bdb_errors.hpp"

/*!
 * @class to provide comma separated number thousands
 */
class comma_numpunct : public std::numpunct<char> {
 protected:
  char do_thousands_sep() const override {
    return ',';
  }

  std::string do_grouping() const override {
    return "\03";
  }
};

/*!
 * @class Bdb_file_IO wrapper class for file related utility functions
 */
class Bdb_file_IO {
 public:
  static std::string comma_format(long num, int width = 0);
  static void create_directory(std::string &dirname, Bdb_errors &errors);
  static std::string get_current_directory();
  static void progress(int count, int blocksize = 100000);
  static void strerror_safe(char *buf, int buflen, int err_code);
  static std::string to_string(const std::string &filename);
};

/*!
 * @class Bdb_File_IO_text_read wrapper class for input stream
 */
class Bdb_File_IO_text_read {
 private:
  std::string filename;
  std::ifstream in_file;
 public:
  ~Bdb_File_IO_text_read();
  Bdb_File_IO_text_read(std::string filename_, Bdb_errors &errors);
  void open(Bdb_errors &errors);
  void close();
  std::string getfile(Bdb_errors &errors);
  bool getline(std::string &line, Bdb_errors &errors);
};

/*!
 * @class Bdb_File_IO_text_read wrapper class for output stream
 */
class Bdb_File_IO_text_write {
 public:
  std::string filename;
  std::ofstream out_file;
 public:
  ~Bdb_File_IO_text_write();
  Bdb_File_IO_text_write(std::string filename_, Bdb_errors &errors);
  void open(Bdb_errors &errors);
  void close();
  void putline(const std::string &line, Bdb_errors &errors);
};
