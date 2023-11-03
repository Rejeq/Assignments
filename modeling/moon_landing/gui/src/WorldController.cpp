#include "WorldController.h"

#include <Log.h>

#define MOON_LOW_PRECISION_TIMER 1000
#define MOON_MID_PRECISION_TIMER 50

PreciseTimerThread::PreciseTimerThread(QObject* parent) : QThread(parent) {
  connect(&m_timer, &QTimer::timeout, this, &PreciseTimerThread::timeout);

  m_timer.setTimerType(Qt::PreciseTimer);
  m_timer.moveToThread(this);
}

PreciseTimerThread::~PreciseTimerThread() {
  m_timer.stop();
}

void PreciseTimerThread::SetInterval(int msec) {
  m_timer.setInterval(msec);
}

void PreciseTimerThread::run() {
  m_timer.start();
  exec();
  m_timer.stop();
}

WorldController::WorldController() {
  m_preciseTimer = new PreciseTimerThread(this);

  connect(
      &m_timer, &QTimer::timeout, this,
      [&] { Update(m_timer.interval() / 1000.0); }, Qt::UniqueConnection);

  connect(
      m_preciseTimer, &PreciseTimerThread::timeout, this,
      [&] {
        // Thread may already destroyed but connection queue can have pending
        // signals that fired from timer thread and have not handle yet
        if (m_preciseTimer->isRunning())
          Update(m_timer.interval() / 1000.0);
      },
      Qt::UniqueConnection);
}

void WorldController::Update(double dt) {
  ConsumeWorld();

  m_world.Update(dt);
  emit Updated(m_world);

  ReturnWorld();
}

WorldController::~WorldController() {
  m_timer.stop();
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

void WorldController::SetAutoUpdate(bool enable) {
  if (enable) {
    LOG_DEBUG("WorldController: Enabling auto update with: {}ms interval",
              m_timer.interval());
    this->disconnect(&m_timer);
  }

  int interval = m_timer.interval();

  if (interval >= MOON_LOW_PRECISION_TIMER) {
    m_timer.setTimerType(Qt::CoarseTimer);
    SetDefaultTimer(enable);
  } else if (interval >= MOON_MID_PRECISION_TIMER) {
    m_timer.setTimerType(Qt::PreciseTimer);
    SetDefaultTimer(enable);
  } else {
    SetPreciseTimer(enable);
  }

  emit AutoUpdateChanged(enable);
}

void WorldController::ToggleAutoUpdate() {
  if (IsAutoUpdateActive())
    DisableAutoUpdate();
  else
    EnableAutoUpdate();
}

void WorldController::SetDefaultTimer(bool enable) {
  if (enable) {
    m_timer.start();
  } else {
    m_timer.stop();
  }
}

void WorldController::SetPreciseTimer(bool enable) {
  if (enable) {
    m_preciseTimer->SetInterval(m_timer.interval());
    m_preciseTimer->start();
  } else {
    this->disconnect(m_preciseTimer);
    m_preciseTimer->quit();
    m_preciseTimer->wait();
  }
}

void WorldController::SetAutoUpdateInterval(int msec) {
  // When interval changed - timer type also can be changed
  bool running = IsAutoUpdateActive();

  if (running) {
    DisableAutoUpdate();
  }

  m_timer.setInterval(msec);
  LOG_DEBUG("WorldController: Auto update interval changed to: {}ms",
            m_timer.interval());

  if (running) {
    EnableAutoUpdate();
  }

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
