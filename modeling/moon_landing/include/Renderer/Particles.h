#pragma once

#include <Eigen/Core>

namespace Ren {

struct Vertex {
  Eigen::Vector3f pos;
};

using Ind = uint16_t;
using Color = Eigen::Vector4f;

class Particles {
 public:
  Mesh();
  std::unique_ptr<Drawable> MakeDrawable();

  const Vertex* GetVertices() const { return m_vertices.data(); }
  int GetVerticesSize() const { return m_vertices.size(); }

  const Vertex* GetIndices() const { return m_indices.data(); }
  int GetIndicesSize() const { return m_indices.size(); }

  const Color* GetColor() const { return m_color.data(); }

 private:
  std::vector<Vertex> m_vertices;
  std::vector<Ind> m_indices;
  Color m_color;
};

class ParticlesDrawer {
  ParticlesDrawer(const Particles& surface) {
    Vertex* vertices = surface.GetVertices();
    Ind* indices = surface.GetIndices();

    if (vertices == nullptr || indices == nullptr) {
      m_isInvalid = true;
      return;
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_verticesId);
    glGenBuffers(1, &m_indicesId);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_verticesId);

    glBufferData(GL_ARRAY_BUFFER, surface.GetVerticesSize() * sizeof(Vertex),
                 vertices, GL_STATIC_DRAW);

    m_indicesSize = surface.GetIndicesSize();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesSize * sizeof(Ind),
                 indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    assert(vertices[0].pos.size() == 3);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)0);
  }

  bool LoadShaderFromMemory(const char* data, size_t size);
  bool LoadShader(const std::string path);

  void Update(const Particles& particles);

  bool Draw(Shader& shader) const {
    shader.Use();

    glBindVertexArray(m_verticesId);

    static_assert(Ind == uint16_t, "glDrawElements requires unsigned int");
    glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);
  }

  bool IsInvalid() const { return m_isInvalid; }

 private:
  size_t m_indicesSize;
  GLuint m_indicesId;
  GLuint m_verticesId;

  bool m_isInvalid = false;
};

}  // namespace Ren


