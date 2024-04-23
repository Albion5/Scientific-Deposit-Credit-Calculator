#include "test_utils.h"
namespace test_utils {
std::string GetPathString(const std::string &data_file_name) {
  return data_path + "/" + data_file_name;
}
double GetToleranceScale(const double &exp_result, const double &fact_result,
                         double eps) {
  return eps * fmin(fabs(exp_result), fabs(fact_result));
}

}  // namespace test_utils