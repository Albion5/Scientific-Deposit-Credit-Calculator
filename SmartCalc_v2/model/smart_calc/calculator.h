#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "third_party/parser.h"

#define NUM_POINTS 20000
namespace s21 {
class SciCalculator {
 public:
  using points_vector = std::vector<std::pair<double, double>>;
  SciCalculator() : answer_(0), x_begin_(0), x_end_(0) {}
  ~SciCalculator() = default;
  void Calculate(const string &input_expression, const string &x_str);
  double GetAnswer();
  bool IsNonLinearFunction();
  void CalculateGraphPoints(const string &input_expression, const string &x_str,
                            double x_start, double x_end,
                            points_vector &points);
  bool HasXChanged(const string &new_x) const;
  void PrintDebugString();

 private:
  using num_stack = stack<double>;
  using token_list = typename MathParser::token_list;
  using Token = typename MathParser::Token;

 private:
  void SetAnswer(double res);
  bool ErrorHappenedLastTime();
  double GetCalculationResult();
  bool HasRangeChanged(double x_begin, double x_end);
  double Calculate(token_list &converted_list_of_tokens, double x);
  void ExecuteUnaryOperation(Token &token);
  void ExecuteBinaryOperation(Token &token);
  double CalculateWithX();
  void SetX(const string &x_str);
  double CalculateWithX(double x);
  void Reset(const std::string &x_st);
  void SendError(const std::string &message);
  void SetRange(double x_start, double x_end);

 private:
  MathParser parser_;
  double answer_;
  std::string error_str_;
  std::string debug_str_;
  string x_str_;
  num_stack numbers_;
  double x_begin_;
  double x_end_;
};
}  // namespace s21
#endif
