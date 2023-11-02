#pragma once

#include <QObject>
#include <QTimer>
#include <QMutex>

#include <Physics/World.h>

class WorldController : public QObject {
  Q_OBJECT

 public:
  WorldController();

  void Update(double dt);
  void Step();
  void Reset();
  void Restart();

  void ToggleAutoUpdate();
  void EnableAutoUpdate();
  void DisableAutoUpdate();

  void SetAutoUpdateInterval(int msec);
  void SetAutoUpdateIntervalSec(double sec);
  int GetAutoUpdateInterval() const { return m_timer.interval(); }

  Phys::World& ConsumeWorld();
  void ReturnWorld();

 signals:
  void Updated(Phys::World& world);
  void Reseted(Phys::World& world);
  void AutoUpdateChanged(bool enabled);
  void IntervalChanged(int msec);

 private:
  bool IsAutoUpdateActive() const { return m_timer.isActive(); }

  Phys::World m_world;
  QTimer m_timer;
  QMutex m_mutex;
};
