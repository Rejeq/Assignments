#pragma once

#include <string>

#include "Renderer/Shader.h"

namespace Ren {

class Drawable {
 public:
  virtual ~Drawable() = default;

  virtual void Draw() const = 0;
  virtual void Destroy() { p_shader.Destroy(); }

  bool CreateShaderFromMemory(const std::string& src) {
    return p_shader.CreateFromMemory(src);
  }

  const Shader& GetShader() const { return p_shader; }
  Shader& GetShader() { return p_shader; }

 protected:
  Shader p_shader;
};

}  // namespace Ren
