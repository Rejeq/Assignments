#include "SetupView.h"

#include <cfloat>

#include <QDoubleSpinBox>

#include "ui_SetupView.h"

SetupView::SetupView(QWidget* parent) : QWidget(parent), ui(new Ui::SetupView) {
  ui->setupUi(this);

  InitSpinBox(*ui->fuelMassVal, 3, 0.001, DBL_MAX);
  InitSpinBox(*ui->shellMassVal, 3, 0.001, DBL_MAX);
  InitSpinBox(*ui->shellMassVal, 3, 0.001, DBL_MAX);
  InitSpinBox(*ui->fuelSpeedVal, 3, 0.001, DBL_MAX);
  InitSpinBox(*ui->heightVal, 3, 0.001, DBL_MAX);
  InitSpinBox(*ui->landingSpeedVal, 3, 0.001, DBL_MAX);
  InitSpinBox(*ui->timeStepVal, 3, 0.001, DBL_MAX);
  InitSpinBox(*ui->gravityVal, 3, 0.001, DBL_MAX);
  InitSpinBox(*ui->maxEmissionVal, 3, 0.001, DBL_MAX);
}

SetupView::~SetupView() {
  delete ui;
}

void SetupView::InitSpinBox(QDoubleSpinBox& spinBox, int count, double min,
                            double max) {
  auto Changed = [&](double) { emit ValueChanged(); };

  spinBox.setDecimals(count);
  spinBox.setMinimum(min);
  spinBox.setMaximum(max);

  connect(&spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          Changed);
}

void SetupView::SetRocket(const Phys::Rocket& rocket) {
  ui->heightVal->setValue(rocket.GetHeight());
  ui->shellMassVal->setValue(rocket.GetShellMass());
  ui->fuelMassVal->setValue(rocket.GetFuellMass());
  ui->maxEmissionVal->setValue(rocket.GetMaxFuelEmission());
  ui->fuelSpeedVal->setValue(rocket.GetFuelSpeed());
  ui->landingSpeedVal->setValue(rocket.GetLandingSpeed());
}

void SetupView::SetGravity(double gravity) {
  ui->gravityVal->setValue(gravity);
}

void SetupView::SetTimeStep(double ts) {
  ui->timeStepVal->setValue(ts);
}

Phys::Rocket SetupView::GetRocket() const {
  Phys::Rocket rocket;

  rocket.SetHeight(ui->heightVal->value());
  rocket.SetShellMass(ui->shellMassVal->value());
  rocket.SetFuellMass(ui->fuelMassVal->value());
  rocket.SetMaxFuelEmission(ui->maxEmissionVal->value());
  rocket.SetFuelSpeed(ui->fuelSpeedVal->value());
  rocket.SetLandingSpeed(ui->landingSpeedVal->value());

  return rocket;
}

double SetupView::GetGravity() const {
  return ui->gravityVal->value();
}

double SetupView::GetTimeStep() const {
  return ui->timeStepVal->value();
}
