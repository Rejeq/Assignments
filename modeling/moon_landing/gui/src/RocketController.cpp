#include "RocketController.h"

#include <Log.h>

#include "CurrentDataView.h"
#include "PlotView.h"
#include "SceneView.h"
#include "SetupView.h"
#include "WorldController.h"

void RocketController::SetCurrentDataView(CurrentDataView* currData) {
  m_currData = currData;
}

void RocketController::SetPlotView(PlotView* plot) {
  m_plot = plot;
  m_plot->SetGraphCount(2);
  m_plot->SetGraphInfo(0, "Формула Мещерского", QPen(Qt::blue));
  m_plot->SetGraphInfo(1, "Формула Циолковского", QPen(Qt::red));
}

void RocketController::SetSceneView(SceneView* scene) {
  if (m_scene != nullptr)
    this->disconnect(m_scene);

  m_scene = scene;
  connect(scene, &SceneView::EmissionChanged, this,
          &RocketController::ShiftFuellEmission);
}

void RocketController::SetSetupView(SetupView* setup) {
  m_setup = setup;
}

void RocketController::SetWorldController(WorldController* controller) {
  if (m_worldController != nullptr)
    this->disconnect(m_worldController);

  m_worldController = controller;

  connect(controller, &WorldController::Updated, this,
          &RocketController::OnWorldUpdate);
  connect(controller, &WorldController::Reseted, this,
          &RocketController::OnWorldReset);
}

void RocketController::ShiftFuellEmission(double delta) {
  LOG_INFO("RocketController: Fuel emission chagned by delta: {}", delta);

  Phys::World& world = m_worldController->ConsumeWorld();
  Phys::Rocket& rocket = GetRocketInstance(world);
  Phys::TestRocket& testRocket = GetTestRocketInstance(world);

  double emission = rocket.GetFuelEmission();
  rocket.SetFuelEmission(emission + delta);
  testRocket.SetFuelEmission(emission + delta);

  if (m_currData != nullptr)
    m_currData->Update(world.GetLifetime(), rocket);

  m_worldController->ReturnWorld();
}

void RocketController::OnWorldUpdate(Phys::World& world) {
  Phys::Rocket& rocket = GetRocketInstance(world);
  Phys::TestRocket& testRocket = GetTestRocketInstance(world);
  const double lifetime = world.GetLifetime();
  float height = rocket.GetHeight();

  LOG_TRACE("RocketController: World lifetime: {}", lifetime);
  LOG_TRACE("RocketController: Rocket height: {}", height);
  LOG_TRACE("RocketController: TestRocket height: {}", testRocket.GetHeight());
  LOG_TRACE("RocketController: TestRocket velocity: {}",
            testRocket.GetVelocity());

  if (height <= 0.0f) {
    m_worldController->DisableAutoUpdate();

    if (std::abs(rocket.GetVelocity()) > rocket.GetLandingSpeed())
      emit Destroyed();
    else
      emit Landed();

    rocket.SetHeight(0.0);
    height = rocket.GetHeight();
  }

  if (m_scene != nullptr) {
    m_scene->UpdateRocketPos({0.0, height, 0.0});
    m_scene->update();
  }

  if (m_plot != nullptr) {
    m_plot->Append(0, lifetime, rocket);
    m_plot->Append(1, lifetime, testRocket);
  }

  if (m_currData != nullptr)
    m_currData->Update(lifetime, rocket);
}

void RocketController::OnWorldReset(Phys::World& world) {
  Phys::Rocket rocket = m_setup->GetRocket();
  double gravity = m_setup->GetGravity();
  double timestep = m_setup->GetTimeStep();

  if (m_plot != nullptr)
    m_plot->Clear();

  if (m_currData != nullptr)
    m_currData->Setup(rocket);

  world.SetRocket(rocket);
  world.SetTestRocket(rocket);
  world.SetGravity({0.0, gravity, 0.0});
  m_worldController->SetAutoUpdateIntervalSec(timestep);

  OnWorldUpdate(world);
}

Phys::Rocket& RocketController::GetRocketInstance(Phys::World& world) {
  return world.GetRocket();
}

const Phys::Rocket& RocketController::GetRocketInstance(
    const Phys::World& world) const {
  return world.GetRocket();
}

Phys::TestRocket& RocketController::GetTestRocketInstance(Phys::World& world) {
  return world.GetTestRocket();
}

const Phys::TestRocket& RocketController::GetTestRocketInstance(
    const Phys::World& world) const {
  return world.GetTestRocket();
}
