#pragma once

#include <cstdint>

#include <Eigen/Core>

namespace Ren {

struct Vertex {
  using Pos = Eigen::Vector3f;

  Pos pos;

  Vertex(float x, float y, float z) : pos(x, y, z) {}
  Vertex(const Pos& pos) : pos(pos) {}
  Vertex(Pos&& pos) : pos(std::move(pos)) {}
};

using Ind = uint16_t;
using Color = Eigen::Vector4f;

enum class Plane {
  XY,
  XZ,
  ZY,
};

struct RgbColor {
  Color color;

  RgbColor(float r, float g, float b, float a)
      : color(r, g, b, a) {}

  operator Color() const { return color / 255.0f; }
};

}  // namespace Ren
