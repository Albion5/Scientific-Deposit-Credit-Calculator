
#include "view.h"
QT_BEGIN_NAMESPACE
namespace s21 {

View::View(Controller *c, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::View), controller(c) {
  ui->setupUi(this);
  InitView();
}

View::~View() {
  delete deposit_bar_;
  delete credit_bar_;
  delete scientific_graph_;
  delete ui;
}

void View::InitView() {
  // <all>

  // DoubleSpinBoxes
  QList<QDoubleSpinBox *> allDSpinBoxes =
      View::findChildren<QDoubleSpinBox *>();
  for (int i = 0; i < allDSpinBoxes.length(); i++) {
    allDSpinBoxes[i]->setButtonSymbols(QAbstractSpinBox::NoButtons);
  }

  // SpinBoxes
  QList<QSpinBox *> allSpinBoxes = View::findChildren<QSpinBox *>();
  for (auto &spinbox : allSpinBoxes) {
    spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
  }

  // <SmartCalc>

  scientific_graph_ = new ScientificGraph(ui->qGraph, this);

  // InputBoxes
  QDoubleValidator validator(std::numeric_limits<double>::lowest(),
                             std::numeric_limits<double>::max(), 7);
  ui->le_InputBox_X->setValidator(&validator);
  QObject::connect(ui->le_InputBox, SIGNAL(returnPressed()), this,
                   SLOT(Calculate()));
  QObject::connect(ui->le_InputBox_X, SIGNAL(returnPressed()), this,
                   SLOT(Calculate()));

  // PushButtons
  QList<QPushButton *> smartPButtons =
      ui->SmartCalc->findChildren<QPushButton *>();
  QList<QPushButton *> specialPButtons = {
      ui->btn_Back,       ui->btn_Equal,      ui->btn_Graph,
      ui->btn_Rescale,    ui->btn_clearGraph, ui->btn_clearGraph,
      ui->btn_randomGraph};

  for (QPushButton *button : smartPButtons) {
    if (specialPButtons.contains(button)) {
      continue;
    }
    if (button->objectName().contains("Pen")) {
      QObject::connect(button, SIGNAL(released()), this,
                       SLOT(SelectPenColor()));
    } else {
      QObject::connect(button, SIGNAL(released()), this,
                       SLOT(AddDataToInputBox()));
      QObject::connect(button, SIGNAL(released()), ui->le_InputBox,
                       SLOT(setFocus()));
    }
  }

  emit ui->btn_redPen->released();

  QObject::connect(ui->btn_Back, &QPushButton::released, ui->le_InputBox,
                   &QLineEdit::backspace);
  QObject::connect(ui->btn_Equal, SIGNAL(released()), this, SLOT(Calculate()));
  QObject::connect(ui->btn_Clear, SIGNAL(released()), this, SLOT(ClearInput()));
  // Graph
  QObject::connect(ui->btn_Graph, SIGNAL(released()), this,
                   SLOT(PlotNewGraph()));
  QObject::connect(ui->btn_randomGraph, SIGNAL(released()), this,
                   SLOT(PlotNewGraph()));

  QObject::connect(ui->btn_Rescale, SIGNAL(released()), this, SLOT(Rescale()));
  QObject::connect(ui->cbx_multGraphs, SIGNAL(stateChanged(int)),
                   scientific_graph_, SLOT(SetMultigraphMode(int)));

  QObject::connect(ui->btn_clearGraph, SIGNAL(released()), scientific_graph_,
                   SLOT(ClearGraph()));
  QObject::connect(scientific_graph_, SIGNAL(NewFormula(QString)), this,
                   SLOT(PlotNewPart(QString)));

  QObject::connect(scientific_graph_, SIGNAL(UpdateXAxis(double, double)), this,
                   SLOT(UpdateXAxis(double, double)));
  QObject::connect(scientific_graph_, SIGNAL(UpdateYAxis(double, double)), this,
                   SLOT(UpdateYAxis(double, double)));
  ui->qGraph->xAxis->setRange(-10, 10);
  ui->qGraph->yAxis->setRange(-10, 10);
  SetScale();

  // <CreditCalc>
  credit_bar_ = new Bar(ui->qChartCredit, "Loan", "Interest", this);
  QObject::connect(ui->btn_CalculateCredit, SIGNAL(released()), this,
                   SLOT(CalculateCredit()));
  QObject::connect(ui->btn_ClearCredit, SIGNAL(released()), this,
                   SLOT(ClearCredit()));

  // <DepositCalc>
  deposit_bar_ = new Bar(ui->qChartDeposit, "Deposit", "Interest", this);
  QObject::connect(ui->btn_CalculateDeposit, SIGNAL(released()), this,
                   SLOT(CalculateDeposit()));
  QObject::connect(ui->btn_ClearDeposit, SIGNAL(released()), this,
                   SLOT(ClearDeposit()));
  QObject::connect(ui->btn_Today, SIGNAL(released()), this, SLOT(ResetDate()));
  QObject::connect(ui->de_StartDate, SIGNAL(editingFinished()), this,
                   SLOT(UpdateDate()));
  QObject::connect(ui->btn_AddR, SIGNAL(released()), this,
                   SLOT(AddReplenishment()));
  QObject::connect(ui->btn_AddW, SIGNAL(released()), this,
                   SLOT(AddWithdrawal()));
  QObject::connect(ui->cbx_ShowR, SIGNAL(activated(int)), this,
                   SLOT(DropSelection()));
  QObject::connect(ui->cbx_ShowW, SIGNAL(activated(int)), this,
                   SLOT(DropSelection()));
  ResetDate();
  ui->cbx_ShowR->view()->setSelectionMode(QAbstractItemView::NoSelection);
  ui->cbx_ShowW->view()->setSelectionMode(QAbstractItemView::NoSelection);
}

// Scientific
void View::SelectPenColor() {
  QPushButton *btn = (QPushButton *)sender();
  QColor pen_color;
  if (btn == ui->btn_yellowPen) {
    btn->setStyleSheet(
        "background-color: rgb(255, 255, 151);"
        "border: 5px solid rgb(36, 71, 71);");
    pen_color = QColor(255, 255, 151);
  } else {
    ui->btn_yellowPen->setStyleSheet(
        "background-color: rgb(255, 255, 151);"
        "border: 5px solid rgb(255, 255, 151);");
  }

  if (btn == ui->btn_greenPen) {
    btn->setStyleSheet(
        "background-color: rgb(36, 71, 71);"
        "border: 5px solid rgb(255, 241, 222);");
    pen_color = QColor(36, 71, 71);
  } else {
    ui->btn_greenPen->setStyleSheet(
        "background-color: rgb(36, 71, 71);"
        "border: 5px solid rgb(36, 71, 71);");
  }

  if (btn == ui->btn_bluePen) {
    btn->setStyleSheet(
        "background-color: rgb(114, 140, 255);"
        "border: 5px solid rgb(255, 241, 222);");
    pen_color = QColor(114, 140, 255);
  } else {
    ui->btn_bluePen->setStyleSheet(
        "background-color: rgb(114, 140, 255);"
        "border: 0px;");
  }

  if (btn == ui->btn_redPen) {
    btn->setStyleSheet(
        "background-color: rgb(255, 93, 117);"
        "border: 5px solid rgb(255, 241, 222);");
    pen_color = QColor(255, 93, 117);
  } else {
    ui->btn_redPen->setStyleSheet(
        "background-color: rgb(255, 93, 117);"
        "border: 5px solid rgb(255, 93, 117);");
  }

  if (btn == ui->btn_blackPen) {
    btn->setStyleSheet(
        "background-color: rgb(29, 15, 30);"
        "border: 5px solid rgb(255, 241, 222);");
    pen_color = QColor(29, 15, 30);
  } else {
    ui->btn_blackPen->setStyleSheet(
        "background-color: rgb(29, 15, 30);"
        "border: 5px solid rgb(29, 15, 30);");
  }
  scientific_graph_->SetPenColor(pen_color);
}

void View::AddDataToInputBox() {
  QPushButton *btn = (QPushButton *)sender();
  if (ui->le_InputBox->text() == "Error") {
    ui->le_InputBox->clear();
  }
  QString new_text = ui->le_InputBox->text() + btn->text();
  ui->le_InputBox->setText(new_text);
}

void View::ClearInput() {
  ui->le_InputBox->clear();
  ui->lbl_Result->setText("");
}

void View::PlotNewPart(QString formula) {
  ui->le_InputBox->setText(formula);
  QString infix_exp = ui->le_InputBox->text();
  GetGraphData(infix_exp, "");
  scientific_graph_->PlotNewPart(points_);
}

void View::Rescale() {
  scientific_graph_->Rescale(ui->dspbx_xMin->value(), ui->dspbx_xMax->value(),
                             ui->dspbx_yMin->value(), ui->dspbx_yMax->value());
}

void View::UpdateXAxis(double x_min, double x_max) {
  ui->dspbx_xMin->setValue(x_min);
  ui->dspbx_xMax->setValue(x_max);
  SetScale();
}

void View::UpdateYAxis(double y_min, double y_max) {
  ui->dspbx_yMin->setValue(y_min);
  ui->dspbx_yMax->setValue(y_max);
  SetScale();
}

void View::SetScale() {
  const auto &[scale_x, scale_y] = scientific_graph_->GetScale();
  ui->lbl_ScaleX->setText(QString::number(scale_x));
  ui->lbl_ScaleY->setText(QString::number(scale_y));
}

void View::GetGraphData(QString infix_expr, QString x_value_str) {
  const auto &[x_begin, x_end] = scientific_graph_->GetXRange();
  QString result = QString::fromStdString(controller->CalculateGraphPoints(
      infix_expr.toStdString(), x_value_str.toStdString(), x_begin, x_end,
      points_));
  ui->lbl_Result->setText(result);
}

void View::PlotNewGraph() {
  QPushButton *btn = (QPushButton *)sender();
  scientific_graph_->UpdateDomain(
      ui->dspbx_xMin->value(), ui->dspbx_xMax->value(), ui->dspbx_yMin->value(),
      ui->dspbx_yMax->value());
  if (btn == ui->btn_randomGraph) {
    scientific_graph_->PlotRandomGraph();
  } else {
    GetGraphData(ui->le_InputBox->text(), ui->le_InputBox_X->text());
    scientific_graph_->PlotCustomGraph(points_);
  }
}

void View::ClearGraph() { scientific_graph_->ClearGraph(); }

void View::Calculate() {
  std::string infix_exp = ui->le_InputBox->text().toStdString();
  std::string x_value_str = ui->le_InputBox_X->text().toStdString();

  QString result =
      QString::fromStdString(controller->Calculate(infix_exp, x_value_str));
  ui->lbl_Result->setText(result);
}

// Credit
void View::PlotCreditBar() {
  credit_bar_->Clear();
  std::pair<double, double> points_credit;
  controller->CalculateCreditBarData(points_credit);
  auto &[loan, interest] = points_credit;
  if (loan == 0) return;
  credit_bar_->PlotBar(loan, interest);
}

void View::ResetCreditInput() {
  ui->dspbx_Loan->setValue(0);
  ui->spbx_Years->setValue(0);
  ui->spbx_Months->setValue(0);
  ui->dspbx_Interest->setValue(0);
}
void View::ClearCredit() {
  credit_bar_->Clear();
  ui->te_OutputDetailed->clear();
  ResetCreditInput();
}

void View::CalculateCredit() {
  double loan = ui->dspbx_Loan->value();
  int n_years = ui->spbx_Years->value();
  int n_months = ui->spbx_Months->value();
  int interest_rate = ui->dspbx_Interest->value();
  int type = ui->cbx_Type->currentIndex();
  QString result = QString::fromStdString(controller->CalculateCredit(
      loan, interest_rate, n_years, n_months, type));
  ui->te_OutputDetailed->setText(result);
  PlotCreditBar();
}

// Deposit
void View::PlotDepositBar() {
  deposit_bar_->Clear();
  std::pair<double, double> points_deposit;
  controller->CalculateDepositBarData(points_deposit);
  auto &[deposit, interest] = points_deposit;
  if (deposit == 0) return;
  deposit_bar_->PlotBar(deposit, interest);
}

void View::ResetDepositInput() {
  ui->dspbx_Deposit->setValue(0);
  ui->spbx_Years_2->setValue(0);
  ui->spbx_Months_2->setValue(0);
  ui->spbx_Days_2->setValue(0);
  ui->dspbx_Interest_2->setValue(0);
  ui->dspbx_Tax->setValue(0);
  ui->dspbx_Replenishment->setValue(0);
  ui->dspbx_Withdrawal->setValue(0);
  ui->cbx_ShowR->clear();
  ui->cbx_ShowW->clear();
  replenishments_.clear();
  withdrawals_.clear();
  ResetDate();
}
void View::ResetDate() {
  ui->de_StartDate->setDate(QDate::currentDate());
  ui->de_ReplenishmentDate->setDate(QDate::currentDate());
  ui->de_WithdrawalDate->setDate(QDate::currentDate());
}

void View::ClearDeposit() {
  deposit_bar_->Clear();
  ui->te_OutputDetailed_2->clear();
  ResetDepositInput();
}

void View::CalculateDeposit() {
  double deposit = ui->dspbx_Deposit->value();
  int n_years = ui->spbx_Years_2->value();
  int n_months = ui->spbx_Months_2->value();
  int n_days = ui->spbx_Days_2->value();
  std::tuple<unsigned int, unsigned int, unsigned int> period = {
      n_years, n_months, n_days};
  double interest_rate = ui->dspbx_Interest_2->value();
  double tax_rate = ui->dspbx_Tax->value();
  int periodicity = ui->cbx_Periodicity->currentIndex();
  bool capitalization = ui->cbx_Capitalization->isChecked();
  QString result = QString::fromStdString(controller->CalculateDeposit(
      ui->de_StartDate->GetDateStruct(), deposit, interest_rate, periodicity,
      period, replenishments_, withdrawals_, capitalization, tax_rate));
  ui->te_OutputDetailed_2->setText(result);
  PlotDepositBar();
}

void View::AddReplenishment() {
  std::tuple<double, double, double> replenishment_date =
      ui->de_ReplenishmentDate->GetDateStruct();
  double value = ui->dspbx_Replenishment->value();
  replenishments_.emplace_back(replenishment_date, value);
  QString date_string = ui->de_ReplenishmentDate->GetDateString();
  QString value_string = QString::number(value);
  ui->cbx_ShowR->addItem(date_string + ":  +" + value_string +
                         ui->dspbx_Replenishment->suffix());
  ui->cbx_ShowR->setCurrentIndex(-1);
}

void View::AddWithdrawal() {
  std::tuple<double, double, double> withdrawal_date =
      ui->de_WithdrawalDate->GetDateStruct();
  double value = ui->dspbx_Withdrawal->value();
  withdrawals_.emplace_back(withdrawal_date, value);
  QString date_string = ui->de_WithdrawalDate->GetDateString();
  QString value_string = QString::number(value);
  ui->cbx_ShowW->addItem(date_string + ":  -" + value_string +
                         ui->dspbx_Withdrawal->suffix());
  ui->cbx_ShowW->setCurrentIndex(-1);
}

void View::DropSelection() {
  QComboBox *cbx = (QComboBox *)sender();
  cbx->setCurrentIndex(-1);
}

void View::UpdateDate() {
  ui->de_ReplenishmentDate->setDate(ui->de_StartDate->date());
  ui->de_WithdrawalDate->setDate(ui->de_StartDate->date());
}
}  // namespace s21
QT_END_NAMESPACE
