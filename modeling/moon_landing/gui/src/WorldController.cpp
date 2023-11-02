#include "WorldController.h"

#include <Log.h>

WorldController::WorldController() {
  connect(&m_timer, &QTimer::timeout, this,
          [&]() { Update(m_timer.interval() / 1000.0); });
}

void WorldController::Update(double dt) {
  ConsumeWorld();

  m_world.Update(dt);
  emit Updated(m_world);

  ReturnWorld();
}

void WorldController::Step() {
  Update(GetAutoUpdateInterval() / 1000.0);
}

void WorldController::Reset() {
  ConsumeWorld();

  DisableAutoUpdate();
  m_world.Clear();
  emit Reseted(m_world);

  ReturnWorld();
}

void WorldController::Restart() {
  bool wasActive = IsAutoUpdateActive();

  Reset();
  if (wasActive)
    EnableAutoUpdate();
}

void WorldController::ToggleAutoUpdate() {
  if (IsAutoUpdateActive())
    DisableAutoUpdate();
  else
    EnableAutoUpdate();
}

void WorldController::EnableAutoUpdate() {
  LOG_DEBUG("WorldController: Enabling auto update with: {}ms interval",
            m_timer.interval());

  m_timer.start();

  emit AutoUpdateChanged(true);
}

void WorldController::DisableAutoUpdate() {
  m_timer.stop();

  emit AutoUpdateChanged(false);
}

void WorldController::SetAutoUpdateInterval(int msec) {
  m_timer.setInterval(msec);
  LOG_DEBUG("WorldController: Auto update interval changed to: {}ms",
            m_timer.interval());

  emit IntervalChanged(m_timer.interval());
}

void WorldController::SetAutoUpdateIntervalSec(double sec) {
  SetAutoUpdateInterval(sec * 1000.0);
}

Phys::World& WorldController::ConsumeWorld() {
  m_mutex.lock();
  return m_world;
}

void WorldController::ReturnWorld() {
  m_mutex.unlock();
}
