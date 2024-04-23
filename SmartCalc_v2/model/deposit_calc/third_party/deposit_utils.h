#ifndef DEPOSIT_UTILS_H
#define DEPOSIT_UTILS_H
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "../../credit_calc/third_party/credit_utils.h"
#include "date.h"
namespace s21 {
namespace deposit {
using date_struct = std::tuple<unsigned int, unsigned int, int>;
using period_struct = std::tuple<unsigned int, unsigned int, unsigned int>;
using balance_history = std::vector<std::pair<date_struct, double>>;
using credit::Alignment;
using credit::GetDaysInMonth;
using credit::GetDaysInYear;
using credit::IsLeapYear;
using credit::kDaysInMonth;
using credit::PrettyFormatter;
using credit::Round;

enum Periodicity {
  ONCE,
  DAILY,
  WEEKLY,
  MONTHLY,
  QUARTERLY,
  SEMI_ANNUALLY,
  ANNUALLY
};

static const period_struct kFullPeriod = std::make_tuple(0, 0, 0);
static const period_struct kDay = std::make_tuple(0, 0, 1);
static const period_struct kWeek = std::make_tuple(0, 0, 7);
static const period_struct kMonth = std::make_tuple(0, 1, 0);
static const period_struct kQuarter = std::make_tuple(0, 3, 0);
static const period_struct kSixMonths = std::make_tuple(0, 6, 0);
static const period_struct kYear = std::make_tuple(1, 0, 0);

static const period_struct kPeriods[7] = {
    kFullPeriod, kDay, kWeek, kMonth, kQuarter, kSixMonths, kYear};

// PERIOD
class Period {
 public:
  Period()
      : start_date_(date_struct()), end_date_{date_struct()}, dur_in_days_(0) {}
  Period(const date_struct &start_date, const period_struct &period);
  Period(const date_struct &start_date, const date_struct &end_date);
  ~Period() = default;

  const date_struct &GetStartDate();
  const date_struct &GetEndDate();
  unsigned int GetDuration();

  void SetStartDate(const date_struct &start_date);
  void SetEndDate(const date_struct &end_date);
  void SetNewPeriod(const date_struct &start_date, const period_struct &period);
  void SetNewPeriod(const Period &other);

 private:
  const date::sys_days ToSysDays(const date_struct &date1);

 private:
  date_struct start_date_;
  date_struct end_date_;
  unsigned int dur_in_days_;
};

// DEPOSIT_PERIOD
class DepositPeriod : public Period {
 public:
  DepositPeriod() : Period(), deposit_(0), interest_(0), rate_(0) {}
  ~DepositPeriod() = default;

  void SetNewData(long double new_deposit, long double new_interest,
                  long double new_rate);
  void SetInterest(long double interest);
  void SetDeposit(long double deposit);
  void SetRate(long double rate);
  void AddInterest(long double amount);
  void AddDeposit(long double amount);

  void CalculateInterest();
  void Capitalize();
  void RoundInterest();

  long double GetInterest();
  long double GetDeposit();
  long double GetRate();

 private:
  long double deposit_;
  long double interest_;
  long double rate_;
};

// FULL_DEPOSIT_PERIOD
class FullDepositPeriod {
 public:
  FullDepositPeriod() : tax_rate_(0), capitalization_(false) {}
  ~FullDepositPeriod() = default;

  void InitPeriod(const date_struct &start_date, const period_struct &period,
                  Periodicity p, long double deposit, long double rate,
                  long double tax_rate = 0, bool capitalization = false);
  void AddDepositChanges(const balance_history &replenishments,
                         const balance_history &withdrawals);
  void ValidateInputData(const period_struct &period, long double deposit,
                         long double rate);
  void CalculateTotalInterest();
  long double GetTotalDeposit();
  long double GetTotalInterest();
  long double GetTotalTaxAmount();
  std::pair<date_struct, date_struct> GetTerm();

 private:
  void CreatePeriods(const date_struct &start_date,
                     const period_struct &period_duration);
  void UpdateRatePeriod();
  void UpdateRate();
  int GetDepositChanges(const date_struct &date);
  void HandleDepositChanges(DepositPeriod &period);
  void HandleOtherDepositChanges();
  void CalculateInterestForPeriod();
  date_struct GetDateOfRateChange(const date_struct &date);
  long double GetDailyRate(const date_struct &date, long double rate_annual);
  period_struct GetPeriodDuration(Periodicity p);
  Period GetRatePeriod(const date_struct &start_date);

 private:
  std::map<date_struct, std::list<double>> deposit_changes_;
  Period rate_period_;
  DepositPeriod full_period_;
  DepositPeriod cur_period_;
  std::vector<Period> periods_;
  long double tax_rate_;
  long double capitalization_;
};

// HELPERS

int GetYear(const date_struct &date);
bool operator<(const date_struct &date1, const date_struct &date2);
bool operator>=(const period_struct &period1, const period_struct &period2);
date_struct GetNextDate(date_struct date, period_struct period);
void PrintPeriod(period_struct &period);
void PrintDate(const date_struct &date);
std::string DateToString(const date_struct &date);

}  // namespace deposit
}  // namespace s21
#endif
