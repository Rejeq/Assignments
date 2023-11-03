#include "MainWindow.h"

#include <spdlog/sinks/qt_sinks.h>

#include <Log.h>

#include <QAction>
#include <QMessageBox>
#include <QSpinBox>
#include <QTextBrowser>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>

#include <DockAreaWidget.h>
#include <DockSplitter.h>
#include <DockWidget.h>

#include "CurrentDataView.h"
#include "PlotView.h"
#include "SceneView.h"
#include "SetupView.h"

#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  using adsFlags = ads::CDockManager;
  ads::CDockManager::setConfigFlag(adsFlags::OpaqueSplitterResize, true);
  ads::CDockManager::setConfigFlag(adsFlags::XmlCompressionEnabled, false);
  ads::CDockManager::setConfigFlag(adsFlags::FocusHighlighting, true);

  m_dockManager = new ads::CDockManager(this);

  auto* sceneDock = InitSceneView();
  auto* sceneArea = m_dockManager->setCentralWidget(sceneDock);

  auto* setupDock = InitSetupView();
  auto* setupArea =
      m_dockManager->addDockWidget(ads::LeftDockWidgetArea, setupDock);

  auto* toolDock = InitToolBarView(m_rocketController.GetSetupView());
  auto* toolArea =
      m_dockManager->addDockWidget(ads::TopDockWidgetArea, toolDock, sceneArea);
  toolArea->setDockAreaFlag(ads::CDockAreaWidget::HideSingleWidgetTitleBar,
                            true);

  // Adjust minimum space for ToolBar widget
  auto* toolAreaSplitter =
      ads::internal::findParent<ads::CDockSplitter*>(toolArea);
  if (toolAreaSplitter != nullptr)
    toolAreaSplitter->setSizes({0, 0});

  auto* dataDock = InitCurrentDataView();
  m_dockManager->addDockWidget(ads::BottomDockWidgetArea, dataDock, setupArea);

  auto* plotDock = InitPlotView();
  m_dockManager->addDockWidget(ads::CenterDockWidgetArea, plotDock, sceneArea);

  auto* logDock = InitLogView();
  m_dockManager->addDockWidget(ads::CenterDockWidgetArea, logDock, setupArea);

  setupDock->toggleView(true);
  plotDock->toggleView(true);

  m_rocketController.SetWorldController(&m_worldController);

  connect(&m_rocketController, &RocketController::Destroyed, this, []() {
    QMessageBox gameOverMsg;
    gameOverMsg.setText("Ракета разбилась");
    gameOverMsg.exec();
  });

  connect(&m_rocketController, &RocketController::Landed, this, []() {
    QMessageBox gameOverMsg;
    gameOverMsg.setText("Ракета успешно приземлилась");
    gameOverMsg.exec();
  });

  m_worldController.Reset();
  m_worldController.SetSpeed(1);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event) {
  m_dockManager->deleteLater();
  QMainWindow::closeEvent(event);
}

ads::CDockWidget* MainWindow::InitSceneView() {
  SceneView* scene = new SceneView();
  scene->Zoom(-15.0f);
  scene->UpdateRocketPos({0.0f, 0.0f, -1.0f});
  installEventFilter(scene);
  m_rocketController.SetSceneView(scene);

  auto* sceneDock = new ads::CDockWidget("Сцена");
  sceneDock->setWidget(scene);
  sceneDock->setFeature(ads::CDockWidget::DeleteContentOnClose, true);

  return sceneDock;
}

ads::CDockWidget* MainWindow::InitToolBarView(SetupView* setup) {
  QToolBar* toolBar = new QToolBar();
  toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

  auto* emptyLeft = new QWidget();
  emptyLeft->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  toolBar->addWidget(emptyLeft);

  auto* pause = new QAction(QIcon(":Moon/Icons/Pause.svg"), "Остановить", this);
  auto* play = new QAction(QIcon(":Moon/Icons/Play.svg"), "Начать", this);

  auto* playBtn = new QToolButton();
  playBtn->setStyleSheet("QToolButton::menu-indicator { image: none; }");

  toolBar->addWidget(playBtn);
  connect(playBtn, &QToolButton::triggered, &m_worldController,
          &WorldController::ToggleAutoUpdate);
  connect(&m_worldController, &WorldController::AutoUpdateChanged,
          [=](bool enabled) {
            playBtn->setDefaultAction((enabled) ? pause : play);
          });

  auto* step = new QAction(QIcon(":Moon/Icons/PlayPause.svg"), "Шаг", this);
  auto* stepBtn = new QToolButton();
  stepBtn->setDefaultAction(step);
  toolBar->addWidget(stepBtn);
  connect(stepBtn, &QToolButton::triggered, &m_worldController,
          &WorldController::Step);
  connect(&m_worldController, &WorldController::AutoUpdateChanged,
          [=](bool enabled) { stepBtn->setDisabled(enabled); });

  auto* stopBtn = new QToolButton();
  auto* stop = new QAction(QIcon(":Moon/Icons/Stop.svg"), "Сбросить", this);
  stopBtn->setDefaultAction(stop);

  toolBar->addWidget(stopBtn);
  connect(stop, &QAction::triggered, this,
          [this, stopBtn]() { m_worldController.Reset(); });
  connect(&m_worldController, &WorldController::Reseted, this,
          [=](Phys::World&) { stopBtn->setStyleSheet(""); });
  connect(setup, &SetupView::ValueChanged, this, [=]() {
    stopBtn->setStyleSheet("QToolButton { border : 3px solid lightgreen; }");
  });

  auto* restart =
      toolBar->addAction(QIcon(":Moon/Icons/Restart.svg"), "Перезапустить");
  connect(restart, &QAction::triggered, &m_worldController,
          &WorldController::Restart);

  QFrame* line = new QFrame;
  line->setFrameShape(QFrame::VLine);
  line->setFrameShadow(QFrame::Sunken);
  toolBar->addWidget(line);

  auto* spinbox = new QSpinBox();
  spinbox->setToolTip("Скорость");
  spinbox->setMinimum(1);
  spinbox->setMaximum(9999);

  connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged),
          &m_worldController,
          [=](int val) { m_worldController.SetSpeed(val); });
  connect(&m_worldController, &WorldController::SpeedChanged, spinbox,
          &QSpinBox::setValue);

  toolBar->addWidget(spinbox);

  auto* emptyRight = new QWidget();
  emptyRight->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  toolBar->addWidget(emptyRight);

  auto* toolDock = new ads::CDockWidget("ToolBar");
  toolDock->setMinimumSizeHintMode(
      ads::CDockWidget::MinimumSizeHintFromDockWidget);
  toolDock->setWidget(toolBar);
  toolDock->setFeatures(ads::CDockWidget::DockWidgetDeleteOnClose |
                        ads::CDockWidget::NoTab);
  ui->menuView->addAction(toolDock->toggleViewAction());

  return toolDock;
}

ads::CDockWidget* MainWindow::InitSetupView() {
  const double gravity = 1.62;
  const double timestep = 0.016;
  Phys::Rocket rocket;
  rocket.SetHeight(10'000.0);
  rocket.SetShellMass(6'000.0);
  rocket.SetFuellMass(30'000.0);
  rocket.SetFuelSpeed(2'500.0);
  rocket.SetMaxFuelEmission(300.0);
  rocket.SetFuelEmission(0.0);
  rocket.SetLandingSpeed(5.0);

  auto* setup = new SetupView();
  setup->SetGravity(gravity);
  setup->SetTimeStep(timestep);
  setup->SetRocket(rocket);
  m_rocketController.SetSetupView(setup);

  auto* setupDock = new ads::CDockWidget("Настройки");
  setupDock->setWidget(setup);
  setupDock->setMinimumSizeHintMode(
      ads::CDockWidget::MinimumSizeHintFromDockWidget);
  ui->menuView->addAction(setupDock->toggleViewAction());

  return setupDock;
}

ads::CDockWidget* MainWindow::InitCurrentDataView() {
  auto* currData = new CurrentDataView();
  m_rocketController.SetCurrentDataView(currData);

  auto* dataDock = new ads::CDockWidget("Текущие данные");
  dataDock->setWidget(currData);
  dataDock->setMinimumSizeHintMode(
      ads::CDockWidget::MinimumSizeHintFromDockWidget);
  ui->menuView->addAction(dataDock->toggleViewAction());

  return dataDock;
}

ads::CDockWidget* MainWindow::InitPlotView() {
  PlotView* plot = new PlotView();
  m_rocketController.SetPlotView(plot);

  auto* plotDock = new ads::CDockWidget("Графики");
  plotDock->setWidget(plot);
  plotDock->setMinimumSizeHintMode(
      ads::CDockWidget::MinimumSizeHintFromDockWidget);
  ui->menuView->addAction(plotDock->toggleViewAction());

  return plotDock;
}

ads::CDockWidget* MainWindow::InitLogView() {
  auto* logEdit = new QTextBrowser();
  logEdit->setPlaceholderText("Журнал пуст");
  Log::Get().AddSink<spdlog::sinks::qt_color_sink_mt>(logEdit, 512);

  auto* logDock = new ads::CDockWidget("Журнал");
  logDock->setWidget(logEdit);
  ui->menuView->addAction(logDock->toggleViewAction());

  return logDock;
}
