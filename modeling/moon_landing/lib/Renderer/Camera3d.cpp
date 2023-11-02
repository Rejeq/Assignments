#include "Renderer/Camera3d.h"

#include <Eigen/Dense>

using namespace Ren;

void Camera3d::ShiftPos(MoveDirection dir, float factor) {
  switch (dir) {
    case MoveDirection::FORWARD: m_pos += m_front * factor; break;
    case MoveDirection::BACKWARD: m_pos -= m_front * factor; break;
    case MoveDirection::LEFT: m_pos -= m_right * factor; break;
    case MoveDirection::RIGHT: m_pos += m_right * factor; break;
    case MoveDirection::WORLD_UP: m_pos += m_worldUp * factor; break;
    case MoveDirection::WORLD_DOWN: m_pos -= m_worldUp * factor; break;
  }

  p_updateView = true;
}

void Camera3d::ShiftFront(float xoffset, float yoffset) {
  m_yaw += radians(xoffset);
  m_pitch -= radians(yoffset);

  constexpr float quarter = ((float)EIGEN_PI) / 2.0f;
  if (m_pitch > quarter)
    m_pitch = quarter - 0.001f;
  else if (m_pitch < -quarter)
    m_pitch = -quarter + 0.001f;

  p_updateView = true;
}

void Camera3d::Resize(float w, float h) {
  m_size = {w, h};
  p_updateProj = true;
}

void Camera3d::LookAtPlane(Plane plane, float sign) {
  float yaw = 0.0f;
  float pitch = 0.0f;

  switch (plane) {
    case Plane::XY: break;  // Either equall to 0
    case Plane::ZY: yaw = (float)M_PI / 2.0f; break;
    case Plane::XZ: pitch = (float)M_PI / 2.0f; break;
  }

  SetYaw(sign * yaw);
  SetPitch(sign * pitch);
}

void Camera3d ::UpdateViewMatrix() {
  const float cYaw = std::cos(m_yaw);
  const float cPitch = std::cos(m_pitch);
  const float sYaw = std::sin(m_yaw);
  const float sPitch = std::sin(m_pitch);

  m_front.x() = cYaw * cPitch;
  m_front.y() = sPitch;
  m_front.z() = sYaw * cPitch;

  m_front.normalize();
  m_right = m_front.cross(m_worldUp).normalized();
  m_up = m_right.cross(m_front).normalized();

  // LookAt
  Eigen::Vector3f f(m_front.normalized());
  const Eigen::Vector3f s(f.cross(m_up).normalized());
  const Eigen::Vector3f u(s.cross(f));

  m_view = Eigen::Matrix4f::Identity();
  m_view(0, 0) = s.x();
  m_view(0, 1) = s.y();
  m_view(0, 2) = s.z();
  m_view(1, 0) = u.x();
  m_view(1, 1) = u.y();
  m_view(1, 2) = u.z();
  m_view(2, 0) = -f.x();
  m_view(2, 1) = -f.y();
  m_view(2, 2) = -f.z();
  m_view(0, 3) = -s.dot(m_pos);
  m_view(1, 3) = -u.dot(m_pos);
  m_view(2, 3) = f.dot(m_pos);
}

void PerspectiveCamera3d::UpdateProjMatrix() {
  const float zNear = 0.001f;
  const float zFar = 1000.0f;
  const float aspect = m_size.x() / m_size.y();
  const float tanHalfFovy = std::tan(m_fov / 2.0f);

  m_proj = Eigen::Matrix4f::Zero();
  m_proj(0, 0) = 1.0f / (aspect * tanHalfFovy);
  m_proj(1, 1) = 1.0f / (tanHalfFovy);
  m_proj(2, 2) = -(zFar + zNear) / (zFar - zNear);
  m_proj(3, 2) = -1.0f;
  m_proj(2, 3) = -(2.0f * zFar * zNear) / (zFar - zNear);
}

void OrthoCamera3d::Zoom(float factor) {
  SetScaleX(m_scaleX - factor);
  SetScaleY(m_scaleY - factor);
}

void OrthoCamera3d::Zoom(float xFactor, float yFactor) {
  SetScaleX(m_scaleX - xFactor);
  SetScaleY(m_scaleY - yFactor);
}

void OrthoCamera3d::UpdateProjMatrix() {
  const float left = -m_scaleX * m_size.x();
  const float right = +m_scaleX * m_size.x();
  const float bottom = -m_scaleY * m_size.y();
  const float top = +m_scaleY * m_size.y();
  const float zNear = -0.001f;
  const float zFar = +100.0f;

  m_proj = Eigen::Matrix4f::Identity();
  m_proj(0, 0) = 2.0f / (right - left);
  m_proj(1, 1) = 2.0f / (top - bottom);
  m_proj(2, 2) = -2.0f / (zFar - zNear);
  m_proj(0, 3) = -(right + left) / (right - left);
  m_proj(1, 3) = -(top + bottom) / (top - bottom);
  m_proj(2, 3) = -(zFar + zNear) / (zFar - zNear);
}
