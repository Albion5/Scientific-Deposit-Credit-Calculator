#include "utils.h"

#include <cmath>
namespace s21 {
namespace parser_utils {

bool Contains(const string &str, const string &substr) {
  return str.find(substr) != string::npos;
}

const map<Type, string> kTokenRegexTypes = {
    {Skip, SPACE_REGEX},
    {Operator, OPER_REGEX},
    {Number, NUM_REGEX},
    {X, X_REGEX},
    {Function, FUNC_REGEX},
    {LeftParenthesis, L_PARENTHESIS_REGEX},
    {RightParenthesis, R_PARENTHESIS_REGEX},
    {Factorial, FACTORIAL_REGEX},

};

const map<string, size_t> kPrioritiesMap = [] {
  map<string, size_t> priorities_map;
  size_t priority = 1;
  for (const auto &oper_group : kTokenValues) {
    for (const string &oper : oper_group) {
      priorities_map[oper] = priority;
    }
    ++priority;
  }
  return priorities_map;
}();

bool IsSign(const Token &token) {
  return token.IsValueEq("+") || token.IsValueEq("-");
}

bool IsUnarySign(const Token &prev_token, const Token &cur_token) {
  return IsSign(cur_token) &&
         (IsSkip(prev_token) || IsOperator(prev_token) ||
          IsFunction(prev_token) || IsLeftParenthesis(prev_token));
}

bool IsSkip(const Token &token) { return token.IsTypeEq(Skip); }
bool IsOperator(const Token &token) { return token.IsTypeEq(Operator); }
bool IsFunction(const Token &token) { return token.IsTypeEq(Function); }
bool IsX(const Token &token) { return token.IsTypeEq(X); }
bool IsNumber(const Token &token) { return token.IsTypeEq(Number); }
bool IsFactorial(const Token &token) { return token.IsTypeEq(Factorial); }
bool IsRightParenthesis(const Token &token) {
  return token.IsTypeEq(RightParenthesis);
}
bool IsLeftParenthesis(const Token &token) {
  return token.IsTypeEq(LeftParenthesis);
}

bool IsRightAssociative(const string &str) { return str == "^"; }

bool CompPrecedence(const Token &t1, const Token &t2) {
  return CompPrecedence(t1.GetValue(), t2.GetValue());
}

bool CompPrecedence(const string &str1, const string &str2) {
  if (kPrioritiesMap.count(str1) == 0 || kPrioritiesMap.count(str2) == 0) {
    throw std::runtime_error("Tokens not found in priorities map");
  }
  size_t t1_priority = kPrioritiesMap.at(str1);
  size_t t2_priority = kPrioritiesMap.at(str2);
  return t1_priority > t2_priority ||
         (t1_priority == t2_priority && !IsRightAssociative(str2));
}

Validator::Validator()
    : m_size_(8), c_matrix_(matrix(m_size_, vector<bool>(m_size_, false))) {
  InitC_Matrix();
}

void Validator::ValidateTokenOrder(const Token &t1, const Token &t2) {
  if (IsRuleException(t1, t2)) return;
  if (AreCompatible(t1.GetType(), t2.GetType())) return;
  std::string error_message = "Order error: {" + t2.GetValue() +
                              "} can't be placed after {" + t1.GetValue() + "}";
  throw std::invalid_argument(error_message);
}

void Validator::ValidateLastToken(const Token &token) {
  if (IsOperator(token) || IsFunction(token) || IsLeftParenthesis(token)) {
    std::string error_message =
        "Last token error: Expression can't be concluded with {" +
        token.GetValue() + "}";
    throw std::invalid_argument(error_message);
  }
}
void Validator::ValidateParentheses(size_t left_count, size_t right_count) {
  if (left_count != right_count)
    throw std::invalid_argument("Error:: Unbalanced parentheses");
}

void Validator::AddRule(Type type, const std::vector<Type> &valid_types) {
  for (const auto &valid_type : valid_types) {
    c_matrix_[type][valid_type] = true;
  }
}
void Validator::AddUniversalRule(Type type) {
  for (size_t i = 0; i < m_size_; i++) {
    c_matrix_[i][type] = true;
    c_matrix_[type][i] = true;
  }
}

void Validator::InitC_Matrix() {
  AddRule(Operator, {Number, X, Function, LeftParenthesis});
  AddRule(Number, {Operator, Factorial, RightParenthesis});
  AddRule(X, {Operator, Factorial, RightParenthesis});
  AddRule(Function, {LeftParenthesis});
  AddRule(Factorial, {Operator, RightParenthesis, Factorial});
  AddRule(LeftParenthesis, {
                               LeftParenthesis,
                               Number,
                               X,
                               Function,
                           });
  AddRule(RightParenthesis, {RightParenthesis, Factorial, Operator});
  AddUniversalRule(Skip);
}

bool Validator::IsRuleException(const Token &t1, const Token &t2) {
  return (IsLeftParenthesis(t1) || IsOperator(t1)) && IsSign(t2);
}

bool Validator::AreCompatible(Type type1, Type type2) {
  return c_matrix_[type1][type2];
}

}  // namespace parser_utils

namespace functions {
const un_func_map kUnaryFuncs = {
    {"abs", [](double x) { return std::fabs(x); }},
    {"acos", [](double x) { return std::acos(x); }},
    {"asin", [](double x) { return std::asin(x); }},
    {"atan", [](double x) { return std::atan(x); }},
    {"sqrt", [](double x) { return std::sqrt(x); }},
    {"cos", [](double x) { return std::cos(x); }},
    {"sin", [](double x) { return std::sin(x); }},
    {"tan", [](double x) { return std::tan(x); }},
    {"log", [](double x) { return std::log10(x); }},
    {"ln", [](double x) { return std::log(x); }},
    {"~", [](double x) { return -(x); }},
    {"#", [](double x) { return x; }},
    {"!",
     [](double n) {
       if (n < 0.0) return std::numeric_limits<double>::quiet_NaN();
       return std::tgamma(n + 1.0);
     }},
};
const bi_func_map kBinaryFuncs = {
    {"mod", [](double x, double y) { return std::fmod(x, y); }},
    {"+", [](double x, double y) { return std::plus<double>()(x, y); }},
    {"-", [](double x, double y) { return std::minus<double>()(x, y); }},
    {"*", [](double x, double y) { return std::multiplies<double>()(x, y); }},
    {"/", [](double x, double y) { return std::divides<double>()(x, y); }},
    {"^", [](double x, double y) { return std::pow(x, y); }}};

bool IsUnaryFunc(const string &func_key) { return kUnaryFuncs.count(func_key); }
bool IsBinaryFunc(const string &func_key) {
  return kBinaryFuncs.count(func_key);
}
const unary_function &GetUnaryFunc(const string &func_key) {
  return kUnaryFuncs.at(func_key);
}
const binary_function &GetBinaryFunc(const string &func_key) {
  return kBinaryFuncs.at(func_key);
}

};  // namespace functions
}  // namespace s21