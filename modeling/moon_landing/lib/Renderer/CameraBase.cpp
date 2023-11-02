#include "Renderer/CameraBase.h"

using namespace Ren;

bool CameraBase::Update() {
  bool out = p_updateMatrix;
  p_updateMatrix = false;

  if (p_updateView) {
    UpdateViewMatrix();
    p_updateView = false;
    out = true;
  }
  if (p_updateProj) {
    UpdateProjMatrix();
    p_updateProj = false;
    out = true;
  }

  return out;
}
