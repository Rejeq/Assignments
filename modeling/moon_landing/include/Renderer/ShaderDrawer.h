#pragma once

#include "Renderer/Common.h"
#include "Renderer/Drawable.h"

namespace Ren {

class ShaderDrawer : public Drawable {
 public:
  void Draw() const override;

  void SetPlane(Plane plane);
  Plane GetPlane() const { return m_plane; }

  void SetVerticesCount(int count) { m_verticesCount = count; }
  int GetVerticesCount() const { return m_verticesCount; }

 private:
  Plane m_plane;
  int m_verticesCount = 0;
};

}  // namespace Ren
