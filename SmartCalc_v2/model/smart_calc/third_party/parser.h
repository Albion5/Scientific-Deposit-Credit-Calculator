
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <stack>
#include <string>
#include <vector>

#include "utils.h"

namespace s21 {
class MathParser {
 public:
  using Tokenizer = typename parser_utils::MathTokenizer;
  using Token = typename Tokenizer::Token;
  using token_list = typename Tokenizer::token_list;
  using token_stack = stack<Token>;

 public:
  MathParser();
  // returns true if successful parsing
  // false if no need for parsing
  // throws error otherwise
  bool Parse(const string &input_string);
  void Convert();
  void Reset();

  token_list &GetConvertedListOfTokens();
  bool HasInputStringChanged(const string &new_string) const;
  bool StringContainsX();

 private:
  void HandleUnarySign(const Token &prev_token, Token &cur_token);
  void HandleOperator(Token &token, token_stack &operators);
  void HandleRightParenthesis(token_stack &operators);
  void HandleFunctionCall(token_stack &operators);
  void ValidateForm();

 private:
  Tokenizer tokenizer_;
  parser_utils::Validator validator_;
  token_list converted_list_of_tokens_;
  string input_string_;
};
}  // namespace s21

#endif
