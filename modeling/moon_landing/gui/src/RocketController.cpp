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

CurrentDataView* RocketController::GetCurrentDataView() {
  return m_currData.data();
}

PlotView* RocketController::GetPlotView() {
  return m_plot.data();
}

SceneView* RocketController::GetSceneView() {
  return m_scene.data();
}

SetupView* RocketController::GetSetupView() {
  return m_setup.data();
}

WorldController* RocketController::GetWorldController() {
  return m_worldController.data();
}

void RocketController::ShiftFuellEmission(double delta) {
  LOG_INFO("RocketController: Fuel emission chagned by delta: {}", delta);

  Phys::World& world = m_worldController->LockWorld();
  Phys::Rocket& rocket = GetRocketInstance(world);
  Phys::TestRocket& testRocket = GetTestRocketInstance(world);
  int speed = m_worldController->GetSpeed();

  double emission = rocket.GetFuelEmission();
  rocket.SetFuelEmission(emission + delta * speed);
  testRocket.SetFuelEmission(emission + delta * speed);

  if (m_currData != nullptr)
    m_currData->Update(world.GetLifetime(), rocket);

  m_worldController->UnlockWorld();
}

double RocketController::GetMaxFuelEmission() const {
  const Phys::World& world = m_worldController->LockWorld();

  const Phys::Rocket& rocket = GetRocketInstance(world);
  double maxEmission = rocket.GetMaxFuelEmission();

  m_worldController->UnlockWorld();

  return maxEmission;
}

void RocketController::OnWorldUpdate(Phys::World& world) {
  if (m_landed) {
    LOG_TRACE("RocketController: Rocket already landed, so it is no updated");
    return;
  }

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
    m_landed = true;
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

  m_landed = false;

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
