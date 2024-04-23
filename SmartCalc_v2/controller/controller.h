#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../model/model.h"
namespace s21 {
class Controller {
 private:
  Model *model_;
  using points_vector = SciCalculator::points_vector;

 public:
  explicit Controller(Model *m) : model_(m){};
  const string Calculate(const string &input_expression, const string &x_str);

  const string CalculateGraphPoints(const string &input_expression,
                                    const string &x_str, double x_min,
                                    double x_max,
                                    vector<std::pair<double, double>> &points);
  const string CalculateCreditBarData(std::pair<double, double> &points);

  const string CalculateCredit(int loan, int interest, int n_years,
                               int n_months, int type);

  const string CalculateDeposit(const date_struct &start_date,
                                double initial_deposit, double interest_rate,
                                int p, const period_struct &period,
                                const balance_history &replenishments,
                                const balance_history &withdrawals,
                                bool capitalization, long double tax_rate);
  const string CalculateDepositBarData(std::pair<double, double> &points);
};
}  // namespace s21

#endif
