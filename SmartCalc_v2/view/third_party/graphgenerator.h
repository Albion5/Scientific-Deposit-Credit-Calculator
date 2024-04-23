#ifndef GRAPHGENERATOR_H
#define GRAPHGENERATOR_H

#include <QDebug>
#include <QObject>
#include <QTimer>
namespace s21 {

class GraphGenerator : public QObject {
  Q_OBJECT
 public:
  explicit GraphGenerator(QObject *parent = nullptr);

 public slots:
  void GenNewGraph();
  void GetNewFormula();
 signals:
  void NewGraphChosen();
  void NewFormula(QString formula);
  void GraphComplete();

 private:
  enum { HEART_FULL, HEART_EMPTY, BATMAN };
  const QString heart_formula =
      "sqrt(cos(x))*cos(200*x)+sqrt(x^2)-3.1415/4*(4-x^2)^0.01";
  const QVector<QString> heart_formulas = {
      "-x^(2/3)+sqrt(1-x^2)", "x^(2/3)+sqrt(1-x^2)", "-x^(2/3)-sqrt(1-x^2)",
      "+x^(2/3)-sqrt(1-x^2)"};
  const QVector<QString> batman_formulas = {
      "2*sqrt(-abs(abs(X)-1)*abs(3-abs(X))/"
      "((abs(X)-1)*(3-abs(X))))*(1+abs(abs(X)-3)/(abs(X)-3))*sqrt(1-(X/7)*(X/"
      "7))+(5+0.97*(abs(X-0.5)+abs(X+0.5))-3*(abs(X-0.75)+abs(X+0.75)))*(1+abs("
      "1-abs(X))/(1-abs(X)))",
      "-3*sqrt(1-(X/7)*(X/7))*sqrt(abs(abs(X)-4)/(abs(X)-4))",
      "abs(X/2)-0.0913722*(X*X)-3+sqrt(1-(abs(abs(X)-2)-1)*(abs(abs(X)-2)-1))",
      "(2.71052+(1.5-0.5*abs(X))-1.35526*sqrt(4-(abs(X)-1)*(abs(X)-1)))*sqrt("
      "abs(abs(X)-1)/(abs(X)-1))+0.9"};
  int graph_num_ = 0;
  int formula_num_ = 0;
  bool graph_chosen_ = false;
  QVector<QString> formulas_;
};
}  // namespace s21

#endif  // GRAPHGENERATOR_H
