#ifndef VIEW_H
#define VIEW_H
#include <QColor>
#include <QComboBox>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QRandomGenerator>
#include <QVector>
#include <QWidget>

#include "controller/controller.h"
#include "third_party/bar.h"
#include "third_party/graphgenerator.h"
#include "ui_view.h"
#include "view/third_party/scientificgraph.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
namespace s21 {

class View : public QMainWindow {
  Q_OBJECT

 public:
  explicit View(Controller *c, QWidget *parent = nullptr);
  ~View();

 private slots:
  void Calculate();
  void PlotNewPart(QString formula);
  void GetGraphData(QString infix_expr, QString x_value_str);
  void SetScale();
  void SelectPenColor();
  void AddDataToInputBox();
  void PlotNewGraph();
  void ClearGraph();
  void ClearInput();
  void Rescale();
  void UpdateXAxis(double x_min, double x_max);
  void UpdateYAxis(double y_min, double y_max);

  void CalculateCredit();
  void ClearCredit();
  void ResetCreditInput();
  void PlotCreditBar();

  void CalculateDeposit();
  void ClearDeposit();
  void ResetDepositInput();
  void ResetDate();
  void PlotDepositBar();

  void AddReplenishment();
  void AddWithdrawal();
  void DropSelection();
  void UpdateDate();

 private:
  void InitView();

 private:
  Ui::View *ui;
  Controller *controller;
  GraphGenerator graph_generator_;
  Bar *credit_bar_ = nullptr;
  Bar *deposit_bar_ = nullptr;
  ScientificGraph *scientific_graph_ = nullptr;
  std::vector<std::pair<double, double>> points_;
  std::vector<std::pair<date_struct, double>> replenishments_;
  std::vector<std::pair<date_struct, double>> withdrawals_;
};
}  // namespace s21

QT_END_NAMESPACE

#endif
