#ifndef CREDIT_H
#define CREDIT_H

#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "third_party/credit_utils.h"
namespace s21 {
class CreditCalculator {
 public:
  CreditCalculator()
      : total_credit_amount_(0),
        interest_rate_annual_(0),
        n_payments_(0),
        type_(credit::ANNUITY) {}
  ~CreditCalculator() = default;
  void Calculate(double total_credit_amount, double interest_rate_annual,
                 int term_years, int term_months, int type);
  std::vector<double> GetResult();
  std::string GetResultString();
  void ValidateInput(double total_credit_amount, double interest_rate_annual,
                     int n_payments, int type);
  void GetBarData(std::pair<double, double>& points);

 private:
  void CalculateAnnuity();
  void CalculateDifferenciated();
  bool StateChanged(double credit_amount, double rate_annual, int term,
                    int type);
  void SetInputData(double total_credit_amount, double interest_rate_annual,
                    int n_payments, int type);

  long double GetFixedMonthlyPayment();
  long double GetMonthlyInterest(long double credit_amount_left,
                                 long double interest);

  void ConstructResultString(double monthly_payment, double total_payment,
                             double overpayment);

 private:
  double total_credit_amount_;
  double interest_rate_annual_;
  int n_payments_;
  int type_;
  std::vector<double> result_double_;
  std::string result_string_;
  std::string error_str_;
};
}  // namespace s21

#endif
