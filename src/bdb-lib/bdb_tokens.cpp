#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include "bdb_tokens.hpp"

/*
std::map<std::string, int> Bdb_tokens::triplet_stoplist = {
    {"the", 1},
    {"ing", 1},
    {"and", 1},
    {"ter", 1},
    {"man", 1},
    {"ion", 1},
};
*/
std::vector<std::string> Bdb_tokens::triplet_stoplist = {
    // calculated for top 8000 title names from title.ratings.tsv/title.basic.tsv
    "the", // 14317
    "iso", // 10237
    "ode", // 10232
    "epi", // 10179
    "pis", // 10154
    "sod", // 10134
    "ing", // 3652
    "and", // 2789
    "ter", // 2313
    "ate", // 2181
//    "ion", // 1756
//    "man", // 1737
//    "ted", // 1656
//    "ers", // 1623
//    "her", // 1468
//    "ent", // 1446
//    "igh", // 1424
//    "dat", // 1396
//    "all", // 1372
//    "ght", // 1338
//    "ove", // 1239
//    "ill", // 1203
//    "art", // 1181
//    "ste", // 1124
//    "ver", // 1098
//    "der", // 1057
//    "ast", // 1055
//    "tio", // 1044
//    "rea", // 1040
//    "for", // 1034
//    "one", // 1014
};

bool Bdb_tokens::in_triplet_stoplist(const std::string &triplet) {
  if (std::find(triplet_stoplist.begin(),
                triplet_stoplist.end(), triplet) != triplet_stoplist.end())
    return true;
  else
    return false;
}

/*!
 * @brief tokenize string per delimiter character
 * @param s string to tokenize
 * @param delim delimiter, default tab
 * @return token vector
 */
std::vector<std::string> Bdb_tokens::tokenize(const std::string &s, const char delim) {
  std::vector<std::string> result;
  std::string::size_type from = 0;
  std::string::size_type stop = s.find(delim);
  while (stop != std::string::npos) {
    result.push_back(s.substr(from, stop - from));
    from = stop + 1;
    stop = s.find(delim, from);
  }
  result.push_back(s.substr(from));
  return result;
}

/**
 * @brief prints line with visible non-printables, includes number if count >= 0
 */
std::string Bdb_tokens::line_print(int count, const std::string &str) {
  std::ostringstream os;
  if (count > -1)
    os << count << ": ";
  for (const char &c: str) {
    if (c == '\t') os << "<tab>";
    else if (c == '\n') os << "<nl>";
    else os << c;
  }
  return os.str();
}

std::vector<std::string> Bdb_tokens::tokenize_triplets(const std::string &s, char delim, bool check_stoplist) {
  // preserve only alpha and blank characters
  std::string filtered;
  for (char c: s)
    if (std::isalnum(c) || std::isblank(c))
      filtered += c;
  auto tokens = Bdb_tokens::tokenize(filtered, delim);
  std::vector<std::string> triplets;
  std::vector<std::string> result;
  for (const std::string &token: tokens) {
    std::string str_lowercase = token;
    transform(str_lowercase.begin(), str_lowercase.end(), str_lowercase.begin(), ::tolower);
    std::transform(str_lowercase.begin(), str_lowercase.end(), str_lowercase.begin(), ::tolower);
    std::size_t length = str_lowercase.size();
    if (length > 2) {
      for (int i = 0; i < length - 2; i++) {
        std::string triplet = str_lowercase.substr(i, 3);
        if (!check_stoplist || !in_triplet_stoplist(triplet))
          result.push_back(str_lowercase.substr(i, 3));
      }
    } else
      result.push_back(str_lowercase);
  }
  return result;
}
