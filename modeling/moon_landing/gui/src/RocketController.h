#pragma once

#include <QKeyEvent>
#include <QObject>

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

  CurrentDataView* GetCurrentDataView() { return m_currData; }
  PlotView* GetPlotView() { return m_plot; }
  SceneView* GetSceneView() { return m_scene; }
  SetupView* GetSetupView() { return m_setup; }
  WorldController* GetWorldController() { return m_worldController; }

  void ShiftFuellEmission(double delta);

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

  CurrentDataView* m_currData = nullptr;
  PlotView* m_plot = nullptr;
  SceneView* m_scene = nullptr;
  SetupView* m_setup = nullptr;

  WorldController* m_worldController = nullptr;
};
