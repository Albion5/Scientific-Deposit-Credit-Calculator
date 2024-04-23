#ifndef MODEL_H
#define MODEL_H
#include <string>

#include "credit_calc/credit.h"
#include "deposit_calc/deposit.h"
#include "smart_calc/calculator.h"
namespace s21 {
class Model {
 public:
  Model() = default;
  ~Model() = default;
  template <typename... Args>
  void Calculate(Args &&...args) {
    return smart_.Calculate(std::forward<Args>(args)...);
  }
  template <typename... Args>
  void CalculateGraphPoints(Args &&...args) {
    return smart_.CalculateGraphPoints(std::forward<Args>(args)...);
  }
  template <typename... Args>
  void GetCreditBarData(Args &&...args) {
    return credit_.GetBarData(std::forward<Args>(args)...);
  }
  template <typename... Args>
  void GetDepositBarData(Args &&...args) {
    return deposit_.GetBarData(std::forward<Args>(args)...);
  }
  template <typename... Args>
  void CalculateCredit(Args &&...args) {
    return credit_.Calculate(std::forward<Args>(args)...);
  }
  template <typename... Args>
  void CalculateDeposit(Args &&...args) {
    return deposit_.Calculate(std::forward<Args>(args)...);
  }
  std::string GetCreditResult() { return credit_.GetResultString(); }
  std::string GetDepositResult() { return deposit_.GetResultString(); }
  std::string GetScientificResult() {
    return std::to_string(smart_.GetAnswer());
  }

 private:
  SciCalculator smart_;
  CreditCalculator credit_;
  DepositCalculator deposit_;
};
}  // namespace s21
#endif
