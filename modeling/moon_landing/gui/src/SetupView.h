#pragma once

#include <QWidget>

#include <Physics/Rocket.h>

namespace Ui {
class SetupView;
}

class QDoubleSpinBox;

class SetupView : public QWidget {
  Q_OBJECT

 public:
  SetupView(QWidget* parent = nullptr);
  ~SetupView();

  void SetRocket(const Phys::Rocket& rocket);
  void SetGravity(double gravity);
  void SetTimeStep(double ts);

  Phys::Rocket GetRocket() const;
  double GetGravity() const;
  double GetTimeStep() const;

 signals:
  void ValueChanged();

 private:
  void InitSpinBox(QDoubleSpinBox& spinBox, int count, double min, double max);

  Ui::SetupView* ui;
};
