#pragma once

#include <string>
#include <unordered_map>

#include <Eigen/Core>

#include "Renderer/GL.h"

namespace Ren {

class Shader {
 public:
  Shader() = default;
  Shader(GLuint id) : m_id(id) {}
  ~Shader();

  void Use() const;
  void Destroy();

  bool CreateFromMemory(const std::string& src);
  bool Create(const std::string& path);

  bool CreateFromMemory(std::string& srcVertex, std::string& srcFragment);
  bool Create(const std::string& pathVertex, const std::string& pathFragment);

  GLuint GetID() const;

  GLint GetUniformLocation(const std::string& Location);
  GLint GetAttribLocation(const std::string& Location) const;

  GLint GetLocation(const std::string& location);

  void Set(const std::string& location, GLint value);
  void Set(const std::string& location, GLfloat value);

  void Set(const std::string& location, const Eigen::Vector2i& value);
  void Set(const std::string& location, const Eigen::Vector3i& value);
  void Set(const std::string& location, const Eigen::Vector4i& value);

  void Set(const std::string& location, const Eigen::Vector2f& value);
  void Set(const std::string& location, const Eigen::Vector3f& value);
  void Set(const std::string& location, const Eigen::Vector4f& value);

  void Set(const std::string& location, const Eigen::Vector2d& value);
  void Set(const std::string& location, const Eigen::Vector3d& value);
  void Set(const std::string& location, const Eigen::Vector4d& value);

  void Set(const std::string& location,
           const Eigen::Matrix<float, 2, 2>& value);
  void Set(const std::string& location,
           const Eigen::Matrix<float, 2, 3>& value);
  void Set(const std::string& location,
           const Eigen::Matrix<float, 2, 4>& value);

  void Set(const std::string& location,
           const Eigen::Matrix<float, 3, 3>& value);
  void Set(const std::string& location,
           const Eigen::Matrix<float, 3, 2>& value);
  void Set(const std::string& location,
           const Eigen::Matrix<float, 3, 4>& value);

  void Set(const std::string& location,
           const Eigen::Matrix<float, 4, 4>& value);
  void Set(const std::string& location,
           const Eigen::Matrix<float, 4, 2>& value);
  void Set(const std::string& location,
           const Eigen::Matrix<float, 4, 3>& value);

 private:
  void SetID(GLuint id);

  static std::string ReadFile(const std::string& path);
  GLuint LoadShader(const char* src, GLint length, GLenum type);

  static GLuint LinkShaders(GLuint vertex, GLuint fragment);

  static bool GetLinkStatusError(GLuint program, std::string& out);
  static bool GetShaderErrorStatus(GLuint program, std::string& out);

  static const char* GetShaderTypeString(GLenum type);

  GLuint m_id = 0;
  std::unordered_map<std::string, GLuint> m_uniforms;
  static constexpr std::string_view m_version = "#version 330 core\n";
};

}  // namespace Ren
