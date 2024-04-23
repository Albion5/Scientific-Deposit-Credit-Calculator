#ifndef DEPOSIT_H
#define DEPOSIT_H
#include "third_party/deposit_utils.h"

namespace s21 {
using namespace deposit;
class DepositCalculator {
 public:
  DepositCalculator() = default;
  ~DepositCalculator() = default;

  void Calculate(const date_struct &start_date, double initial_deposit,
                 double interest_rate, int p, const period_struct &period,
                 const balance_history &replenishments,
                 const balance_history &withdrawals,
                 bool capitalization = false, long double tax_rate = 0);

  void GetBarData(std::pair<double, double> &points);
  std::tuple<double, double, double> GetResult();

  std::string GetResultString();

 private:
  void ConstructResultString();

 private:
  FullDepositPeriod period_data_;
  std::string result_string_;
  std::string error_string_;
};
}  // namespace s21

#endif
