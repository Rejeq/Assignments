#include "Renderer/ShaderDrawer.h"

using namespace Ren;

void ShaderDrawer::Draw() const {
  p_shader.Use();

  glDrawArrays(GL_TRIANGLES, 0, m_verticesCount);
}
