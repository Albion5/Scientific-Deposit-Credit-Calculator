#ifndef TEST_RUNNER_H_
#define TEST_RUNNER_H_

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "test_utils.h"

class TestDataFactory {
 public:
  using data_tuple = std::tuple<std::string, double, std::string>;
  using data_vector = std::vector<data_tuple>;

  TestDataFactory() = default;
  explicit TestDataFactory(const std::string &data_file_name,
                           bool data_with_x = false);
  ~TestDataFactory() = default;

  void UpdateData(const std::string &data_file_name, bool data_with_x = false);
  data_vector &GetData();

 private:
  void CreateTestData();

 private:
  std::ifstream data_file_;
  data_vector data_;
  bool data_with_x_;
};

class TestRunner {
 public:
  TestRunner() = default;
  explicit TestRunner(const std::string &data_file_name,
                      bool data_with_x = false);
  void RunCalculatorTests(bool invalid_input = false);
  void RunParserTests(bool invalid_input = false);
  void RunTokenizerTests(bool invalid_input = false);
  void RunAllTests();
  void SetData(const std::string &data_file_name, bool data_with_x = false);

 private:
  TestDataFactory data_factory_;
};

#endif