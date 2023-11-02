#pragma once

#include "Physics/RigidBody.h"

namespace Phys {

class World;

class RocketBase {
 public:
  virtual ~RocketBase() = default;
  virtual void Update(double dt, const World& ctx) = 0;

  void SetHeight(double height) { m_body.pos = Vec3(0.0, height, 0.0); }
  void SetShellMass(double mass) { m_shellMass = std::max(mass, 0.0); }
  void SetFuellMass(double mass) { m_fuelMass = std::max(mass, 0.0); }
  void SetFuelSpeed(double speed) { m_fuelSpeed = speed; }
  void SetFuelEmission(double emission) {
    m_fuelEmission = std::clamp(emission, 0.0, m_maxFuelEmission);
  }
  void SetMaxFuelEmission(double max) {
    m_maxFuelEmission = std::max(max, 0.0);
  }
  void SetLandingSpeed(double speed) { m_landingSpeed = std::max(speed, 0.0); }

  double GetHeight() const { return m_body.pos.y(); }
  double GetVelocity() const { return m_body.velocity.y(); }
  double GetAcceleration() const { return m_body.acceleration.y(); }
  double GetFuelSpeed() const { return m_fuelSpeed; }
  double GetShellMass() const { return m_shellMass; }
  double GetFuellMass() const { return m_fuelMass; }
  double GetFuelEmission() const { return m_fuelEmission; }
  double GetMaxFuelEmission() const { return m_maxFuelEmission; }
  double GetLandingSpeed() const { return m_landingSpeed; }

 protected:
  RigidBody m_body;
  double m_shellMass = 0.0f;
  double m_fuelMass = 0.0f;
  double m_fuelSpeed = 0.0f;
  double m_fuelEmission = 0.0f;
  double m_maxFuelEmission = 0.0f;
  double m_landingSpeed = 0.0f;
};

class Rocket : public RocketBase {
 public:
  void Update(double dt, const World& ctx) override;
  double GetMass() const { return GetShellMass() + GetFuellMass(); }

 private:
  double CalcFuellMass(double dt) const;
  double CalcAcceleration(double fuelJetSpeed, double fuelEmission,
                          double currMass, double gravity) const;
};

class TestRocket : public RocketBase {
 public:
  TestRocket() = default;
  TestRocket(const Rocket& rocket) { (RocketBase&) *this = (const RocketBase&) rocket; }

  void Update(double dt, const World& ctx) override;
  double GetMass(double lifetime) { return (m_shellMass + m_startFuelMass) - m_fuelEmission * lifetime; }
 private:
  double CalcFuellMass(double dt);
  double CalcVelocity(double fuelSpeed, double startMass, double mass, double gravity, double lifetime);

  double m_startFuelMass = 0.0;
  bool m_isFirstIter = true;
};

}  // namespace Phys
