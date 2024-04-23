#include "bar.h"

namespace s21 {

Bar::Bar(QCustomPlot *widget, const QString &name1, const QString &name2,
         QObject *parent)
    : QObject(parent), widget_(widget) {
  x_axis_ = widget_->xAxis;
  y_axis_ = widget_->yAxis;
  interest_ = new QCPBars(x_axis_, y_axis_);
  loan_ = new QCPBars(x_axis_, y_axis_);
  loan_label_ = new QCPItemText(widget_);
  interest_label_ = new QCPItemText(widget_);
  InitBar(name1, name2);
}

void Bar::InitBar(const QString &name1, const QString &name2) {
  widget_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  widget_->setBackground(QBrush(QColor(255, 252, 229)));
  widget_->axisRect()->insetLayout()->setInsetAlignment(
      0, Qt::AlignBottom | Qt::AlignLeft);

  ConfigXAxis();
  ConfigYAxis();
  ConfigLegend(widget_->legend);
  ConfigBar(loan_, name1, QColor(111, 9, 176), 170);
  ConfigBar(interest_, name2, QColor(250, 170, 20), 150);
  interest_->moveAbove(loan_);
  ConfigLabel(loan_label_);
  ConfigLabel(interest_label_);
}

void Bar::ConfigXAxis() {
  x_axis_->grid()->setVisible(true);
  x_axis_->grid()->setSubGridVisible(true);
  x_axis_->setTicks(false);
  x_axis_->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
  x_axis_->setRange(-1, 1);
}

void Bar::ConfigYAxis() {
  y_axis_->grid()->setVisible(true);
  y_axis_->grid()->setSubGridVisible(true);
  y_axis_->setPadding(5);
  y_axis_->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
  y_axis_->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
  y_axis_->setSubTicks(false);
  y_axis_->setTickLabels(false);
  y_axis_->setTickLength(0, 0);
}

void Bar::ConfigLegend(QCPLegend *legend) {
  legend->setBrush(QColor(255, 255, 255, 100));
  legend->setBorderPen(Qt::NoPen);
  legend->setFont(QFont(widget_->font().family(), 14));
  legend->setVisible(true);
}

void Bar::ConfigBar(QCPBars *bar, const QString &name, QColor color,
                    int lighter) {
  bar->setAntialiased(false);
  bar->setStackingGap(1);
  bar->setName(name);
  bar->setBrush(color);
  bar->setPen(QPen(color.lighter(lighter)));
}

void Bar::ConfigLabel(QCPItemText *label) {
  label->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);
  label->position->setType(QCPItemPosition::ptPlotCoords);
  label->setFont(QFont(widget_->font().family(), 16));
  label->setVisible(false);
}

void Bar::Clear() {
  loan_label_->setVisible(false);
  interest_label_->setVisible(false);
  y_axis_->setTickLabels(false);
  interest_->data()->clear();
  loan_->data()->clear();
  widget_->replot();
}

void Bar::PlotBar(double loan, double interest) {
  loan_->addData(0, loan);
  interest_->addData(0, interest);
  loan_label_->setVisible(true);
  interest_label_->setVisible(true);

  loan_label_->position->setCoords(interest_->width() / 1.8, loan / 1.8);
  loan_label_->setText(QString::number(loan, 'f', 2) + "$");
  interest_label_->position->setCoords(interest_->width() / 1.8,
                                       interest / 1.8 + loan);
  interest_label_->setText(QString::number(interest, 'f', 2) + "$");
  x_axis_->setRange(-1, 1);
  y_axis_->rescale();
  widget_->replot();
}

}  // namespace s21
