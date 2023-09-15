#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <utility>
#include <sys/stat.h>
#include "bdb_file_io.hpp"
#ifdef _WIN32
#include <direct.h>
#define get_current_dir _getcwd
#else
#include <unistd.h>
#define get_current_dir getcwd
#endif

/*!
 * @brief format number with comma separated thousands
 * @param num number to format
 * @param width minimum width (default if 0)
 * @return string comma formatted number
 */
std::string Bdb_file_IO::comma_format(long num, int width) {
  std::locale comma_locale(std::locale(), new comma_numpunct());
  std::ostringstream os;
  os.imbue(comma_locale);
  if (width == 0)
    os << num;
  else
    os << std::setw(width) << num;
  return os.str();
}

/*!
 * @brief create filesystem director
 * @param dirname directory to create
 * @param errors create error (with system errno)
 */
void Bdb_file_IO::create_directory(std::string &dirname, Bdb_errors &errors) {
  struct stat info{};
  if (stat(dirname.c_str(), &info) != 0) {
    int ret = mkdir(dirname.c_str(), 0755);
    if (ret)
      errors.add("File_IO::create_directory", "1", "create directory " + dirname + " failed", errno);
  } else if (!(info.st_mode & S_IFDIR))
    errors.add("File_IO::create_directory", "2", dirname + " failed, exists as a file", errno);
}

/*!
 * @brief get current working directory
 */
std::string Bdb_file_IO::get_current_directory() {
  char buf[FILENAME_MAX]; //create string buffer to hold path
  if (get_current_dir(buf, FILENAME_MAX) == nullptr)
    return "";
  std::string current_working_dir(buf);
  return current_working_dir;
}

/*!
 * @brief produce progress dots based on count
 * @param count current count
 * @param blocksize counts before next dot produced, default 100,000
 */
void Bdb_file_IO::progress(int count, int blocksize) {
  if (count % blocksize == 0) {
    std::cerr << std::endl << Bdb_file_IO::comma_format(count, 12) << " ";
    std::flush(std::cerr);
  }
  if (count % 1000 == 0) {
    std::cout << ".";
    std::flush(std::cout);
  }
}

/*!
 * length-constrained version of C strerror
 * @param buf provided buffer for error message
 * @param buflen maximum buffer length
 * @param err_code system call errno
 */
void Bdb_file_IO::strerror_safe(char *buf, int buflen, int err_code) {
  char *error = strerror_r(err_code, buf, buflen);
  if (error)
    snprintf(buf, buflen, "Error: %s", error);
}

/*!
 * format filename in current directory
 * @param filename to format
 * @return "current-directory/filename"
 */
std::string Bdb_file_IO::to_string(const std::string &filename) {
  return Bdb_file_IO::get_current_directory() + "/" + filename;
}

// File_IO_text_read methods

/*!
 * @brief closes file when destructing
 */
Bdb_File_IO_text_read::~Bdb_File_IO_text_read() {
  close();
}

/*!
 * @brief open text file for reading
 * @param filename_ file to open
 * @param errors system open errors
 */
Bdb_File_IO_text_read::Bdb_File_IO_text_read(std::string filename_, Bdb_errors &errors)
    : filename(std::move(filename_)) {
  open(errors);
}

/*!
 * @brief close open file
 */
void Bdb_File_IO_text_read::close() {
  in_file.close();
}

/*!
 * @brief read entire text file
 * @param errors system read errors
 * @return file text contents
 */
std::string Bdb_File_IO_text_read::getfile(Bdb_errors &errors) {
  std::stringstream strStream;
  // https://cplusplus.com/reference/ios/ios/rdbuf/
  strStream << in_file.rdbuf();
  return strStream.str();
}

/*!
 * @brief read next text file line
 * @param errors system read errors
 * @return file text line
 */
bool Bdb_File_IO_text_read::getline(std::string &line, Bdb_errors &errors) {
  std::getline(in_file, line);
  const size_t buflen = 200;
  char buf[buflen];
  if (in_file.bad()) {
    Bdb_file_IO::strerror_safe(buf, buflen, errno);
    errors.add("File_IO_text_read::getline", "1", Bdb_file_IO::to_string(filename) + " (" + std::string(buf) + ")");
    return false;
  } else if (in_file.eof()) {
    return false;
  } else if (in_file.fail()) {
    if (errno) {
      Bdb_file_IO::strerror_safe(buf, buflen, errno);
      errors.add("File_IO_text_read::getline", "2", Bdb_file_IO::to_string(filename) + " (" + std::string(buf) + ")");
    }
    return false;
  }
  return true;
}

/*!
 * @brief open text file for reading
 * @param errors system open errors
 */
void Bdb_File_IO_text_read::open(Bdb_errors &errors) {
  in_file.open(filename);
  const size_t buflen = 200;
  char buf[buflen];
  if (in_file.bad()) {
    Bdb_file_IO::strerror_safe(buf, buflen, errno);
    errors.add("File_IO::open_read", "1", Bdb_file_IO::to_string(filename) + " (" + std::string(buf) + ")");
  } else if (in_file.fail()) {
    Bdb_file_IO::strerror_safe(buf, buflen, errno);
    errors.add("File_IO::open_read", "2", Bdb_file_IO::to_string(filename) + " (" + std::string(buf) + ")");
  } else if (in_file.eof()) {
    Bdb_file_IO::strerror_safe(buf, buflen, errno);
    errors.add("File_IO::open_read", "3", Bdb_file_IO::to_string(filename) + " (" + std::string(buf) + ")");
  }
}

// File_IO_text_write methods

/*!
 * @brief closes file when destructing
 */
Bdb_File_IO_text_write::~Bdb_File_IO_text_write() {
  close();
}

/*!
 * @brief open text file for writing
 * @param filename_ file to open
 * @param errors system open errors
 */
Bdb_File_IO_text_write::Bdb_File_IO_text_write(std::string filename_, Bdb_errors &errors)
    : filename(std::move(filename_)) {
  open(errors);
}

/*!
 * @brief close open file
 */
void Bdb_File_IO_text_write::close() {
  out_file.close();
}

/*!
 * @brief open text file for writing
 * @param errors system open errors
 */
void Bdb_File_IO_text_write::open(Bdb_errors &errors) {

  out_file.open(filename, std::ios::out);
  const size_t buflen = 200;
  char buf[buflen];
  if (out_file.bad()) {
    Bdb_file_IO::strerror_safe(buf, buflen, errno);
    errors.add("File_IO_text_write::open", "1", Bdb_file_IO::to_string(filename) + " (" + std::string(buf) + ")");
  } else if (out_file.fail()) {
    Bdb_file_IO::strerror_safe(buf, buflen, errno);
    errors.add("File_IO_text_write::open", "2", Bdb_file_IO::to_string(filename) + " (" + std::string(buf) + ")");
  } else if (out_file.eof()) {
    Bdb_file_IO::strerror_safe(buf, buflen, errno);
    errors.add("File_IO_text_write::open", "3", Bdb_file_IO::to_string(filename) + " (" + std::string(buf) + ")");
  }
}

/*!
 * @brief write write text file line
 * @param errors system read errors
 * @return file text line
 */
void Bdb_File_IO_text_write::putline(const std::string &line, Bdb_errors &errors) {
  out_file << line << std::endl;
}

