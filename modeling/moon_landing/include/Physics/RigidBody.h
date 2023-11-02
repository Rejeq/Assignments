#pragma once

#include <Eigen/Core>

namespace Phys {

using Vec3 = Eigen::Vector3d;

struct RigidBody {
  Vec3 pos = Vec3(0.0, 0.0, 0.0);
  Vec3 mass = Vec3(0.0, 0.0, 0.0);
  Vec3 velocity = Vec3(0.0, 0.0, 0.0);
  Vec3 acceleration = Vec3(0.0, 0.0, 0.0);
};

}  // namespace Phys
