#include "graphgenerator.h"

namespace s21 {
GraphGenerator::GraphGenerator(QObject *parent) : QObject(parent) {}

void GraphGenerator::GenNewGraph() {
  formula_num_ = 0;
  formulas_.clear();
  switch (graph_num_) {
    case HEART_FULL:
      formulas_.append(heart_formula);
      break;
    case HEART_EMPTY:
      formulas_.append(heart_formulas);
      break;
    case BATMAN:
      formulas_.append(batman_formulas);
      break;
  }
  graph_chosen_ = true;
  graph_num_++;
  if (graph_num_ > 2) graph_num_ = 0;
  GetNewFormula();
}

void GraphGenerator::GetNewFormula() {
  if (!graph_chosen_) return;
  emit NewFormula(formulas_[formula_num_]);
  formula_num_++;
  if (formula_num_ >= formulas_.size()) {
    formula_num_ = 0;
    emit GraphComplete();
    graph_chosen_ = false;
  }
}
}  // namespace s21
