#include "credit.h"
namespace s21 {
void CreditCalculator::Calculate(double total_credit_amount,
                                 double interest_rate_annual, int term_years,
                                 int term_months, int type) {
  int n_payments = term_years * 12 + term_months;
  ValidateInput(total_credit_amount, interest_rate_annual, n_payments, type);
  if (!StateChanged(total_credit_amount, interest_rate_annual, n_payments,
                    type)) {
    if (error_str_.empty())
      return;
    else
      throw std::invalid_argument(error_str_);
  }
  result_double_.clear();
  result_string_.clear();
  SetInputData(total_credit_amount, interest_rate_annual, n_payments, type);
  switch (type) {
    case credit::ANNUITY:
      CalculateAnnuity();
      break;
    case credit::DIFFERENCIATED:
      CalculateDifferenciated();
      break;
  }
}
void CreditCalculator::CalculateAnnuity() {
  long double monthly_payment = credit::Round(GetFixedMonthlyPayment(), 2);
  long double total_payment = monthly_payment * n_payments_;
  long double overpayment = total_payment - total_credit_amount_;

  result_double_.push_back(total_payment);
  result_double_.push_back(overpayment);
  for (int i = 0; i < n_payments_; i++) {
    result_double_.push_back(monthly_payment);
  }
  ConstructResultString(monthly_payment, total_payment, overpayment);
  error_str_.clear();
}

void CreditCalculator::ValidateInput(double total_credit_amount,
                                     double interest_rate_annual,
                                     int n_payments, int type) {
  if (total_credit_amount > 0 && interest_rate_annual > 0 && n_payments > 0)
    return;
  result_double_.clear();
  result_string_.clear();
  SetInputData(total_credit_amount, interest_rate_annual, n_payments, type);
  error_str_ = "Error:Invalid input";
  throw std::invalid_argument("Error: Invalid input");
}

void CreditCalculator::CalculateDifferenciated() {
  result_double_.assign(2, 0);
  long double main_credit_part =
      (long double)total_credit_amount_ / n_payments_;
  long double credit_amount_left = total_credit_amount_;
  long double interest = interest_rate_annual_ / 100;

  long double total_payment = 0;
  long double monthly_payment = 0;
  for (int i = 0; i < n_payments_; i++) {
    monthly_payment = credit::Round(
        main_credit_part + GetMonthlyInterest(credit_amount_left, interest), 2);
    result_double_.emplace_back(monthly_payment);
    credit_amount_left -= main_credit_part;
    total_payment += monthly_payment;
  }
  long double overpayment = total_payment - total_credit_amount_;
  result_double_[0] = total_payment;
  result_double_[1] = overpayment;
  ConstructResultString(monthly_payment, total_payment, overpayment);
  error_str_.clear();
}

void CreditCalculator::SetInputData(double total_credit_amount,
                                    double interest_rate_annual, int n_payments,
                                    int type) {
  total_credit_amount_ = total_credit_amount;
  interest_rate_annual_ = interest_rate_annual;
  n_payments_ = n_payments;
  type_ = type;
}

bool CreditCalculator::StateChanged(double credit_amount, double rate_annual,
                                    int n_payments, int type) {
  return credit_amount != total_credit_amount_ ||
         rate_annual != interest_rate_annual_ || n_payments != n_payments_ ||
         type != type_;
}

long double CreditCalculator::GetFixedMonthlyPayment() {
  long double annuity_factor =
      credit::GetAnnuityFactor(interest_rate_annual_ / 1200, n_payments_);
  return annuity_factor * total_credit_amount_;
}

std::vector<double> CreditCalculator::GetResult() {
  if (!error_str_.empty()) throw std::invalid_argument(error_str_);
  return result_double_;
}

std::string CreditCalculator::GetResultString() {
  if (!error_str_.empty()) throw std::invalid_argument(error_str_);
  return result_string_;
}

void CreditCalculator::ConstructResultString(double monthly_payment,
                                             double total_payment,
                                             double overpayment) {
  std::pair<int, int> date = credit::GetDate();
  auto month_index = date.first;
  auto year_value = date.second;

  credit::PrettyFormatter formatter;
  formatter.SetWidth(21);
  formatter.SetAlignment(credit::RIGHT);
  if (type_ == credit::ANNUITY) {
    formatter.PrettyFormat("Monthly payment: ", monthly_payment);
  } else {
    double max_monthly_payment = result_double_[2];
    formatter.PrettyFormat(
        "Monthly payment: ",
        formatter.Concat(max_monthly_payment, "...", monthly_payment));
  }
  formatter.PrettyFormat("Total payment: ", total_payment);
  formatter.PrettyFormat("Overpayment: ", overpayment, "\n");
  formatter.SetWidth(8);
  formatter.SetAlignment(credit::LEFT, credit::LEFT, credit::RIGHT);
  formatter.PrettyFormat("№", "Month", "Payment");
  auto it = result_double_.begin() + 2;
  unsigned int month_num = 1;

  formatter.SetWidth(6);
  for (; it != result_double_.end(); it++) {
    formatter.PrettyFormat(
        formatter.Concat(month_num++, "."),
        formatter.Concat(credit::GetMonthName(month_index), " ", year_value),
        *it);
    month_index++;
    if (month_index >= 12) {
      month_index = 0;
      year_value++;
    }
  }
  result_string_ = formatter.GetFormattedString();
}
long double CreditCalculator::GetMonthlyInterest(long double credit_amount_left,
                                                 long double interest) {
  return credit_amount_left * interest * credit::kAvgDaysInMonth /
         credit::kAvgDaysInYear;
}

void CreditCalculator::GetBarData(std::pair<double, double> &points) {
  if (result_double_.empty()) return;
  points.first = total_credit_amount_;
  points.second = result_double_[1];
  error_str_.clear();
}
}  // namespace s21