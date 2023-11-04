#pragma once

#include <QKeyEvent>
#include <QObject>
#include <QPointer>

#include <Physics/Rocket.h>

class CurrentDataView;
class PlotView;
class SceneView;
class SetupView;
class WorldController;

// This is a presenter actually
class RocketController : public QObject {
  Q_OBJECT

 public:
  void SetCurrentDataView(CurrentDataView* currData);
  void SetPlotView(PlotView* plot);
  void SetSceneView(SceneView* scene);
  void SetSetupView(SetupView* setup);
  void SetWorldController(WorldController* controller);

  CurrentDataView* GetCurrentDataView();
  PlotView* GetPlotView();
  SceneView* GetSceneView();
  SetupView* GetSetupView();
  WorldController* GetWorldController();

  void ShiftFuellEmission(double delta);
  double GetMaxFuelEmission() const;

 signals:
  void Destroyed();
  void Landed();

 private:
  void OnWorldUpdate(Phys::World& world);
  void OnWorldReset(Phys::World& world);

  const Phys::Rocket& GetRocketInstance(const Phys::World& world) const;
  Phys::Rocket& GetRocketInstance(Phys::World& world);

  const Phys::TestRocket& GetTestRocketInstance(const Phys::World& world) const;
  Phys::TestRocket& GetTestRocketInstance(Phys::World& world);

  // TODO: Maybe do not use views here, use signal system instead
  // cons - if the world updated every milliseconds - at least there will be
  // 1000 emit calls per second
  QPointer<CurrentDataView> m_currData;
  QPointer<PlotView> m_plot;
  QPointer<SceneView> m_scene;
  QPointer<SetupView> m_setup;

  QPointer<WorldController> m_worldController;
  bool m_landed = false;
};
