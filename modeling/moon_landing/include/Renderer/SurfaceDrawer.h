#pragma once

#include <vector>

#include "Renderer/Common.h"
#include "Renderer/Drawable.h"
#include "Renderer/GL.h"
#include "Renderer/Shader.h"
#include "Renderer/Surface.h"

namespace Ren {

class SurfaceDrawer : public Drawable {
 public:
  SurfaceDrawer() = default;
  ~SurfaceDrawer();

  void Draw() const override;
  void Destroy() override;

  template <typename Iter>
  void AddSurface(Iter begin, Iter end) {
    size_t totalVert = 0;
    size_t totalInd = 0;

    for (Iter it = begin; it != end; it++) {
      totalVert += it->GetVerticesSize();
      totalInd += it->GetIndicesSize();
    }

    ReserveVertices(totalVert);
    ReserveIndices(totalInd);

    for (Iter it = begin; it != end; it++)
      AddSurface(*it);
  }

  bool AddSurface(const Surface& surface);

  void ReserveVertices(size_t newCapacity);
  void ReserveIndices(size_t newCapacity);

  size_t GetVerticesSize() const { return m_posBuff.GetSize(); }
  size_t GetIndicesSize() const { return m_indBuff.GetSize(); }

  bool IsValid() const { return  m_vao != 0; }

 private:
  void SetupVaoBuffer();

  struct SurfaceData {
    size_t vertOffset;
    size_t vertSize;

    size_t indOffset;
    size_t indSize;
    Ind indMax;
  };

  std::vector<SurfaceData> m_data;

  GLuint m_vao = 0;
  Buffer m_posBuff = Buffer(sizeof(Vertex::Pos));
  Buffer m_colBuff = Buffer(sizeof(Color));
  Buffer m_indBuff = Buffer(sizeof(Ind));
};

}  // namespace Ren
