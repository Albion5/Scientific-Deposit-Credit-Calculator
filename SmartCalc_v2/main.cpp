#include <QApplication>

#include "view/view.h"
int main(int argc, char *argv[]) {
  setenv("LC_NUMERIC", "C", 1);
  QApplication a(argc, argv);
  s21::Model model;
  s21::Controller controller(&model);
  s21::View view(&controller);

  view.show();
  return a.exec();
}
