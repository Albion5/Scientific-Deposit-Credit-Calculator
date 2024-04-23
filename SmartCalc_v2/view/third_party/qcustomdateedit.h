#ifndef QCUSTOMDATEEDIT_H
#define QCUSTOMDATEEDIT_H

#include <QWidget>

#include "qdatetimeedit.h"
namespace s21 {
class QCustomDateEdit : public QDateEdit {
  Q_OBJECT
 public:
  using date_struct = std::tuple<unsigned int, unsigned int, int>;
  explicit QCustomDateEdit(QWidget *parent = nullptr);
  date_struct GetDateStruct() const;
  QString GetDateString() const;

 signals:
};
}  // namespace s21
#endif  // QCUSTOMDATEEDIT_H
