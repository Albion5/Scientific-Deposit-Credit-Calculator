#ifndef CREDIT_UTILS_H
#define CREDIT_UTILS_H

#include <array>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
namespace s21 {
namespace credit {
enum CreditType { ANNUITY, DIFFERENCIATED };
enum Alignment { LEFT, RIGHT };
static const std::string kMonths[] = {
    "January", "February", "March",     "April",   "May",      "June",
    "July",    "August",   "September", "October", "November", "December"};

static const std::array<unsigned int, 12> kDaysInMonth = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool IsLeapYear(int year);
unsigned int GetDaysInMonth(unsigned int month_index, int year);
int GetDaysInYear(int year);

static constexpr long double kAvgDaysInMonth = 30.4375;
static constexpr long double kAvgDaysInYear = 365.25;

long double GetAnnuityFactor(long double interest_rate_monthly,
                             long double n_payments);
long double Round(long double x, int digits);
std::pair<int, int> GetDate();
const std::string GetMonthName(int month_index);

class PrettyFormatter {
 public:
  PrettyFormatter();

  template <typename... Args>
  void SetAlignment(Args... args) {
    col_alignments_.clear();
    (col_alignments_.emplace_back(args), ...);
  }

  template <typename... Args>
  void SetWidth(Args... args) {
    col_widths_.clear();
    (col_widths_.emplace_back(args), ...);
  }

  template <typename... Args>
  void PrettyFormat(Args... args) {
    auto FormatArgs = [this, i = 0u](auto &&arg) mutable {
      unsigned int width = (i < col_widths_.size()) ? col_widths_[i] : 20;
      Alignment alignment =
          (i < col_alignments_.size()) ? col_alignments_[i] : LEFT;
      oss_ << (alignment == RIGHT ? std::right : std::left) << std::setw(width)
           << arg;
      i++;
    };
    (FormatArgs(args), ...);
    oss_ << "\n";
  }

  template <typename... Args>
  void Format(Args... args) {
    auto FormatArgs = [this](auto &&arg) { oss_ << std::setw(0) << arg; };
    (FormatArgs(args), ...);
    oss_ << "\n";
  }

  template <typename... Args>
  const std::string Concat(Args... args) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    auto FormatArgs = [&](auto &&arg) { oss << arg; };
    (FormatArgs(args), ...);
    return oss.str();
  }

  const std::string GetFormattedString();
  void Clear();
  ~PrettyFormatter() = default;
  using format_type = std::pair<unsigned char, unsigned char>;

 private:
  std::vector<unsigned char> col_widths_;
  std::vector<Alignment> col_alignments_;
  std::ostringstream oss_;
};

}  // namespace credit
}  // namespace s21
#endif