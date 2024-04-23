#include <iomanip>

#include "tests_entry.h"

TEST(deposit, test1) {
  deposit::date_struct date1 = std::make_tuple(1, 12, 2023);
  deposit::date_struct date2 = std::make_tuple(7, 3, 2024);
  deposit::date_struct date3 = std::make_tuple(29, 4, 2024);
  deposit::date_struct date4 = std::make_tuple(6, 3, 2024);
  deposit::period_struct period1 = std::make_tuple(50, 0, 0);
  DepositCalculator calc;
  std::vector<std::pair<date_struct, double>> replenishments = {{date2, 10000},
                                                                {date3, 2000}};
  std::vector<std::pair<date_struct, double>> withdrawals = {{date4, 10000}};
  double deposit = 5000;
  double interest_rate = 16;
  double tax_rate = 3;
  double exp_deposit = 152444.38;
  double exp_interest = 135444.38;
  double exp_tax = exp_interest * tax_rate * 0.01;
  calc.Calculate(date1, deposit, interest_rate, deposit::Periodicity::ONCE,
                 period1, replenishments, withdrawals, false, tax_rate);
  const auto &[fact_deposit, fact_interest, fact_tax] = calc.GetResult();

  ASSERT_NEAR(fact_deposit, exp_deposit,
              test_utils::GetToleranceScale(fact_deposit, exp_deposit));
  ASSERT_NEAR(fact_interest, exp_interest,
              test_utils::GetToleranceScale(fact_interest, exp_interest));
  ASSERT_NEAR(fact_tax, exp_tax,
              test_utils::GetToleranceScale(fact_tax, exp_tax));
  ASSERT_FALSE(calc.GetResultString().empty());
}

TEST(deposit, test2) {
  deposit::date_struct date1 = std::make_tuple(1, 12, 2023);
  deposit::date_struct date2 = std::make_tuple(7, 3, 2024);
  deposit::date_struct date3 = std::make_tuple(29, 4, 2024);
  deposit::date_struct date4 = std::make_tuple(6, 3, 2024);
  deposit::period_struct period1 = std::make_tuple(37, 0, 0);
  DepositCalculator calc;
  std::vector<std::pair<date_struct, double>> replenishments = {{date2, 10000},
                                                                {date3, 2000}};
  std::vector<std::pair<date_struct, double>> withdrawals = {{date4, 10000}};
  double deposit = 5000;
  double interest_rate = 16;
  double tax_rate = 3;
  double exp_deposit = 6074192.74;
  double exp_interest = 6057192.74;
  double exp_tax = exp_interest * tax_rate * 0.01;
  calc.Calculate(date1, deposit, interest_rate, deposit::Periodicity::WEEKLY,
                 period1, replenishments, withdrawals, true, tax_rate);
  const auto &[fact_deposit, fact_interest, fact_tax] = calc.GetResult();

  ASSERT_NEAR(fact_deposit, exp_deposit,
              test_utils::GetToleranceScale(fact_deposit, exp_deposit, 1e-6));
  ASSERT_NEAR(fact_interest, exp_interest,
              test_utils::GetToleranceScale(fact_interest, exp_interest, 1e-6));
  ASSERT_NEAR(fact_tax, exp_tax,
              test_utils::GetToleranceScale(fact_tax, exp_tax, 1e-6));
  ASSERT_FALSE(calc.GetResultString().empty());
}

TEST(deposit, test3) {
  deposit::date_struct date1 = std::make_tuple(1, 12, 2023);
  deposit::date_struct date2 = std::make_tuple(7, 3, 2024);
  deposit::date_struct date3 = std::make_tuple(29, 4, 2024);
  deposit::date_struct date4 = std::make_tuple(6, 3, 2024);
  deposit::period_struct period1 = std::make_tuple(0, 37, 0);
  DepositCalculator calc;
  std::vector<std::pair<date_struct, double>> replenishments = {{date2, 10000},
                                                                {date3, 2000}};
  std::vector<std::pair<date_struct, double>> withdrawals = {{date4, 1000}};
  double deposit = 5000;
  double interest_rate = 16;
  double tax_rate = 3;
  double exp_deposit = 25312.29;
  double exp_interest = 9312.29;
  double exp_tax = exp_interest * tax_rate * 0.01;
  calc.Calculate(date1, deposit, interest_rate, deposit::Periodicity::MONTHLY,
                 period1, replenishments, withdrawals, true, tax_rate);
  const auto &[fact_deposit, fact_interest, fact_tax] = calc.GetResult();
  ASSERT_FALSE(calc.GetResultString().empty());
  ASSERT_NEAR(fact_deposit, exp_deposit,
              test_utils::GetToleranceScale(fact_deposit, exp_deposit, 1e-6));
  ASSERT_NEAR(fact_interest, exp_interest,
              test_utils::GetToleranceScale(fact_interest, exp_interest, 1e-5));
  ASSERT_NEAR(fact_tax, exp_tax,
              test_utils::GetToleranceScale(fact_tax, exp_tax, 1e-5));
}

TEST(deposit, test4) {
  deposit::date_struct date1 = std::make_tuple(1, 12, 2023);
  deposit::date_struct date2 = std::make_tuple(7, 3, 2024);
  deposit::date_struct date3 = std::make_tuple(29, 4, 2024);
  deposit::date_struct date4 = std::make_tuple(6, 3, 2024);
  deposit::period_struct period1 = std::make_tuple(0, 0, 37);
  DepositCalculator calc;
  std::vector<std::pair<date_struct, double>> replenishments = {{date2, 10000},
                                                                {date3, 2000}};
  std::vector<std::pair<date_struct, double>> withdrawals = {{date4, 1000}};
  double deposit = 5000;
  double interest_rate = 16;
  double tax_rate = 3;
  double exp_deposit = 16081.6;
  double exp_interest = 81.6;
  double exp_tax = exp_interest * tax_rate * 0.01;
  calc.Calculate(date1, deposit, interest_rate, deposit::Periodicity::WEEKLY,
                 period1, replenishments, withdrawals, true, tax_rate);
  const auto &[fact_deposit, fact_interest, fact_tax] = calc.GetResult();

  ASSERT_FALSE(calc.GetResultString().empty());
  ASSERT_NEAR(fact_deposit, exp_deposit,
              test_utils::GetToleranceScale(fact_deposit, exp_deposit));
  ASSERT_NEAR(fact_interest, exp_interest,
              test_utils::GetToleranceScale(fact_interest, exp_interest));
  ASSERT_NEAR(fact_tax, exp_tax,
              test_utils::GetToleranceScale(fact_tax, exp_tax));
}

TEST(deposit, test5) {
  deposit::date_struct date1 = std::make_tuple(1, 12, 2023);
  deposit::date_struct date2 = std::make_tuple(7, 3, 2024);
  deposit::date_struct date3 = std::make_tuple(29, 4, 2024);
  deposit::date_struct date4 = std::make_tuple(6, 3, 2024);
  deposit::period_struct period1 = std::make_tuple(0, 0, 37);
  DepositCalculator calc;
  std::vector<std::pair<date_struct, double>> replenishments = {{date2, 10000},
                                                                {date3, 2000}};
  std::vector<std::pair<date_struct, double>> withdrawals = {{date4, 1000}};
  double deposit = 5000;
  double interest_rate = 16;
  double tax_rate = 3;
  double exp_deposit = 16081.71;
  double exp_interest = 81.71;
  double exp_tax = exp_interest * tax_rate * 0.01;
  calc.Calculate(date1, deposit, interest_rate, deposit::Periodicity::DAILY,
                 period1, replenishments, withdrawals, true, tax_rate);
  const auto &[fact_deposit, fact_interest, fact_tax] = calc.GetResult();

  ASSERT_FALSE(calc.GetResultString().empty());
  ASSERT_NEAR(fact_deposit, exp_deposit,
              test_utils::GetToleranceScale(fact_deposit, exp_deposit));
  ASSERT_NEAR(fact_interest, exp_interest,
              test_utils::GetToleranceScale(fact_interest, exp_interest));
  ASSERT_NEAR(fact_tax, exp_tax,
              test_utils::GetToleranceScale(fact_tax, exp_tax));
}

TEST(deposit, test6) {
  deposit::date_struct date1 = std::make_tuple(1, 1, 2017);
  deposit::period_struct period1 = std::make_tuple(0, 5, 0);
  DepositCalculator calc;
  std::vector<std::pair<date_struct, double>> replenishments;
  std::vector<std::pair<date_struct, double>> withdrawals;
  double deposit = 38000;
  double interest_rate = 4;
  double tax_rate = 2;
  double exp_deposit = 38628.16;
  double exp_interest = 628.16;
  double exp_tax = exp_interest * tax_rate * 0.01;
  calc.Calculate(date1, deposit, interest_rate, deposit::Periodicity::DAILY,
                 period1, replenishments, withdrawals, false, tax_rate);
  const auto &[fact_deposit, fact_interest, fact_tax] = calc.GetResult();

  ASSERT_NEAR(fact_deposit, exp_deposit,
              test_utils::GetToleranceScale(fact_deposit, exp_deposit));
  ASSERT_NEAR(fact_interest, exp_interest,
              test_utils::GetToleranceScale(fact_interest, exp_interest));
  ASSERT_NEAR(fact_tax, exp_tax,
              test_utils::GetToleranceScale(fact_tax, exp_tax));
  ASSERT_FALSE(calc.GetResultString().empty());
}

TEST(deposit, test7) {
  DepositCalculator calc;

  std::pair<double, double> result;
  calc.GetBarData(result);
  ASSERT_EQ(result.first, 0);
  ASSERT_EQ(result.second, 0);
  deposit::date_struct date1 = std::make_tuple(1, 1, 2017);
  deposit::period_struct period1 = std::make_tuple(0, 5, 0);
  std::vector<std::pair<date_struct, double>> replenishments;
  std::vector<std::pair<date_struct, double>> withdrawals;
  double deposit = 38000;
  double interest_rate = 4;
  double tax_rate = 2;
  double exp_deposit = 38634.02;
  double exp_interest = 634.02;
  double exp_tax = exp_interest * tax_rate * 0.01;
  calc.Calculate(date1, deposit, interest_rate, deposit::Periodicity::DAILY,
                 period1, replenishments, withdrawals, true, tax_rate);
  const auto &[fact_deposit, fact_interest, fact_tax] = calc.GetResult();
  ASSERT_NEAR(fact_deposit, exp_deposit,
              test_utils::GetToleranceScale(fact_deposit, exp_deposit));
  ASSERT_NEAR(fact_interest, exp_interest,
              test_utils::GetToleranceScale(fact_interest, exp_interest));
  ASSERT_NEAR(fact_tax, exp_tax,
              test_utils::GetToleranceScale(fact_tax, exp_tax));
  ASSERT_FALSE(calc.GetResultString().empty());
  calc.GetBarData(result);
  ASSERT_NEAR(
      result.first, exp_deposit - exp_interest,
      test_utils::GetToleranceScale(result.first, exp_deposit - exp_interest));
  ASSERT_NEAR(result.second, exp_interest,
              test_utils::GetToleranceScale(result.second, exp_interest));
}

TEST(deposit, test8) {
  deposit::date_struct date1 = std::make_tuple(10, 3, 2024);
  deposit::period_struct period1 = std::make_tuple(0, 5, 0);
  DepositCalculator calc;
  std::vector<std::pair<date_struct, double>> replenishments;
  std::vector<std::pair<date_struct, double>> withdrawals;
  double deposit = 50000;
  double interest_rate = 0;
  double tax_rate = 2;
  ASSERT_THROW(
      calc.Calculate(date1, deposit, interest_rate, deposit::Periodicity::DAILY,
                     period1, replenishments, withdrawals, true, tax_rate),
      std::invalid_argument);

  ASSERT_THROW(calc.GetResult(), std::invalid_argument);
  ASSERT_THROW(calc.GetResultString(), std::invalid_argument);
}

TEST(deposit, test9) {
  deposit::date_struct date1 = std::make_tuple(10, 3, 2028);
  deposit::date_struct date2 = std::make_tuple(10, 3, 2024);
  deposit::period_struct period1 = std::make_tuple(0, 37, 0);
  DepositCalculator calc;
  std::vector<std::pair<date_struct, double>> replenishments = {{date2, 1000}};
  std::vector<std::pair<date_struct, double>> withdrawals;
  double deposit = 1200;
  double interest_rate = 24;
  double tax_rate = 2;
  double exp_deposit = 3510.53;
  double exp_interest = 1310.53;
  double exp_tax = exp_interest * tax_rate * 0.01;
  calc.Calculate(date1, deposit, interest_rate, deposit::Periodicity::WEEKLY,
                 period1, replenishments, withdrawals, true, tax_rate);
  const auto &[fact_deposit, fact_interest, fact_tax] = calc.GetResult();

  ASSERT_NEAR(fact_deposit, exp_deposit,
              test_utils::GetToleranceScale(fact_deposit, exp_deposit));
  ASSERT_NEAR(fact_interest, exp_interest,
              test_utils::GetToleranceScale(fact_interest, exp_interest));
  ASSERT_NEAR(fact_tax, exp_tax,
              test_utils::GetToleranceScale(fact_tax, exp_tax));
  ASSERT_FALSE(calc.GetResultString().empty());
}