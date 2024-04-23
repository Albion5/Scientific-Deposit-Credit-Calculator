#include "deposit.h"
namespace s21 {
void DepositCalculator::Calculate(const date_struct &start_date,
                                  double initial_deposit, double interest_rate,
                                  int p, const period_struct &period,
                                  const balance_history &replenishments,
                                  const balance_history &withdrawals,
                                  bool capitalization, long double tax_rate) {
  try {
    period_data_.InitPeriod(start_date, period, (Periodicity)p, initial_deposit,
                            (long double)interest_rate / 100,
                            (long double)tax_rate / 100, capitalization);
    period_data_.AddDepositChanges(replenishments, withdrawals);
    period_data_.CalculateTotalInterest();
    ConstructResultString();
    error_string_.clear();
  } catch (const std::exception &ex) {
    result_string_.clear();
    error_string_ = ex.what();
    throw;
  }
}

std::tuple<double, double, double> DepositCalculator::GetResult() {
  if (!error_string_.empty()) throw std::invalid_argument(error_string_);
  return {period_data_.GetTotalDeposit(), period_data_.GetTotalInterest(),
          period_data_.GetTotalTaxAmount()};
}

std::string DepositCalculator::GetResultString() {
  if (!error_string_.empty()) throw std::invalid_argument(error_string_);
  return result_string_;
}

void DepositCalculator::ConstructResultString() {
  result_string_.clear();
  PrettyFormatter formatter;
  formatter.SetWidth(21);
  formatter.SetAlignment(Alignment::LEFT);
  const auto &[start_date, end_date] = period_data_.GetTerm();
  formatter.PrettyFormat("Deposit term: ",
                         formatter.Concat(DateToString(start_date), " - ",
                                          DateToString(end_date)));
  formatter.PrettyFormat("Total deposit: ", period_data_.GetTotalDeposit());
  formatter.PrettyFormat("Total interest: ", period_data_.GetTotalInterest());
  formatter.PrettyFormat("Total tax amount: ", period_data_.GetTotalTaxAmount(),
                         "\n");
  result_string_ = formatter.GetFormattedString();
}

void DepositCalculator::GetBarData(std::pair<double, double> &points) {
  if (result_string_.empty()) return;
  points.second = period_data_.GetTotalInterest();
  points.first = period_data_.GetTotalDeposit() - points.second;
}
}  // namespace s21