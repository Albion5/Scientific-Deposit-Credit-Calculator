#include "tests_entry.h"

TEST(credit, test1) {
  CreditCalculator calc;
  double total_credit_amount = 50000;
  double interest_rate_annual = 10;
  double term_years = 1;
  double term_months = 0;
  double exp_monthly_payment = 4395.79;
  double exp_overpayment = 2749.48;
  double exp_total_payment = 52749.48;
  calc.Calculate(total_credit_amount, interest_rate_annual, term_years,
                 term_months, credit::ANNUITY);
  ASSERT_FALSE(calc.GetResultString().empty());
  vector<double> result = calc.GetResult();
  double fact_total_payment = result[0];
  double fact_overpayment = result[1];
  double fact_monthly_payment = result[2];
  ASSERT_NEAR(
      fact_total_payment, exp_total_payment,
      test_utils::GetToleranceScale(fact_total_payment, exp_total_payment));
  ASSERT_NEAR(fact_overpayment, exp_overpayment,
              test_utils::GetToleranceScale(fact_overpayment, exp_overpayment));
  ASSERT_NEAR(
      fact_monthly_payment, exp_monthly_payment,
      test_utils::GetToleranceScale(fact_monthly_payment, exp_monthly_payment));
  // The same data => no need to recalculate
  calc.Calculate(total_credit_amount, interest_rate_annual, 0, 12,
                 credit::ANNUITY);
  ASSERT_FALSE(calc.GetResultString().empty());
}

TEST(credit, test2) {
  CreditCalculator calc;
  std::pair<double, double> result;
  calc.GetBarData(result);
  ASSERT_EQ(result.first, 0);
  ASSERT_EQ(result.second, 0);
  double total_credit_amount = 50000;
  double interest_rate_annual = 10;
  double term_years = 1;
  double term_months = 0;
  double exp_min_monthly_payment = 4201.39;
  double exp_max_monthly_payment = 4583.33;
  double exp_overpayment = 2708.33;
  double exp_total_payment = total_credit_amount + exp_overpayment;
  calc.Calculate(total_credit_amount, interest_rate_annual, term_years,
                 term_months, credit::DIFFERENCIATED);
  vector<double> res = calc.GetResult();
  double fact_total_payment = res[0];
  double fact_overpayment = res[1];
  double fact_max_monthly_payment = res[2];
  double fact_min_monthly_payment = res.back();
  ASSERT_FALSE(calc.GetResultString().empty());
  ASSERT_NEAR(
      fact_total_payment, exp_total_payment,
      test_utils::GetToleranceScale(fact_total_payment, exp_total_payment));
  ASSERT_NEAR(fact_overpayment, exp_overpayment,
              test_utils::GetToleranceScale(fact_overpayment, exp_overpayment));
  ASSERT_NEAR(fact_min_monthly_payment, exp_min_monthly_payment,
              test_utils::GetToleranceScale(fact_min_monthly_payment,
                                            exp_min_monthly_payment));
  ASSERT_NEAR(fact_max_monthly_payment, exp_max_monthly_payment,
              test_utils::GetToleranceScale(fact_max_monthly_payment,
                                            exp_max_monthly_payment));
  // The same data => no need to recalculate
  calc.Calculate(total_credit_amount, interest_rate_annual, 0, 12,
                 credit::DIFFERENCIATED);
  ASSERT_FALSE(calc.GetResultString().empty());
  calc.GetBarData(result);
  ASSERT_NEAR(result.first, total_credit_amount,
              test_utils::GetToleranceScale(result.first, total_credit_amount));
  ASSERT_NEAR(result.second, exp_overpayment,
              test_utils::GetToleranceScale(result.second, exp_overpayment));
}

TEST(credit, test3) {
  CreditCalculator calc;
  double total_credit_amount = 0;
  double interest_rate_annual = 15;
  double term_years = 0;
  double term_months = 16;
  ASSERT_THROW(calc.Calculate(total_credit_amount, interest_rate_annual,
                              term_years, term_months, credit::DIFFERENCIATED),
               std::invalid_argument);
  ASSERT_THROW(calc.GetResult(), std::invalid_argument);
  ASSERT_THROW(calc.GetResultString(), std::invalid_argument);
}
