#include "credit_utils.h"
namespace s21 {
namespace credit {

long double GetAnnuityFactor(long double interest_rate_monthly,
                             long double n_payments) {
  return (interest_rate_monthly *
          powl((1 + interest_rate_monthly), n_payments)) /
         (powl((1 + interest_rate_monthly), n_payments) - 1);
}

long double Round(long double x, int digits) {
  double scale = std::pow(10.0, digits);
  return std::round(x * scale) / scale;
}

std::pair<int, int> GetDate() {
  time_t cur_time = time(0);
  tm *time_struct = localtime(&cur_time);
  int month_index = time_struct->tm_mon;
  int year = 1900 + time_struct->tm_year;
  return std::make_pair(month_index, year);
}

const std::string GetMonthName(int month_index) {
  return kMonths[month_index % 12];
}

bool IsLeapYear(int year) {
  if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
    return true;  // Leap year
  } else {
    return false;  // Not a leap year
  }
}
unsigned int GetDaysInMonth(unsigned int month_index, int year) {
  int m_i = month_index % 12;
  if (m_i == 1 && IsLeapYear(year)) {
    return 29;
  } else {
    return kDaysInMonth[m_i];
  }
}

int GetDaysInYear(int year) { return IsLeapYear(year) ? 366 : 365; }
PrettyFormatter::PrettyFormatter() {
  oss_ << std::fixed << std::setprecision(2);
}

void PrettyFormatter::Clear() {
  oss_.clear();
  oss_ << std::fixed << std::setprecision(2);
}
const std::string PrettyFormatter::GetFormattedString() { return oss_.str(); }

}  // namespace credit
}  // namespace s21