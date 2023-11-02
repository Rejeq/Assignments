#pragma once

#include <cassert>
#include <map>
#include <vector>

#include <Eigen/Core>

#include "Renderer/Common.h"

namespace Ren {

class Surface {
 public:
  template <typename Container>
  void SetVertices(const Container& vertices) {
    m_vertices = vertices;
  }
  template <typename Container>
  void SetVertices(const Container&& vertices) {
    m_vertices = vertices;
  }
  void SetVertices(const std::initializer_list<Vertex> vertices) {
    m_vertices = vertices;
  }
  template <typename Iter>
  void SetVertices(Iter begin, Iter end) {
    m_vertices = std::vector<Ind>(begin, end);
  }

  template <typename Container>
  void SetIndices(const Container& indices) {
    m_indices = indices;
  }
  template <typename Container>
  void SetIndices(const Container&& indices) {
    m_indices = indices;
  }
  void SetIndices(const std::initializer_list<Ind> indices) {
    m_indices = indices;
  }
  template <typename Iter>
  void SetIndices(Iter begin, Iter end) {
    m_indices = std::vector<Ind>(begin, end);
  }

  void SetColor(const Color& color) { m_color = color; }
  void SetColor(float r, float g, float b, float a) {
    m_color = Color(r, g, b, a);
  }

  const Vertex* GetVertices() const { return m_vertices.data(); }
  int GetVerticesSize() const { return m_vertices.size(); }

  const Ind* GetIndices() const { return m_indices.data(); }
  int GetIndicesSize() const { return m_indices.size(); }

  const Color& GetColor() const { return m_color; }

 private:
  std::vector<Vertex> m_vertices;
  std::vector<Ind> m_indices;
  Color m_color;
};

}  // namespace Ren
