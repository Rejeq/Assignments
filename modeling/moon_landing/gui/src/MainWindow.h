#pragma once

#include <QMainWindow>

#include <DockManager.h>

#include "RocketController.h"
#include "WorldController.h"

namespace Ui {
class MainWindow;
}

class SetupView;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 protected:
  void closeEvent(QCloseEvent* event) override;

 private:
  ads::CDockWidget* InitSceneView();
  ads::CDockWidget* InitToolBarView(SetupView* setup);
  ads::CDockWidget* InitSetupView();
  ads::CDockWidget* InitCurrentDataView();
  ads::CDockWidget* InitPlotView();
  ads::CDockWidget* InitLogView();

  WorldController m_worldController;
  RocketController m_rocketController;

  ads::CDockManager* m_dockManager = nullptr;
  Ui::MainWindow* ui = nullptr;
};
