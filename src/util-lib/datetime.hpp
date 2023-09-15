#pragma once

#include <ctime>
#include "bdb_errors.hpp"
#include "bdb_serialization.hpp"

/*
  The format specifiers match the POSIX function strptime():
  Conversion
  specifier	Explanation	Writes to fields
  %	matches a literal %. The full conversion specification must be %%.	(none)
  t	matches any whitespace.	(none)
  n	matches any whitespace.	(none)
  Year
  Y	parses full year as a 4 digit decimal number, leading zeroes permitted but not required	tm_year
  EY	parses year in the alternative representation, e.g.平成23年 (year Heisei 23) which writes 2011 to tm_year in ja_JP locale	tm_year
  y	parses last 2 digits of year as a decimal number. Range [69,99] results in values 1969 to 1999, range [00,68] results in 2000-2068	tm_year
  Oy	parses last 2 digits of year using the alternative numeric system, e.g. 十一 is parsed as 11 in ja_JP locale	tm_year
  Ey	parses year as offset from locale's alternative calendar period %EC	tm_year
  C	parses the first 2 digits of year as a decimal number (range [00,99])	tm_year
  EC	parses the name of the base year (period) in the locale's alternative representation, e.g. 平成 (Heisei era) in ja_JP	tm_year
  Month
  b	parses the month name, either full or abbreviated, e.g. Oct	tm_mon
  h	synonym of b	tm_mon
  B	synonym of b	tm_mon
  m	parses the month as a decimal number (range [01,12]), leading zeroes permitted but not required	tm_mon
  Om	parses the month using the alternative numeric system, e.g. 十二 parses as 12 in ja_JP locale	tm_mon
  Week
  U	parses the week of the year as a decimal number (Sunday is the first day of the week) (range [00,53]), leading zeroes permitted but not required	tm_year, tm_wday, tm_yday
  OU	parses the week of the year, as by %U, using the alternative numeric system, e.g. 五十二 parses as 52 in ja_JP locale	tm_year, tm_wday, tm_yday
  W	parses the week of the year as a decimal number (Monday is the first day of the week) (range [00,53]), leading zeroes permitted but not required	tm_year, tm_wday, tm_yday
  OW	parses the week of the year, as by %W, using the alternative numeric system, e.g. 五十二 parses as 52 in ja_JP locale	tm_year, tm_wday, tm_yday
  Day of the year/month
  j	parses day of the year as a decimal number (range [001,366]), leading zeroes permitted but not required	tm_yday
  d	parses the day of the month as a decimal number (range [01,31]), leading zeroes permitted but not required	tm_mday
  Od	parses the day of the month using the alternative numeric system, e.g. 二十七 parses as 27 in ja_JP locale, leading zeroes permitted but not required	tm_mday
  e	synonym of d	tm_mday
  Oe	synonym of Od	tm_mday
  Day of the week
  a	parses the name of the day of the week, either full or abbreviated, e.g. Fri	tm_wday
  A	synonym of a	tm_wday
  w	parses weekday as a decimal number, where Sunday is 0 (range [0-6])	tm_wday
  Ow	parses weekday as a decimal number, where Sunday is 0, using the alternative numeric system, e.g. 二 parses as 2 in ja_JP locale	tm_wday
  Hour, minute, second
  H	parses the hour as a decimal number, 24 hour clock (range [00-23]), leading zeroes permitted but not required	tm_hour
  OH	parses hour from 24-hour clock using the alternative numeric system, e.g. 十八 parses as 18 in ja_JP locale	tm_hour
  I	parses hour as a decimal number, 12 hour clock (range [01,12]), leading zeroes permitted but not required	tm_hour
  OI	parses hour from 12-hour clock using the alternative numeric system, e.g. 六 reads as 06 in ja_JP locale	tm_hour
  M	parses minute as a decimal number (range [00,59]), leading zeroes permitted but not required	tm_min
  OM	parses minute using the alternative numeric system, e.g. 二十五 parses as 25 in ja_JP locale	tm_min
  S	parses second as a decimal number (range [00,60]), leading zeroes permitted but not required	tm_sec
  OS	parses second using the alternative numeric system, e.g. 二十四 parses as 24 in ja_JP locale	tm_sec
  Other
  c	parses the locale's standard date and time string format, e.g. Sun Oct 17 04:41:13 2010 (locale dependent)	all
  Ec	parses the locale's alternative date and time string format, e.g. expecting 平成23年 (year Heisei 23) instead of 2011年 (year 2011) in ja_JP locale	all
  x	parses the locale's standard date representation	all
  Ex	parses the locale's alternative date representation, e.g. expecting 平成23年 (year Heisei 23) instead of 2011年 (year 2011) in ja_JP locale	all
  X	parses the locale's standard time representation	all
  EX	parses the locale's alternative time representation	all
  D	equivalent to "%m / %d / %y "	tm_mon, tm_mday, tm_year
  r	parses locale's standard 12-hour clock time (in POSIX, "%I : %M : %S %p")	tm_hour, tm_min, tm_sec
  R	equivalent to "%H : %M"	tm_hour, tm_min
  T	equivalent to "%H : %M : %S"	tm_hour, tm_min, tm_sec
  p	parses the locale's equivalent of a.m. or p.m.	tm_hour
  Note: tm_isdst is not written to, and needs to be set explicitly for use with functions such as mktime
 */

class Datetime : Bdb_serialization {
 public:
  time_t epoch_time{};

  Datetime();
  explicit Datetime(std::tm *tm);
  explicit Datetime(time_t epoch_time_);
  Datetime(const std::string &date_time_str, Bdb_errors &errors, const std::string &format = "%m/%d/%Y");

  size_t buffer_size();
  inline int day() { return to_tm()->tm_mday; }
  std::string day_to_string(bool zero_prefix = false);
  [[nodiscard]] double days_before(time_t start_time) const;
  [[nodiscard]] double days_since(time_t start_time) const;
  void *deserialize(void *buffer);
  void from_string(const std::string &date_time_str, Bdb_errors &errors, const std::string &format = "%m/%d/%Y");
  void from_time(time_t epoch_time);
  void from_tm(std::tm *tm);
  inline int hour() { return to_tm()->tm_hour; }
  std::string hour_to_string(bool zero_prefix = false);
  inline int minute() { return to_tm()->tm_min; }
  std::string minute_to_string(bool zero_prefix = false);
  inline int month() { return to_tm()->tm_mon; }
  std::string month_to_string(bool zero_prefix = false);
  inline static time_t now() { return std::time(nullptr); }
  inline int second() { return to_tm()->tm_sec; }
  std::string second_to_string(bool zero_prefix = false);
  void *serialize(void *buffer);
  inline void set_time(time_t epoch_time_) { epoch_time = epoch_time_; }
  inline void set_now() { epoch_time = now(); }
  std::string to_string();
  std::string to_string(const std::string &format, Bdb_errors &errors);
  std::tm *to_tm();
  [[nodiscard]] inline time_t time() const { return epoch_time; }
  inline int week_day() { return to_tm()->tm_wday; }
  std::string week_day_to_string(bool zero_prefix = false);
  int year() { return to_tm()->tm_year + 1900; }
  std::string year_to_string();
  inline int year_day() { return to_tm()->tm_yday; }
  std::string year_day_to_string();
};
