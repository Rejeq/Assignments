#pragma once

#include "Physics/Rocket.h"

namespace Phys {

class World {
 public:
  void Update(double dt);
  void Clear();

  void SetRocket(const Rocket& rocket) { m_rocket = rocket; }
  const Rocket& GetRocket() const { return m_rocket; }
  Rocket& GetRocket() { return m_rocket; }

  void SetTestRocket(const TestRocket& rocket) { m_testRocket = rocket; }
  const TestRocket& GetTestRocket() const { return m_testRocket; }
  TestRocket& GetTestRocket() { return m_testRocket; }

  void SetGravity(const Vec3& gravity) { m_gravity = gravity; }
  const Vec3& GetGravity() const { return m_gravity; }

  double GetLifetime() const { return m_lifetime; }

 private:
  Rocket m_rocket;
  TestRocket m_testRocket;

  Vec3 m_gravity = Vec3(0.0, -9.807, 0.0);
  double m_lifetime = 0.0;
};

}  // namespace Phys
