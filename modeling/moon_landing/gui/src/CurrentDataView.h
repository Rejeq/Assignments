#pragma once

#include <QWidget>

#include <Physics/Rocket.h>

namespace Ui {
class CurrentDataView;
}

class CurrentDataView : public QWidget {
  Q_OBJECT

 public:
  CurrentDataView(QWidget* parent = nullptr);
  ~CurrentDataView();

  void Setup(const Phys::Rocket& rocket);
  void Update(float time, const Phys::Rocket& rocket);

 private:
  Ui::CurrentDataView* ui;
};
