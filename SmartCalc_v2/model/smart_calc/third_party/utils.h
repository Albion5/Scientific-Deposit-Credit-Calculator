#ifndef UTILS_H
#define UTILS_H
#include <cmath>
#include <stack>

#include "tokenizer.h"
using std::function;
using std::list;
using std::map;
using std::regex;
using std::stack;
using std::string;
using std::vector;

namespace s21 {
namespace tokens {

namespace patterns {
constexpr char SPACE_REGEX[] = "^\\s+";
constexpr char OPER_REGEX[] = "^(\\+|-|\\*|\\/|\\^|mod)";
constexpr char NUM_REGEX[] = "^(\\d+(\\.?\\d+)?(e[+-]?\\d+)?)";
constexpr char X_REGEX[] = "^x";

constexpr char FUNC_REGEX[] = "^(ln|cos|sin|tan|log|acos|asin|atan|sqrt|abs)";
// constexpr char PARENTHESES_REGEX[] = "^(\\(|\\))";
constexpr char L_PARENTHESIS_REGEX[] = "^\\(";
constexpr char R_PARENTHESIS_REGEX[] = "^\\)";
constexpr char FACTORIAL_REGEX[] = "^!";

}  // namespace patterns

namespace values {

const vector<vector<string>> kTokenValues = {
    {"(", ")"},
    {"+", "-"},
    {"/", "*", "%", "mod"},
    {"cos", "sin", "tan", "acos", "asin", "atan", "ln", "log", "sqrt", "abs"},
    {"^"},
    {"#", "~"},
    {"!"}};
}  // namespace values
namespace types {

enum Type {
  Skip,
  Operator,
  Number,
  X,
  Function,
  LeftParenthesis,
  RightParenthesis,
  Factorial
};
}

};  // namespace tokens

namespace parser_utils {

using namespace tokens::patterns;
using namespace tokens::types;
using namespace tokens::values;
using MathTokenizer = classes::Tokenizer<Type>;
using Token = classes::BasicToken<Type>;

extern const map<Type, string> kTokenRegexTypes;
extern const map<string, size_t> kPrioritiesMap;

bool IsSign(const Token &token);
bool IsUnarySign(const Token &prev_token, const Token &cur_token);
bool IsSkip(const Token &token);
bool IsOperator(const Token &token);
bool IsFunction(const Token &token);
bool IsX(const Token &token);
bool IsNumber(const Token &token);
bool IsFactorial(const Token &token);
bool IsRightParenthesis(const Token &token);
bool IsRightAssociative(const string &str);
bool IsLeftParenthesis(const Token &token);
bool CompPrecedence(const Token &t1, const Token &t2);
bool CompPrecedence(const string &str1, const string &str2);

bool Contains(const string &str, const string &substr);

class Validator {
  using matrix = vector<vector<bool>>;

 public:
  Validator();
  ~Validator() = default;
  void ValidateTokenOrder(const Token &t1, const Token &t2);
  void ValidateLastToken(const Token &token);
  void ValidateParentheses(size_t left_count, size_t right_count);
  void AddRule(Type type, const std::vector<Type> &valid_types);
  void AddUniversalRule(Type type);

 private:
  void InitC_Matrix();
  bool IsRuleException(const Token &t1, const Token &t2);
  bool AreCompatible(Type type1, Type type2);

 private:
  size_t m_size_;
  matrix c_matrix_;
};

}  // namespace parser_utils

namespace functions {

using unary_function = function<double(double)>;
using binary_function = function<double(double, double)>;
using un_func_map = map<string, unary_function>;
using bi_func_map = map<string, binary_function>;

extern const un_func_map kUnaryFuncs;
extern const bi_func_map kBinaryFuncs;

bool IsUnaryFunc(const string &func_key);
bool IsBinaryFunc(const string &func_key);

const unary_function &GetUnaryFunc(const string &func_key);
const binary_function &GetBinaryFunc(const string &func_key);

};  // namespace functions

}  // namespace s21
#endif