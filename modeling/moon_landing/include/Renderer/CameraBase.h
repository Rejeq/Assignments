#pragma once

namespace Ren {

class CameraBase {
 public:
  CameraBase() = default;
  virtual ~CameraBase() = default;

  bool Update();

  void ForceUpdateView() { p_updateView = true; }
  void ForceUpdateProjection() { p_updateProj = true; }
  void ForceUpdateMatrix() { p_updateMatrix = true; }

 protected:
  virtual void UpdateViewMatrix() = 0;
  virtual void UpdateProjMatrix() = 0;

  bool p_updateView = true;
  bool p_updateProj = true;
  bool p_updateMatrix = true;
  bool p_usePerspective = true;
};

}  // namespace Ren
