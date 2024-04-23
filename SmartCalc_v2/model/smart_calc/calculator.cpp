#include "calculator.h"
namespace s21 {
void SciCalculator::Calculate(const string &input_expression,
                              const string &x_str) {
  if (input_expression.empty()) {
    Reset(x_str);
    return;
  }
  try {
    bool state_changed = parser_.Parse(input_expression);
    if (!state_changed && ErrorHappenedLastTime()) SendError(error_str_);
    if (state_changed || HasXChanged(x_str)) {
      SetX(x_str);
      SetAnswer(CalculateWithX());
    }
  } catch (const std::exception &ex) {
    SendError(ex.what());
  }
}

bool SciCalculator::ErrorHappenedLastTime() { return !error_str_.empty(); }

bool SciCalculator::IsNonLinearFunction() {
  return x_str_.empty() && parser_.StringContainsX();
}

void SciCalculator::SetAnswer(double res) {
  answer_ = res;
  error_str_.clear();
  // PrintDebugString();
}

double SciCalculator::GetCalculationResult() {
  if (numbers_.size() != 1) throw std::invalid_argument("Calculation error");
  double res = numbers_.top();
  numbers_.pop();
  return res;
}

bool SciCalculator::HasRangeChanged(double x_begin, double x_end) {
  return x_begin != x_begin_ || x_end != x_end_;
}
double SciCalculator::CalculateWithX() {
  if (x_str_.empty()) {
    return Calculate(parser_.GetConvertedListOfTokens(), 0.0);
  }
  return Calculate(parser_.GetConvertedListOfTokens(), std::stod(x_str_));
}

double SciCalculator::CalculateWithX(double x) {
  return Calculate(parser_.GetConvertedListOfTokens(), x);
}

bool SciCalculator::HasXChanged(const string &new_x) const {
  return x_str_ != new_x;
}

void SciCalculator::SetX(const string &x_str) { x_str_ = x_str; }
double SciCalculator::GetAnswer() {
  if (!error_str_.empty()) SendError(error_str_);
  return answer_;
}

double SciCalculator::Calculate(list<Token> &converted_list_of_tokens,
                                double x) {
  debug_str_.clear();
  while (!numbers_.empty()) {
    numbers_.pop();
  }
  for (auto &token : converted_list_of_tokens) {
    if (parser_utils::IsNumber(token)) {
      numbers_.emplace(std::stod(token.GetValue()));
    } else if (parser_utils::IsX(token)) {
      numbers_.emplace(x);
    } else if (functions::IsUnaryFunc(token.GetValue())) {
      if (numbers_.empty()) {
        throw std::invalid_argument(
            "Insufficient number of operands for unary operation");
      }
      ExecuteUnaryOperation(token);
    } else {
      if (numbers_.size() < 2) {
        throw std::invalid_argument(
            "Insufficient number of operands for binary operation");
      }
      ExecuteBinaryOperation(token);
    }
  }
  return GetCalculationResult();
}
void SciCalculator::PrintDebugString() { cout << debug_str_; }
void SciCalculator::ExecuteUnaryOperation(Token &token) {
  double n = numbers_.top();
  numbers_.pop();
  debug_str_ +=
      "\n=== " + token.GetValue() + " " + std::to_string(n) + " ===\n";
  numbers_.push(functions::GetUnaryFunc(token.GetValue())(n));
}

void SciCalculator::ExecuteBinaryOperation(Token &token) {
  double n2 = numbers_.top();
  numbers_.pop();
  double n1 = numbers_.top();
  numbers_.pop();
  debug_str_ += "\n===" + std::to_string(n1) + " " + token.GetValue() + " " +
                std::to_string(n2) + "===\n";
  numbers_.push(functions::GetBinaryFunc(token.GetValue())(n1, n2));
}

void SciCalculator::CalculateGraphPoints(const string &input_expression,
                                         const string &x_str, double x_start,
                                         double x_end, points_vector &points) {
  // cout << error_str_ << "\n";
  if (input_expression.empty()) {
    points.clear();
    Reset(x_str);
    SetRange(x_start, x_end);
    return;
  }
  try {
    bool state_changed = parser_.Parse(input_expression);
    if (!state_changed && ErrorHappenedLastTime()) SendError(error_str_);
    if (state_changed || HasXChanged(x_str) ||
        HasRangeChanged(x_start, x_end)) {
      points.clear();
      SetX(x_str);
      SetRange(x_start, x_end);
      SetAnswer(CalculateWithX());
      double step = (x_end - x_start) / (NUM_POINTS - 2);
      auto x = x_start;
      while (x < x_end) {
        points.emplace_back(
            x, IsNonLinearFunction() ? CalculateWithX(x) : answer_);
        x += step;
      }
      error_str_.clear();
    }
  } catch (const std::exception &ex) {
    points.clear();
    SendError(ex.what());
  }
}

void SciCalculator::Reset(const std::string &x_str) {
  x_str_ = x_str;
  answer_ = 0;
  error_str_.clear();
}
void SciCalculator::SetRange(double x_start, double x_end) {
  x_begin_ = x_start;
  x_end_ = x_end;
}

void SciCalculator::SendError(const std::string &message) {
  error_str_ = message;
  throw std::invalid_argument(message);
}
}  // namespace s21