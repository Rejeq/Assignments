#pragma once

#include <Eigen/Core>

#include "Renderer/CameraBase.h"
#include "Renderer/Common.h"

namespace Ren {

inline float radians(float degrees) {
  return degrees * static_cast<float>(0.01745329251994329576923690768489);
}

enum class MoveDirection {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  WORLD_UP,
  WORLD_DOWN,
};

// Base class for 3 dimensional cameras
class Camera3d : public CameraBase {
 public:
  void ShiftPos(MoveDirection dir, float factor);
  void ShiftFront(float xoffset, float yoffset);
  void Resize(float w, float h);

  void LookAtPlane(Plane plane, float sign = +1.0f);

  void SetPosition(const Eigen::Vector3f& pos) {
    m_pos = pos;
    p_updateView = true;
  }

  void SetWorldUp(const Eigen::Vector3f& worldUp) {
    m_worldUp = worldUp;
    p_updateView = true;
  }

  void SetYaw(float yaw) {
    m_yaw = yaw;
    p_updateView = true;
  }

  void SetPitch(float pitch) {
    m_pitch = pitch;
    p_updateView = true;
  }

  Eigen::Matrix4f GetProjView() const { return m_proj * m_view; }
  const Eigen::Matrix4f& GetView() const { return m_view; }
  const Eigen::Matrix4f& GetProj() const { return m_proj; }
  const Eigen::Vector3f& GetPos() const { return m_pos; }
  const Eigen::Vector2f& GetSize() const { return m_size; }
  const float& GetWidth() const { return m_size.x(); }
  const float& GetHeight() const { return m_size.y(); }
  float GetYaw() const { return m_yaw; }
  float GetPitch() const { return m_pitch; }

 protected:
  void UpdateViewMatrix() override;

  Eigen::Vector3f m_pos = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
  Eigen::Vector3f m_front = Eigen::Vector3f(0.0f, 0.0f, -1.0f);
  Eigen::Vector3f m_worldUp = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
  Eigen::Vector2f m_size = Eigen::Vector2f(400.0f, 400.0f);

  Eigen::Vector3f m_right;
  Eigen::Vector3f m_up;

  float m_yaw = 0.0f;
  float m_pitch = 0.0f;

  Eigen::Matrix4f m_view;
  Eigen::Matrix4f m_proj;
};

class PerspectiveCamera3d : public Camera3d {
 public:
  void SetFov(float fov) {
    m_fov = fov;
    p_updateProj = true;
  }
  float GetFov() const { return m_fov; }

 private:
  void UpdateProjMatrix() override;

  float m_fov = radians(45.0f);
};

class OrthoCamera3d : public Camera3d {
 public:
  void Zoom(float factor);
  void Zoom(float xFactor, float yFactor);

  void SetScaleX(float x) {
    m_scaleX = x;
    p_updateProj = true;
  }
  float GetScaleX() const { return m_scaleX; }

  void SetScaleY(float y) {
    m_scaleY = y;
    p_updateProj = true;
  }
  float GetScaleY() const { return m_scaleY; }

 private:
  void UpdateProjMatrix() override;

  float m_scaleX = 2.0f;
  float m_scaleY = 2.0f;
};

}  // namespace Ren
