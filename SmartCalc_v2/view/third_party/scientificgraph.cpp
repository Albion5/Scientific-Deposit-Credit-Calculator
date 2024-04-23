#include "scientificgraph.h"

namespace s21 {
ScientificGraph::ScientificGraph(QCustomPlot *widget, QObject *parent)
    : QObject(parent),
      widget_(widget),
      pen_color_(QColor(255, 93, 117)),
      multigraph_(0),
      num_graphs_(0),
      data_index_(0) {
  QObject::connect(&graph_generator_, SIGNAL(NewFormula(QString)), this,
                   SIGNAL(NewFormula(QString)));

  QObject::connect(&graph_generator_, SIGNAL(GraphComplete()), this,
                   SLOT(ResetGraphMode()));

  QObject::connect(&timer_, SIGNAL(timeout()), this, SLOT(PlotNewPoints()));

  QObject::connect(widget_->xAxis, SIGNAL(rangeChanged(QCPRange)), this,
                   SLOT(UpdateRange()));
  QObject::connect(widget_->yAxis, SIGNAL(rangeChanged(QCPRange)), this,
                   SLOT(UpdateRange()));

  widget_->setInteraction(QCP::iRangeZoom, true);
  widget_->setInteraction(QCP::iRangeDrag, true);
}

void ScientificGraph::UpdateRange() {
  QCPAxis *axis = (QCPAxis *)sender();
  if (axis == widget_->xAxis) {
    emit UpdateXAxis(axis->range().lower, axis->range().upper);
  } else {
    emit UpdateYAxis(axis->range().lower, axis->range().upper);
  }
}

std::pair<double, double> ScientificGraph::GetScale() {
  double rangeX = widget_->xAxis->range().size();
  double rangeY = widget_->yAxis->range().size();
  double scaleX = 1.0, scaleY = 1.0;
  if (rangeX < rangeY) {
    scaleX = rangeY / rangeX;
  } else if (rangeX > rangeY) {
    scaleY = rangeX / rangeY;
  }
  return {scaleX, scaleY};
}

void ScientificGraph::ClearGraph() {
  ResetTimer();
  widget_->clearPlottables();
  widget_->xAxis->setRange(-10, 10);
  widget_->yAxis->setRange(-10, 10);
  widget_->replot();
}

void ScientificGraph::SetMultigraphMode(int mode) { multigraph_ = mode; }

void ScientificGraph::UpdateDomain(double x_min, double x_max, double y_min,
                                   double y_max) {
  double xMin = fmin(x_min, x_max);
  double xMax = fmax(x_min, x_max);
  if (fabs(xMax - xMin) < 1) {
    xMin = -10;
    xMax = 10;
  }
  x_begin_ = xMin;
  x_end_ = xMax;
  widget_->xAxis->setRange(xMin, xMax);
  double yMin = fmin(y_min, y_max);
  double yMax = fmax(y_min, y_max);
  if (fabs(yMax - yMin) < 1) {
    yMin = -10;
    yMax = 10;
  }
  widget_->yAxis->setRange(yMin, yMax);
}

std::pair<double, double> ScientificGraph::GetXRange() {
  return {x_begin_, x_end_};
}

void ScientificGraph::Rescale(double x_min, double x_max, double y_min,
                              double y_max) {
  UpdateDomain(x_min, x_max, y_min, y_max);
  widget_->replot();
}

void ScientificGraph::PlotNewPoints() {
  for (int i = 0; i < 50 && data_index_ < points_.size(); i++) {
    const auto [x, y] = points_[data_index_++];
    widget_->graph(num_graphs_ - 1)->addData(x, y);
  }
  if (data_index_ >= points_.size()) {
    ResetTimer();
    (graph_mode_ == RANDOM) ? graph_generator_.GetNewFormula()
                            : ResetGraphMode();
  }
  widget_->replot();
}

void ScientificGraph::ResetGraphMode() { graph_mode_ = DEFAULT; }

void ScientificGraph::PlotNewPart(
    std::vector<std::pair<double, double>> &points) {
  points_ = points;
  Plot();
}

void ScientificGraph::SetPenColor(QColor color) { pen_color_ = color; }

void ScientificGraph::PlotRandomGraph() {
  ResetTimer();
  graph_mode_ = RANDOM;
  graph_generator_.GenNewGraph();
}

void ScientificGraph::PlotCustomGraph(
    std::vector<std::pair<double, double>> &points) {
  ResetTimer();
  graph_mode_ = CUSTOM;
  points_ = points;
  Plot();
}

void ScientificGraph::Plot() {
  if (points_.empty()) return;
  if (!multigraph_) {
    widget_->clearPlottables();
  }
  AddNewGraph();
  timer_.start(1);
}

void ScientificGraph::AddNewGraph() {
  QCPGraph *new_graph = widget_->addGraph();
  num_graphs_ = widget_->graphCount();
  new_graph->setPen(QPen(pen_color_));
  new_graph->setScatterStyle(QCPScatterStyle::ssCircle);
  new_graph->setLineStyle(QCPGraph::lsNone);
}

void ScientificGraph::ResetTimer() {
  timer_.stop();
  data_index_ = 0;
}
}  // namespace s21
