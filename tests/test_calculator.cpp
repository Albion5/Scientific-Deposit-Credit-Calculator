#include "tests_entry.h"

TEST(tokenizer, test1) {
  TestRunner test_runner("valid_expressions.dat");
  test_runner.RunTokenizerTests();
}

TEST(tokenizer, test2) {
  TestRunner test_runner("simple_expressions.dat");
  test_runner.RunTokenizerTests();
}

TEST(parser, test1) {
  TestRunner test_runner("simple_expressions.dat");
  test_runner.RunParserTests();
}

TEST(parser, test2) {
  TestRunner test_runner("valid_expressions.dat");
  test_runner.RunParserTests();
}

TEST(calculator, test1) {
  TestRunner test_runner("simple_expressions.dat");
  test_runner.RunCalculatorTests();
}

TEST(calculator, test2) {
  TestRunner test_runner("valid_expressions.dat");
  test_runner.RunCalculatorTests();
}

TEST(test_runner, test1) {
  TestRunner test_runner("valid_expressions.dat");
  test_runner.RunAllTests();
}

TEST(test_runner, test2) {
  TestRunner test_runner("valid_expressions.dat");
  test_runner.RunAllTests();
  test_runner.SetData("simple_expressions.dat");
  test_runner.RunAllTests();
}

TEST(tokenizer, invalid_expressions) {
  TestRunner test_runner("invalid_expressions_for_tokenizer.dat");
  test_runner.RunTokenizerTests(true);
}

TEST(parser, invalid_expressions) {
  TestRunner test_runner("invalid_expressions_for_parser.dat");
  test_runner.RunTokenizerTests();
  test_runner.RunParserTests(true);
}

TEST(calculator, invalid_expressions) {
  TestRunner test_runner("invalid_expressions_for_calculator.dat");
  test_runner.RunParserTests();
  test_runner.RunCalculatorTests(true);
}

TEST(calculator, range) {
  SciCalculator calculator;
  double x_start = -100;
  double x_end = -20;
  double step = (x_end - x_start) / (NUM_POINTS - 2);
  vector<std::pair<double, double>> points;
  double exp_x = x_start;
  calculator.CalculateGraphPoints("sin(x)-cos(x)", "", x_start, x_end, points);
  for (auto &[fact_x, fact_y] : points) {
    double exp_y = sin(exp_x) - cos(exp_x);
    ASSERT_NEAR(exp_y, fact_y, test_utils::GetToleranceScale(exp_y, fact_y));
    ASSERT_NEAR(exp_x, fact_x, test_utils::GetToleranceScale(exp_x, fact_x));
    exp_x += step;
  }
}

TEST(calculator, range2) {
  SciCalculator calculator;
  double x_start = -100;
  double x_end = -20;
  vector<std::pair<double, double>> points;
  calculator.CalculateGraphPoints("sin(x)-cos(x)", "", x_start, x_end, points);
  double res = calculator.GetAnswer();
  double exp_res = sin(0) - cos(0);
  ASSERT_NEAR(res, exp_res, test_utils::GetToleranceScale(res, exp_res));

  calculator.CalculateGraphPoints("", "", x_start, x_end, points);
  res = calculator.GetAnswer();
  exp_res = 0;
  ASSERT_NEAR(res, exp_res, test_utils::GetToleranceScale(res, exp_res));
  ASSERT_ANY_THROW(
      calculator.CalculateGraphPoints("!", "", x_start, x_end, points););
}

TEST(calculator, batman) {
  SciCalculator calculator;
  std::string expr =
      "2*sqrt(-abs(abs(X)-1)*abs(3-abs(X))/((abs(X)-1)*(3-abs(X))))";
  calculator.Calculate(expr, "");
  double res1 = calculator.GetAnswer();
  double X = 0;
  double exp_res1 = 2 * sqrt(-abs(abs(X) - 1) * abs(3 - abs(X)) /
                             ((abs(X) - 1) * (3 - abs(X))));
  expr = "(1+abs(abs(X)-3)/(abs(X)-3))*sqrt(1-(X/7)*(X/7))";
  calculator.Calculate(expr, "");
  double res2 = calculator.GetAnswer();
  double exp_res2 =
      (1 + abs(abs(X) - 3) / (abs(X) - 3)) * sqrt(1 - (X / 7) * (X / 7));
  expr = "(5+0.97*(abs(X-0.5)+abs(X+0.5))-3*(abs(X-0.75)+abs(X+0.75)))";
  calculator.Calculate(expr, "");
  double res3 = calculator.GetAnswer();
  double exp_res3 = (5 + 0.97 * (abs(X - 0.5) + abs(X + 0.5)) -
                     3 * (abs(X - 0.75) + abs(X + 0.75)));
  // cout << "\nfact=" << res3 << "; exp_res=" << exp_res3 << "\n";
  expr = "(1+abs(1-abs(X))/(1-abs(X)))";
  calculator.Calculate(expr, "");
  double res4 = calculator.GetAnswer();
  double exp_res4 = (1 + abs(1 - abs(X)) / (1 - abs(X)));
  double final_res = res1 * res2 + res3 * res4;
  double final_exp_res = exp_res1 * exp_res2 + exp_res3 * exp_res4;
  ASSERT_NEAR(final_exp_res, final_res,
              test_utils::GetToleranceScale(final_exp_res, final_res));
}

TEST(calculator, batman2) {
  SciCalculator calculator;
  std::string expr =
      "2*sqrt(-abs(abs(X)-1)*abs(3-abs(X))/"
      "((abs(X)-1)*(3-abs(X))))*(1+abs(abs(X)-3)/(abs(X)-3))*sqrt(1-(X/7)*(X/"
      "7))+(5+0.97*(abs(X-0.5)+abs(X+0.5))-3*(abs(X-0.75)+abs(X+0.75)))*(1+abs("
      "1-abs(X))/(1-abs(X)))";
  calculator.Calculate(expr, "");
  double res1 = calculator.GetAnswer();
  double X = 0;
  double exp_res1 = 2 *
                        sqrt(-abs(abs(X) - 1) * abs(3 - abs(X)) /
                             ((abs(X) - 1) * (3 - abs(X)))) *
                        (1 + abs(abs(X) - 3) / (abs(X) - 3)) *
                        sqrt(1 - (X / 7) * (X / 7)) +
                    (5 + 0.97 * (abs(X - 0.5) + abs(X + 0.5)) -
                     3 * (abs(X - 0.75) + abs(X + 0.75))) *
                        (1 + abs(1 - abs(X)) / (1 - abs(X)));
  ASSERT_NEAR(res1, exp_res1, test_utils::GetToleranceScale(res1, exp_res1));
}

TEST(tokenizer, x_expressions) {
  TestRunner test_runner("expressions_with_x.dat", true);
  test_runner.RunTokenizerTests();
}
TEST(parser, x_expressions) {
  TestRunner test_runner("expressions_with_x.dat", true);
  test_runner.RunParserTests();
}

TEST(calculator, x_expressions) {
  TestRunner test_runner("expressions_with_x.dat", true);
  test_runner.RunCalculatorTests();
}

TEST(calculator, empty) {
  SciCalculator calculator;
  calculator.Calculate("", "");
  ASSERT_DOUBLE_EQ(calculator.GetAnswer(), 0.0);
}
