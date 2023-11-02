#include "CurrentDataView.h"

#include <cfloat>

#include "ui_CurrentDataView.h"

CurrentDataView::CurrentDataView(QWidget* parent)
    : QWidget(parent), ui(new Ui::CurrentDataView) {
  ui->setupUi(this);
}

CurrentDataView::~CurrentDataView() {
  delete ui;
}

void CurrentDataView::Setup(const Phys::Rocket& rocket) {
  const float maxHeight = rocket.GetHeight();
  ui->heightBar->setRange(0, maxHeight);

  const float maxMassFuel = rocket.GetFuellMass();
  ui->fuelMassBar->setRange(0, maxMassFuel);

  const float maxFuelEmission = rocket.GetMaxFuelEmission();
  ui->fuelEmissionBar->setRange(0, maxFuelEmission);

  Update(0.0f, rocket);
}

static void SetBarValue(QProgressBar& bar, float val) {
  bar.setRange(std::min(bar.minimum(), (int)val),
               std::max(bar.maximum(), (int)val));
  bar.setValue(val);
}

void CurrentDataView::Update(float time, const Phys::Rocket& rocket) {
  const float height = rocket.GetHeight();
  ui->heightVal->setText(QString("%1").arg(height));
  SetBarValue(*ui->heightBar, height);

  const float fuelMass = rocket.GetFuellMass();
  ui->fuelMassVal->setText(QString("%1").arg(fuelMass));
  SetBarValue(*ui->fuelMassBar, fuelMass);

  const float emission = rocket.GetFuelEmission();
  ui->fuelEmissionVal->setText(QString("%1").arg(emission));
  SetBarValue(*ui->fuelEmissionBar, emission);

  const float velocity = rocket.GetVelocity();
  ui->velocityVal->setText(QString("%1").arg(velocity));

  const float accel = rocket.GetAcceleration();
  ui->accelVal->setText(QString("%1").arg(accel));

  ui->timeVal->setText(QString("%1").arg(time));
}
