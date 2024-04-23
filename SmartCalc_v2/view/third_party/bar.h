#ifndef BAR_H
#define BAR_H
#include <QObject>

#include "../../qcustomplot.h"
namespace s21 {

class Bar : public QObject {
  Q_OBJECT
 public:
  Bar(QCustomPlot *widget, const QString &name1, const QString &name2,
      QObject *parent = nullptr);

 public:
  void Clear();
  void PlotBar(double loan, double interest);

 private:
  void InitBar(const QString &name1, const QString &name2);
  void ConfigXAxis();
  void ConfigYAxis();
  void ConfigLegend(QCPLegend *legend);
  void ConfigBar(QCPBars *bar, const QString &name, QColor color, int lighter);
  void ConfigLabel(QCPItemText *label);

  QCustomPlot *widget_;
  QCPAxis *x_axis_;
  QCPAxis *y_axis_;
  QCPBars *interest_ = nullptr;
  QCPBars *loan_ = nullptr;
  QCPItemText *loan_label_ = nullptr;
  QCPItemText *interest_label_ = nullptr;
};

}  // namespace s21
#endif  // BAR_H
