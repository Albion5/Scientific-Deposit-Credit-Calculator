#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <string>
using std::cout;
using std::function;
using std::list;
using std::map;
using std::regex;
using std::stack;
using std::string;
using std::vector;

namespace s21 {
namespace parser_utils::classes {

template <typename T>
class BasicToken {
 public:
  using token_label_type = T;
  using token_value_type = string;

 public:
  BasicToken<T>() : type_(token_label_type()), value_(token_value_type()) {}
  BasicToken<T>(const token_label_type type, const token_value_type &value)
      : type_(type), value_(value) {}
  ~BasicToken<T>() = default;

  void SetValue(const token_value_type &value);
  void SetType(const token_label_type type);
  const token_value_type &GetValue() const;
  token_label_type GetType() const;

  bool IsValueEq(const token_value_type &value) const;
  bool IsTypeEq(const token_label_type type) const;

 private:
  token_label_type type_;
  token_value_type value_;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class Tokenizer {
 public:
  using Token = BasicToken<T>;
  using token_label_type = typename Token::token_label_type;
  using token_value_type = typename Token::token_value_type;
  using token_list = list<Token>;

 public:
  Tokenizer<T>() = default;
  explicit Tokenizer<T>(const map<token_label_type, string> &token_regex_map,
                        bool case_sensitive = false);

  Tokenizer<T>(const map<token_label_type, regex> &token_regex_map,
               bool case_sensitive);
  ~Tokenizer<T>() = default;

  void SetRegexMap(const map<token_label_type, regex> &token_regex_map,
                   bool case_sensitive);

  void SetRegexMap(const map<token_label_type, string> &token_regex_map,
                   bool case_sensitive = true);
  void Tokenize(const string &input_string,
                const token_label_type type_to_skip);
  void Tokenize(const string &input_string);
  void PrintListOfTokens() const;
  void PrintListOfTokens(const token_list &list_of_tokens) const;
  token_list &GetListOfTokens();
  size_t GetCountByValue(const Token &token) const;
  size_t GetCountByValue(const token_value_type &val) const;
  size_t GetCountByType(const Token &token) const;
  size_t GetCountByType(const token_label_type &type) const;
  void PrintToken(const Token &token) const;
  void Reset();

 private:
  void InitString(const string &input_string);
  void MoveCursor(const size_t step);
  void GetNextToken(const token_label_type *type_to_skip);
  bool HasMoreTokens() const;
  void ResetTokenCount();
  void UpdateTokenCount(const token_label_type type,
                        const token_value_type &val);

 private:
  map<token_value_type, size_t> value_count_map_;
  map<token_label_type, size_t> type_count_map_;
  map<token_label_type, regex> token_regex_map_;
  string::const_iterator cursor_;
  string::const_iterator end_;
  token_list list_of_tokens_;
  bool case_sensitive_;
};

////////////////////////////////////TOKEN//////////////////////////////////////
template <typename T>
void BasicToken<T>::SetValue(const token_value_type &value) {
  value_ = value;
}
template <typename T>
void BasicToken<T>::SetType(const token_label_type type) {
  type_ = type;
}

template <typename T>
const typename BasicToken<T>::token_value_type &BasicToken<T>::GetValue()
    const {
  return value_;
}
template <typename T>
typename BasicToken<T>::token_label_type BasicToken<T>::GetType() const {
  return type_;
}

template <typename T>
bool BasicToken<T>::IsValueEq(const token_value_type &value) const {
  return value_ == value;
}
template <typename T>
bool BasicToken<T>::IsTypeEq(const token_label_type type) const {
  return type_ == type;
}

///////////////////////////////TOKENIZER//////////////////////////////////////

template <typename T>
Tokenizer<T>::Tokenizer(const map<token_label_type, string> &token_regex_map,
                        bool case_sensitive)
    : Tokenizer() {
  SetRegexMap(token_regex_map, case_sensitive);
}
template <typename T>
Tokenizer<T>::Tokenizer(const map<token_label_type, regex> &token_regex_map,
                        bool case_sensitive) {
  SetRegexMap(token_regex_map, case_sensitive);
}
template <typename T>
void Tokenizer<T>::SetRegexMap(
    const map<token_label_type, regex> &token_regex_map, bool case_sensitive) {
  case_sensitive_ = case_sensitive;
  token_regex_map_.clear();
  token_regex_map_ = token_regex_map;
}
template <typename T>
void Tokenizer<T>::SetRegexMap(
    const map<token_label_type, string> &token_regex_map, bool case_sensitive) {
  case_sensitive_ = case_sensitive;
  token_regex_map_.clear();
  regex::flag_type flags = (case_sensitive_) ? std::regex_constants::ECMAScript
                                             : std::regex_constants::icase;
  for (const auto &[type, pattern] : token_regex_map) {
    token_regex_map_.emplace(type, regex(pattern, flags));
  }
}

template <typename T>
void Tokenizer<T>::Tokenize(const string &input_string,
                            const token_label_type type_to_skip) {
  InitString(input_string);
  while (HasMoreTokens()) {
    GetNextToken(&type_to_skip);
  }
}

template <typename T>
void Tokenizer<T>::Tokenize(const string &input_string) {
  InitString(input_string);
  while (HasMoreTokens()) {
    GetNextToken(nullptr);
  }
}

template <typename T>
void Tokenizer<T>::PrintListOfTokens() const {
  PrintListOfTokens(list_of_tokens_);
}

template <typename T>
void Tokenizer<T>::PrintListOfTokens(const token_list &list_of_tokens) const {
  for (const auto &token : list_of_tokens) {
    cout << "{" << token.GetValue() << "} ";
  }
}

template <typename T>
size_t Tokenizer<T>::GetCountByValue(const Token &token) const {
  try {
    return value_count_map_.at(token.GetValue());
  } catch (const std::out_of_range &e) {
    return 0;
  }
}

template <typename T>
size_t Tokenizer<T>::GetCountByValue(const token_value_type &val) const {
  try {
    return value_count_map_.at(val);
  } catch (const std::out_of_range &e) {
    return 0;
  }
}

template <typename T>
size_t Tokenizer<T>::GetCountByType(const Token &token) const {
  try {
    return type_count_map_.at(token.GetValue());
  } catch (const std::out_of_range &e) {
    return 0;
  }
}

template <typename T>
size_t Tokenizer<T>::GetCountByType(const token_label_type &type) const {
  try {
    return type_count_map_.at(type);
  } catch (const std::out_of_range &e) {
    return 0;
  }
}
template <typename T>
void Tokenizer<T>::PrintToken(const Token &token) const {
  cout << "Type: " << token.GetType() << ", Value: " << token.GetValue()
       << "\n";
}
template <typename T>
void Tokenizer<T>::InitString(const string &input_string) {
  list_of_tokens_.clear();
  ResetTokenCount();
  cursor_ = input_string.begin();
  end_ = input_string.end();
}
template <typename T>
void Tokenizer<T>::MoveCursor(const size_t step) {
  cursor_ += step;
}
template <typename T>
void Tokenizer<T>::GetNextToken(const token_label_type *type_to_skip) {
  if (!HasMoreTokens()) return;
  for (const auto &[type, pattern] : token_regex_map_) {
    std::smatch match;
    if (regex_search(cursor_, end_, match, pattern)) {
      MoveCursor(match.length());
      if (type_to_skip && type == *type_to_skip) {
        return GetNextToken(type_to_skip);
      }
      token_value_type val = (case_sensitive_) ? match.str() : [&match]() {
        string str = match.str();
        for (char &c : str) {
          c = std::tolower(c);
        }
        return str;
      }();
      UpdateTokenCount(type, val);
      list_of_tokens_.emplace_back(type, val);
      return;
    }
  }
  // PrintListOfTokens();
  throw std::runtime_error("Error: No valid token found");
}
template <typename T>
bool Tokenizer<T>::HasMoreTokens() const {
  return cursor_ != end_;
}

template <typename T>
void Tokenizer<T>::ResetTokenCount() {
  value_count_map_.clear();
  type_count_map_.clear();
}
template <typename T>
void Tokenizer<T>::UpdateTokenCount(const token_label_type type,
                                    const token_value_type &val) {
  value_count_map_[val]++;
  type_count_map_[type]++;
}

template <typename T>
typename Tokenizer<T>::token_list &Tokenizer<T>::GetListOfTokens() {
  return list_of_tokens_;
}
template <typename T>
void Tokenizer<T>::Reset() {
  ResetTokenCount();
  list_of_tokens_.clear();
}

}  // namespace parser_utils::classes

}  // namespace s21
#endif
