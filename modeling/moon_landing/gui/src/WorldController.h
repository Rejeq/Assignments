#pragma once

#include <QMutex>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QPointer>

#include <Physics/World.h>

class PreciseTimerThread : public QThread {
  Q_OBJECT

 public:
  PreciseTimerThread(QObject* parent = nullptr);
  ~PreciseTimerThread();

  void SetInterval(int msec);

 signals:
  void timeout();

 protected:
  void run() override;

 private:
  QTimer m_timer;
};

class WorldController : public QObject {
  Q_OBJECT

 public:
  WorldController();
  ~WorldController();

  void Update(double dt);
  void Step();
  void Reset();
  void Restart();

  void SetAutoUpdate(bool enable);
  void ToggleAutoUpdate();
  void EnableAutoUpdate() { SetAutoUpdate(true); }
  void DisableAutoUpdate() { SetAutoUpdate(false); }

  void SetAutoUpdateInterval(int msec);
  void SetAutoUpdateIntervalSec(double sec);
  int GetAutoUpdateInterval() const { return m_timer.interval(); }

  void SetSpeed(int speed);
  int GetSpeed() const { return m_speed; }

  Phys::World& LockWorld();
  void UnlockWorld();

 signals:
  void Updated(Phys::World& world);
  void Reseted(Phys::World& world);
  void AutoUpdateChanged(bool enabled);
  void IntervalChanged(int msec);
  void SpeedChanged(int speed);

 private:
  bool IsAutoUpdateActive() const {
    return m_timer.isActive() || m_preciseTimer->isRunning();
  }

  void SetDefaultTimer(bool enable);
  void SetPreciseTimer(bool enable);

  Phys::World m_world;
  QTimer m_timer;
  QPointer<PreciseTimerThread> m_preciseTimer;
  QMutex m_mutex;
  int m_speed = 1;
};
