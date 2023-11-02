#include "Physics/World.h"

using namespace Phys;

void World::Update(double dt) {
  m_rocket.Update(dt, *this);
  m_testRocket.Update(dt, *this);

  m_lifetime += dt;
}

void World::Clear() {
  m_gravity = Vec3(0.0, -9.807, 0.0);
  m_rocket = Rocket();
  m_lifetime = 0.0;
}
