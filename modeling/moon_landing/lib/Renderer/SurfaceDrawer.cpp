#include "Renderer/SurfaceDrawer.h"

#include <cstdint>
#include <type_traits>

#include "Log.h"

using namespace Ren;

SurfaceDrawer::~SurfaceDrawer() {
  if (IsValid()) {
    LOG_TRACE("Destroying SurfaceDrawer from destructor");
    Destroy();
  }
}

void SurfaceDrawer::Draw() const {
  p_shader.Use();

  glBindVertexArray(m_vao);
  m_indBuff.Bind(GL_ELEMENT_ARRAY_BUFFER);

  static_assert(std::is_same<Ind, uint16_t>::value,
                "glDrawElements requires unsigned short");
  glDrawElements(GL_TRIANGLES, m_indBuff.GetSize(), GL_UNSIGNED_SHORT, 0);
}

void SurfaceDrawer::Destroy() {
  m_data.clear();
  glDeleteVertexArrays(1, &m_vao);
  m_vao = 0;

  Drawable::Destroy();
  m_posBuff.Destroy();
  m_colBuff.Destroy();
  m_indBuff.Destroy();

  LOG_TRACE("SurfaceDrawer are destroyed");
}

bool SurfaceDrawer::AddSurface(const Surface& surface) {
  const Vertex* vertices = surface.GetVertices();
  size_t surVertSize = surface.GetVerticesSize();

  const Ind* indices = surface.GetIndices();
  size_t surIndSize = surface.GetIndicesSize();

  const Color& col = surface.GetColor();

  if (vertices == nullptr || indices == nullptr)
    return true;

  LOG_DEBUG("Adding surface to drawer: Vertices size: {}; Indices size: {}",
            surVertSize, surIndSize);

  SurfaceData data;
  data.vertOffset = GetVerticesSize();
  data.vertSize = surVertSize;

  data.indOffset = GetIndicesSize();
  data.indSize = surIndSize;

  ReserveVertices(GetVerticesSize() + surVertSize);
  m_posBuff.Append(vertices, surVertSize);
  m_colBuff.StretchAppend(col.data(), 1, surVertSize);

  const size_t maxIndex = (m_data.size() != 0) ? m_data.back().indMax + 1 : 0;
  Ind newMaxIndex = maxIndex;

  ReserveIndices(GetIndicesSize() + surIndSize);
  m_indBuff.Append(indices, surIndSize, [maxIndex, &newMaxIndex](void* idx) {
    *(Ind*)idx += maxIndex;
    newMaxIndex = std::max(*(Ind*)idx, newMaxIndex);
  });

  data.indMax = newMaxIndex;
  m_data.emplace_back(std::move(data));

  LOG_DEBUG(
      "Surface added: Vertices offset: {}; Indices offset: {}; Max index: {}",
      m_data.back().vertOffset, m_data.back().indOffset, m_data.back().indMax);

  return false;
}

void SurfaceDrawer::ReserveVertices(size_t newCapacity) {
  bool changed = false;
  changed |= m_posBuff.Reserve(newCapacity);
  changed |= m_colBuff.Reserve(newCapacity);

  if (changed)
    SetupVaoBuffer();
}

void SurfaceDrawer::ReserveIndices(size_t newCapacity) {
  m_indBuff.Reserve(newCapacity);
}

void SurfaceDrawer::SetupVaoBuffer() {
  if (m_vao == 0)
    glGenVertexArrays(1, &m_vao);

  glBindVertexArray(m_vao);

  m_posBuff.Bind(GL_ARRAY_BUFFER);
  glDisableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_posBuff.GetElementSize(),
                        (void*)0);
  glEnableVertexAttribArray(0);

  m_colBuff.Bind(GL_ARRAY_BUFFER);
  glDisableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, m_colBuff.GetElementSize(),
                        (void*)0);
  glEnableVertexAttribArray(1);

  LOG_TRACE("Vao buffer in SurfaceDrawer are changed");
}
