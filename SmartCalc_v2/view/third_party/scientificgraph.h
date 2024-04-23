#ifndef SCIENTIFICGRAPH_H
#define SCIENTIFICGRAPH_H

#include <QObject>

#include "../../qcustomplot.h"
#include "graphgenerator.h"

namespace s21 {

class ScientificGraph : public QObject {
  Q_OBJECT
 public:
  explicit ScientificGraph(QCustomPlot *widget, QObject *parent = nullptr);

 public slots:
  void PlotNewPart(std::vector<std::pair<double, double>> &points);
  void Plot();
  void PlotNewPoints();
  void AddNewGraph();
  void ResetGraphMode();
  void ResetTimer();

  void UpdateDomain(double x_min, double x_max, double y_min, double y_max);
  std::pair<double, double> GetScale();
  std::pair<double, double> GetXRange();
  void ClearGraph();
  void Rescale(double x_min, double x_max, double y_min, double y_max);
  void UpdateRange();
  void SetMultigraphMode(int mode);
  void SetPenColor(QColor color);
  void PlotRandomGraph();
  void PlotCustomGraph(std::vector<std::pair<double, double>> &points);
 signals:
  void UpdateXAxis(double x_min, double x_max);
  void UpdateYAxis(double y_min, double y_max);
  void NewFormula(QString formula);

 private:
  enum { DEFAULT, CUSTOM, RANDOM };
  QCustomPlot *widget_ = nullptr;
  GraphGenerator graph_generator_;
  QTimer timer_;
  QColor pen_color_;
  bool multigraph_;
  int num_graphs_;
  int data_index_;
  double x_begin_, x_end_;
  int graph_mode_ = DEFAULT;
  std::vector<std::pair<double, double>> points_;
};
}  // namespace s21
#endif  // SCIENTIFICGRAPH_H
