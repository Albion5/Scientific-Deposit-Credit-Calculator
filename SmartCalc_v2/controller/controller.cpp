#include "controller.h"
namespace s21 {
const string Controller::Calculate(const string &input_expression,
                                   const string &x_str) {
  try {
    model_->Calculate(input_expression, x_str);

    return model_->GetScientificResult();
  } catch (const std::exception &ex) {
    return ex.what();
  }
}

const string Controller::CalculateGraphPoints(
    const string &input_expression, const string &x_str, double x_min,
    double x_max, vector<std::pair<double, double>> &points) {
  try {
    model_->CalculateGraphPoints(input_expression, x_str, x_min, x_max, points);
    return model_->GetScientificResult();
  } catch (const std::exception &ex) {
    return ex.what();
  }
}
const string Controller::CalculateCredit(int loan, int interest, int n_years,
                                         int n_months, int type) {
  try {
    model_->CalculateCredit(loan, interest, n_years, n_months, type);
    return model_->GetCreditResult();
  } catch (const std::exception &ex) {
    return ex.what();
  }
}
const string Controller::CalculateCreditBarData(
    std::pair<double, double> &points) {
  try {
    model_->GetCreditBarData(points);
    return model_->GetCreditResult();
  } catch (const std::exception &ex) {
    return ex.what();
  }
}

const string Controller::CalculateDeposit(
    const date_struct &start_date, double initial_deposit, double interest_rate,
    int p, const period_struct &period, const balance_history &replenishments,
    const balance_history &withdrawals, bool capitalization,
    long double tax_rate) {
  try {
    model_->CalculateDeposit(start_date, initial_deposit, interest_rate, p,
                             period, replenishments, withdrawals,
                             capitalization, tax_rate);
    return model_->GetDepositResult();
  } catch (const std::exception &ex) {
    return ex.what();
  }
}

const string Controller::CalculateDepositBarData(
    std::pair<double, double> &points) {
  try {
    model_->GetDepositBarData(points);
    return model_->GetDepositResult();
  } catch (const std::exception &ex) {
    return ex.what();
  }
}
}  // namespace s21