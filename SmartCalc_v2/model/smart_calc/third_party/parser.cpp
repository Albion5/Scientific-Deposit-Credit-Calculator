#include "parser.h"

namespace s21 {
///////////////////////////////MATH_PARSER//////////////////////////////
MathParser::MathParser()
    : tokenizer_(), converted_list_of_tokens_(), input_string_() {
  tokenizer_.SetRegexMap(parser_utils::kTokenRegexTypes, false);
}

bool MathParser::Parse(const string &input_string) {
  if (input_string.empty() || !HasInputStringChanged(input_string)) {
    input_string_ = input_string;
    return false;
  }
  input_string_ = input_string;
  tokenizer_.Tokenize(input_string, parser_utils::Skip);
  ValidateForm();
  // tokenizer_.PrintListOfTokens();
  Convert();
  // cout << "\nConverted:\n";
  // tokenizer_.PrintListOfTokens(converted_list_of_tokens_);
  return true;
}

void MathParser::ValidateForm() {
  validator_.ValidateParentheses(tokenizer_.GetCountByValue("("),
                                 tokenizer_.GetCountByValue(")"));
  validator_.ValidateLastToken(tokenizer_.GetListOfTokens().back());
}

void MathParser::Convert() {
  converted_list_of_tokens_.clear();
  Token prev_token;
  token_stack operators;
  for (auto cur_token : tokenizer_.GetListOfTokens()) {
    validator_.ValidateTokenOrder(prev_token, cur_token);
    if (parser_utils::IsNumber(cur_token) || parser_utils::IsX(cur_token) ||
        parser_utils::IsFactorial(cur_token)) {
      converted_list_of_tokens_.push_back(cur_token);
    } else if (parser_utils::IsLeftParenthesis(cur_token) ||
               parser_utils::IsFunction(cur_token)) {
      operators.push(cur_token);
    } else if (parser_utils::IsRightParenthesis(cur_token)) {
      HandleRightParenthesis(operators);
    } else if (parser_utils::IsOperator(cur_token)) {
      HandleUnarySign(prev_token, cur_token);
      HandleOperator(cur_token, operators);
    }
    prev_token = cur_token;
  }
  while (!operators.empty()) {
    converted_list_of_tokens_.push_back(operators.top());
    operators.pop();
  }
}

typename MathParser::token_list &MathParser::GetConvertedListOfTokens() {
  return converted_list_of_tokens_;
}

bool MathParser::HasInputStringChanged(const string &new_string) const {
  return input_string_ != new_string;
}

bool MathParser::StringContainsX() {
  return tokenizer_.GetCountByType(parser_utils::X);
}

void MathParser::HandleUnarySign(const Token &prev_token, Token &cur_token) {
  if (!parser_utils::IsUnarySign(prev_token, cur_token)) return;
  cur_token.IsValueEq("+") ? cur_token.SetValue("#") : cur_token.SetValue("~");
}

void MathParser::HandleOperator(Token &token, token_stack &operators) {
  while (!operators.empty() &&
         parser_utils::CompPrecedence(operators.top(), token)) {
    converted_list_of_tokens_.push_back(operators.top());
    operators.pop();
  }
  operators.push(token);
}

void MathParser::HandleRightParenthesis(token_stack &operators) {
  while (!operators.empty() &&
         !parser_utils::IsLeftParenthesis(operators.top())) {
    converted_list_of_tokens_.push_back(operators.top());
    operators.pop();
  }
  if (!operators.empty() && parser_utils::IsLeftParenthesis(operators.top())) {
    operators.pop();
    HandleFunctionCall(operators);
  }
}

void MathParser::HandleFunctionCall(token_stack &operators) {
  if (!operators.empty() && parser_utils::IsFunction(operators.top())) {
    converted_list_of_tokens_.push_back(operators.top());
    operators.pop();
  }
}

void MathParser::Reset() {
  input_string_.clear();
  converted_list_of_tokens_.clear();
  tokenizer_.Reset();
}
}  // namespace s21
