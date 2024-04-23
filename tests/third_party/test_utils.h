#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../tests_entry.h"

namespace test_utils {
constexpr double EPS = 1e-7;
constexpr char TEST_DATA_REGEX[] = "([^=]+)\\s*=\\s*([^=]+)";
constexpr char TEST_DATA_WITH_X_REGEX[] =
    "(.+)\\s*=\\s*([^\\s]+)\\s*x=\\s*([^\\s]+)";

constexpr char TEST_DATA_DIR_NAME[] = "data";
const std::regex kTestDataRegex(TEST_DATA_REGEX);
const std::regex kTestDataWithXRegex(TEST_DATA_WITH_X_REGEX);
namespace fs = std::filesystem;
const std::string data_path = []() {
  fs::path cur_file(__FILE__);
  fs::path cur_dir = cur_file.parent_path();
  fs::path data_path = cur_dir / ".." / TEST_DATA_DIR_NAME;
  return data_path.string();
}();
std::string GetPathString(const std::string &data_file_name);
double GetToleranceScale(const double &exp_result, const double &fact_result,
                         double eps = EPS);
}  // namespace test_utils

#endif