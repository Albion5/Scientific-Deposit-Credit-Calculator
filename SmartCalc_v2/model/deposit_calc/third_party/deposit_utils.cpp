#include "deposit_utils.h"

namespace s21 {
namespace deposit {

// PERIOD
Period::Period(const date_struct &start_date, const period_struct &period)
    : start_date_(start_date),
      end_date_(GetNextDate(start_date, period)),
      dur_in_days_((ToSysDays(end_date_) - ToSysDays(start_date_)).count()) {}

Period::Period(const date_struct &start_date, const date_struct &end_date)
    : start_date_(start_date),
      end_date_(end_date),
      dur_in_days_((ToSysDays(end_date_) - ToSysDays(start_date_)).count()) {}
const date_struct &Period::GetStartDate() { return start_date_; }

const date_struct &Period::GetEndDate() { return end_date_; }
unsigned int Period::GetDuration() { return dur_in_days_; }
void Period::SetEndDate(const date_struct &end_date) {
  end_date_ = end_date;
  dur_in_days_ = (ToSysDays(end_date_) - ToSysDays(start_date_)).count();
}
void Period::SetStartDate(const date_struct &start_date) {
  start_date_ = start_date;
  dur_in_days_ = (ToSysDays(end_date_) - ToSysDays(start_date_)).count();
}
void Period::SetNewPeriod(const date_struct &start_date,
                          const period_struct &period) {
  start_date_ = start_date;
  end_date_ = GetNextDate(start_date, period);
  dur_in_days_ = (ToSysDays(end_date_) - ToSysDays(start_date_)).count();
}

void Period::SetNewPeriod(const Period &other) {
  start_date_ = other.start_date_;
  end_date_ = other.end_date_;
  dur_in_days_ = other.dur_in_days_;
}

const date::sys_days Period::ToSysDays(const date_struct &date1) {
  const auto &[day1, month1, year1] = date1;
  return {date::year{year1} / month1 / day1};
}

// DEPOSIT_PERIOD
void DepositPeriod::SetNewData(long double new_deposit,
                               long double new_interest, long double new_rate) {
  deposit_ = new_deposit;
  interest_ = new_interest;
  rate_ = new_rate;
}

void DepositPeriod::SetInterest(long double interest) { interest_ = interest; }
void DepositPeriod::SetRate(long double rate) { rate_ = rate; }
void DepositPeriod::SetDeposit(long double deposit) { deposit_ = deposit; }

void DepositPeriod::Capitalize() { deposit_ += interest_; }
void DepositPeriod::CalculateInterest() {
  interest_ += deposit_ * GetDuration() * rate_;
}
void DepositPeriod::RoundInterest() { interest_ = Round(interest_, 2); }

void DepositPeriod::AddInterest(long double amount) { interest_ += amount; }
void DepositPeriod::AddDeposit(long double amount) {
  if (deposit_ + amount >= 0) deposit_ += amount;
}
long double DepositPeriod::GetInterest() { return interest_; }
long double DepositPeriod::GetDeposit() { return deposit_; }
long double DepositPeriod::GetRate() { return rate_; }

// FULL_DEPOSIT_PERIOD
void FullDepositPeriod::InitPeriod(const date_struct &start_date,
                                   const period_struct &period, Periodicity p,
                                   long double deposit, long double rate,
                                   long double tax_rate, bool capitalization) {
  ValidateInputData(period, deposit, rate);
  capitalization_ = (p == ONCE) ? false : capitalization;
  tax_rate_ = tax_rate;
  full_period_.SetNewPeriod(start_date, period);
  full_period_.SetNewData(0, 0, rate);
  rate_period_ = GetRatePeriod(start_date);
  CreatePeriods(start_date, GetPeriodDuration(p));
  cur_period_.SetNewData(deposit, 0, GetDailyRate(start_date, rate));
}

void FullDepositPeriod::ValidateInputData(const period_struct &period,
                                          long double deposit,
                                          long double rate) {
  if (period >= kDay && deposit > 0 && rate > 0) return;
  throw std::invalid_argument("Error: Invalid input");
}

void FullDepositPeriod::AddDepositChanges(const balance_history &replenishments,
                                          const balance_history &withdrawals) {
  deposit_changes_.clear();
  auto AddReplenishments = [&](const std::pair<date_struct, double> &pair) {
    const auto &[date, replenishment] = pair;
    deposit_changes_[date].push_back(replenishment);
  };
  auto AddWithdrawals = [&](const std::pair<date_struct, double> &pair) {
    const auto &[date, withdrawal] = pair;
    deposit_changes_[date].push_back(-1 * withdrawal);
  };
  std::for_each(replenishments.begin(), replenishments.end(),
                AddReplenishments);
  std::for_each(withdrawals.begin(), withdrawals.end(), AddWithdrawals);
  int n_changes = GetDepositChanges(full_period_.GetStartDate());
  for (int i = 0; i < n_changes; i++) {
    auto &[date, values] = *deposit_changes_.begin();
    for (const auto &value : values) full_period_.AddDeposit(value);
    deposit_changes_.erase(date);
  }
}

void FullDepositPeriod::UpdateRatePeriod() {
  rate_period_ = GetRatePeriod(rate_period_.GetEndDate());
}

void FullDepositPeriod::CreatePeriods(const date_struct &start_date,
                                      const period_struct &period_duration) {
  periods_.clear();
  long int days_left = full_period_.GetDuration();
  Period cur_period = (period_duration == kFullPeriod)
                          ? rate_period_
                          : Period(start_date, period_duration);
  while (days_left > 0) {
    if (cur_period.GetDuration() > days_left) {
      cur_period.SetEndDate(full_period_.GetEndDate());
    }
    periods_.emplace_back(cur_period);
    days_left -= cur_period.GetDuration();
    if (period_duration == kFullPeriod) {
      cur_period = GetRatePeriod(cur_period.GetEndDate());
    } else {
      cur_period.SetNewPeriod(cur_period.GetEndDate(), period_duration);
    }
  }
}

void FullDepositPeriod::UpdateRate() {
  UpdateRatePeriod();
  cur_period_.SetRate(
      GetDailyRate(rate_period_.GetStartDate(), full_period_.GetRate()));
}

void FullDepositPeriod::CalculateInterestForPeriod() {
  date_struct rate_change = rate_period_.GetEndDate();
  if (rate_change < cur_period_.GetEndDate()) {
    DepositPeriod first_half = cur_period_;
    first_half.SetEndDate(rate_change);
    HandleDepositChanges(first_half);
    UpdateRate();
    DepositPeriod second_half = cur_period_;
    second_half.SetStartDate(rate_change);
    HandleDepositChanges(second_half);
  } else {
    HandleDepositChanges(cur_period_);
  }
  cur_period_.RoundInterest();
  if (capitalization_) cur_period_.Capitalize();
}

int FullDepositPeriod::GetDepositChanges(const date_struct &date) {
  auto it = deposit_changes_.begin();
  while (it != deposit_changes_.end() && (*it).first < date) {
    it++;
  }

  return std::distance(deposit_changes_.begin(), it);
}

void FullDepositPeriod::HandleDepositChanges(DepositPeriod &period) {
  int n_changes = GetDepositChanges(period.GetEndDate());
  DepositPeriod cur_sub_period = period;
  for (int i = 0; i < n_changes; i++) {
    auto &[date, values] = *deposit_changes_.begin();
    cur_sub_period.SetEndDate(date);
    cur_sub_period.CalculateInterest();
    cur_sub_period.SetStartDate(date);
    for (const auto &value : values) cur_sub_period.AddDeposit(value);
    deposit_changes_.erase(date);
  }
  if (cur_sub_period.GetEndDate() < period.GetEndDate()) {
    cur_sub_period.SetEndDate(period.GetEndDate());
  }
  cur_sub_period.CalculateInterest();
  cur_period_.SetDeposit(cur_sub_period.GetDeposit());
  cur_period_.SetInterest(cur_sub_period.GetInterest());
}

void FullDepositPeriod::CalculateTotalInterest() {
  for (auto &cur_period : periods_) {
    cur_period_.SetNewPeriod(cur_period);
    rate_period_.SetStartDate(cur_period.GetStartDate());
    CalculateInterestForPeriod();
    full_period_.AddInterest(cur_period_.GetInterest());
    cur_period_.SetInterest(0);
  }
  full_period_.AddDeposit(cur_period_.GetDeposit());
  HandleOtherDepositChanges();
}

void FullDepositPeriod::HandleOtherDepositChanges() {
  while (!deposit_changes_.empty()) {
    auto &[date, values] = *deposit_changes_.begin();
    for (const auto &value : values) full_period_.AddDeposit(value);
    deposit_changes_.erase(date);
  }
}

date_struct FullDepositPeriod::GetDateOfRateChange(const date_struct &date) {
  int year = GetYear(date);
  if (IsLeapYear(year)) {
    return std::make_tuple(1, 1, year + 1);
  }
  while (!(IsLeapYear(year))) {
    year++;
  }
  return std::make_tuple(1, 1, year);
}

long double FullDepositPeriod::GetDailyRate(const date_struct &date,
                                            long double rate_annual) {
  int year = GetYear(date);
  return rate_annual / GetDaysInYear(year);
}

period_struct FullDepositPeriod::GetPeriodDuration(Periodicity p) {
  return kPeriods[p];
}

long double FullDepositPeriod::GetTotalDeposit() {
  long double total = full_period_.GetDeposit();
  if (!capitalization_) total += full_period_.GetInterest();
  return total;
}
long double FullDepositPeriod::GetTotalInterest() {
  return full_period_.GetInterest();
}
long double FullDepositPeriod::GetTotalTaxAmount() {
  return GetTotalInterest() * tax_rate_;
}

Period FullDepositPeriod::GetRatePeriod(const date_struct &start_date) {
  return {start_date, GetDateOfRateChange(start_date)};
}

std::pair<date_struct, date_struct> FullDepositPeriod::GetTerm() {
  return {full_period_.GetStartDate(), full_period_.GetEndDate()};
}
// HELPERS

void PrintDate(const date_struct &date) {
  const auto &[day, month, year] = date;
  std::cout << "\nDate: " << day << "-" << month << "-" << year << std::endl;
}

std::string DateToString(const date_struct &date) {
  const auto &[day, month, year] = date;
  std::ostringstream oss;
  oss << day << "." << month << "." << year;
  return oss.str();
}

void PrintPeriod(period_struct &period) {
  const auto &[years, months, days] = period;
  std::cout << "Period: " << years << "y-" << months << "m-" << days << "d"
            << std::endl;
}

date_struct GetNextDate(date_struct date, period_struct period) {
  const auto &[day, month, year] = date;
  const auto &[years, months, days] = period;

  unsigned int year_to_months = year * 12 + years * 12 + months + month;
  unsigned int new_year = year_to_months / 12;
  unsigned int new_month = year_to_months % 12;
  unsigned int new_day = day + days;

  while (new_month < 1) {
    new_month += 12;
    new_year--;
  }
  while (new_day > GetDaysInMonth(new_month - 1, new_year)) {
    new_day -= GetDaysInMonth(new_month - 1, new_year);
    new_month++;
    if (new_month > 12) {
      new_month = 1;
      new_year++;
    }
  }
  return std::make_tuple(new_day, new_month, new_year);
}

int GetYear(const date_struct &date) { return std::get<2>(date); }

bool operator<(const date_struct &date1, const date_struct &date2) {
  // Compare years
  if (std::get<2>(date1) != std::get<2>(date2)) {
    return std::get<2>(date1) < std::get<2>(date2);
  }
  // Compare months
  if (std::get<1>(date1) != std::get<1>(date2)) {
    return std::get<1>(date1) < std::get<1>(date2);
  }
  // Compare days
  return std::get<0>(date1) < std::get<0>(date2);
}

bool operator>=(const period_struct &period1, const period_struct &period2) {
  const auto &[years1, months1, days1] = period1;
  const auto &[years2, months2, days2] = period2;
  auto p1 = date::years(years1) + date::months(months1) + date::days(days1);
  auto p2 = date::years(years2) + date::months(months2) + date::days(days2);
  return p1 >= p2;
}

}  // namespace deposit
}  // namespace s21
