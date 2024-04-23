#include "qcustomdateedit.h"
namespace s21 {
QCustomDateEdit::QCustomDateEdit(QWidget *parent) : QDateEdit(parent) {}

QCustomDateEdit::date_struct QCustomDateEdit::GetDateStruct() const {
  int year;
  int month;
  int day;
  date_struct d = std::make_tuple(year, month, day);
  date().getDate(&year, &month, &day);
  return {day, month, year};
}

QString QCustomDateEdit::GetDateString() const {
  return date().toString("dd.MM.yyyy");
}
}  // namespace s21
