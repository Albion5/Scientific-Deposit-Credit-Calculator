#include "test_runner.h"

TestDataFactory::TestDataFactory(const std::string& data_file_name,
                                 bool data_with_x)
    : data_file_(test_utils::GetPathString(data_file_name)),
      data_with_x_(data_with_x) {
  CreateTestData();
}

void TestDataFactory::UpdateData(const std::string& data_file_name,
                                 bool data_with_x) {
  data_with_x_ = data_with_x;
  data_file_.close();
  data_file_.open(test_utils::GetPathString(data_file_name));
  CreateTestData();
}

void TestDataFactory::CreateTestData() {
  std::regex data_pattern = (data_with_x_) ? test_utils::kTestDataWithXRegex
                                           : test_utils::kTestDataRegex;
  std::string x_str;
  data_.clear();
  if (!data_file_.is_open()) {
    throw std::runtime_error("Failed to open file: ");
  }
  std::string line;
  while (std::getline(data_file_, line)) {
    std::smatch match;
    if (std::regex_match(line, match, data_pattern)) {
      // cout << "\n" << match[1].str() << "\n";
      // cout << "{" << match[2].str() << "}\n";
      // cout << "{" << ((data_with_x_) ? match[3].str() : x_str) << "}\n";
      data_.emplace_back(match[1].str(), std::stod(match[2].str()),
                         ((data_with_x_) ? match[3].str() : x_str));
    } else {
      std::cerr << "Skipping invalid line: " << line << std::endl;
    }
  }
}

TestDataFactory::data_vector& TestDataFactory::GetData() { return data_; }

TestRunner ::TestRunner(const std::string& data_file_name, bool data_with_x)
    : data_factory_(TestDataFactory(data_file_name, data_with_x)) {}

void TestRunner ::RunTokenizerTests(bool invalid_input) {
  MathParser::Tokenizer tokenizer;
  tokenizer.SetRegexMap(parser_utils::kTokenRegexTypes);
  for (auto& [expr, exp_result, x_str] : data_factory_.GetData()) {
    // std::cout << "\nExpression: " << expr << ", Answer: " << exp_result
    //           << std::endl;
    if (invalid_input) {
      ASSERT_ANY_THROW(tokenizer.Tokenize(expr, parser_utils::Skip));
    } else {
      tokenizer.Tokenize(expr, parser_utils::Skip);
      // tokenizer.PrintListOfTokens();
    }
  }
}

void TestRunner ::RunParserTests(bool invalid_input) {
  MathParser parser;
  for (auto& [expr, exp_result, x_str] : data_factory_.GetData()) {
    // std::cout << "Expression: " << expr << ", Answer: " << exp_result
    //           << std::endl;
    if (invalid_input) {
      ASSERT_ANY_THROW(parser.Parse(expr));
    } else {
      parser.Parse(expr);
    }
  }
}
void TestRunner::RunCalculatorTests(bool invalid_input) {
  SciCalculator calculator;
  for (auto& [expr, exp_result, x_str] : data_factory_.GetData()) {
    // std::cout << "Expression: " << expr << ", Answer: " << exp_result
    //           << ", X: " << x_str << std::endl;
    if (invalid_input) {
      ASSERT_ANY_THROW(calculator.Calculate(expr, x_str));
    } else {
      calculator.Calculate(expr, x_str);
      // std::cout << calculator.GetAnswer() << "\n";
      double fact_result = calculator.GetAnswer();
      double tolerance = test_utils::GetToleranceScale(fact_result, exp_result);
      ASSERT_NEAR(exp_result, fact_result, tolerance);
    }
  }
}

void TestRunner ::RunAllTests() {
  RunTokenizerTests();
  RunParserTests();
  RunCalculatorTests();
}

void TestRunner ::SetData(const std::string& data_file_name, bool data_with_x) {
  data_factory_.UpdateData(data_file_name, data_with_x);
}