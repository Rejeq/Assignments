#include "Physics/Rocket.h"

#include <Eigen/Geometry>

#include "Physics/World.h"

using namespace Phys;

void Rocket::Update(double dt, const World& ctx) {
  const RigidBody oldBody = m_body;

  double fuelSpeed = m_fuelSpeed;
  double fuelEmission = m_fuelEmission;
  double gravity = ctx.GetGravity().y();

  double vertNewAccel =
      CalcAcceleration(fuelSpeed, fuelEmission, GetMass(), gravity);
  Vec3 newAccel = Vec3(0.0, vertNewAccel, 0.0);

  m_body.pos += oldBody.velocity * dt + 0.5 * oldBody.acceleration * (dt * dt);
  m_body.velocity += 0.5 * (oldBody.acceleration + newAccel) * dt;
  m_body.acceleration = newAccel;
  m_fuelMass = CalcFuellMass(dt);
}

inline double Rocket::CalcFuellMass(double dt) const {
  return std::max(m_fuelMass - (m_fuelEmission * dt), 0.0);
}

inline double Rocket::CalcAcceleration(double fuelSpeed, double fuelEmission,
                                       double mass, double gravity) const {
  return fuelSpeed * fuelEmission / mass - gravity;
}

void TestRocket::Update(double dt, const World& ctx) {
  const RigidBody oldBody = m_body;

  if (m_isFirstIter) {
    m_startFuelMass = m_fuelMass;
    m_isFirstIter = false;
  }

  double gravity = ctx.GetGravity().y();
  double lifetime = ctx.GetLifetime();

  m_fuelMass = std::max(m_startFuelMass - (m_fuelEmission * lifetime), 0.0);
  m_body.mass = Vec3::Constant(GetMass(lifetime));

  double newVertVelocity =
      CalcVelocity(m_fuelSpeed, m_startFuelMass + m_shellMass, m_body.mass.y(), gravity, lifetime);
  Vec3 newVelocity = Vec3(0.0, newVertVelocity, 0.0);

  m_body.pos += oldBody.velocity * dt;// + 0.5 * oldBody.mass * (dt * dt);
  m_body.velocity = newVelocity;// + 0.5 * (oldBody.mass + m_body.mass) * dt;
}

inline double TestRocket::CalcFuellMass(double dt) {

}

inline double TestRocket::CalcVelocity(double fuelSpeed, double startMass,
                                       double mass, double gravity,
                                       double lifetime) {
  return fuelSpeed * std::log(startMass / mass) - gravity * lifetime;
}
