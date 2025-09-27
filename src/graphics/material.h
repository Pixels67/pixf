#pragma once

#include <vec4.hpp>

#include "shader.h"
#include "texture.h"

namespace engine::graphics {
struct Material {
  glm::vec4 color;
  Texture texture;
  Shader shader;

  void SetColor(const glm::vec4& color);

  void Bind() const;
  static void Unbind();
};
}  // namespace engine::graphics
