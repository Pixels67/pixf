#pragma once

#include <string>

namespace engine::graphics {
struct TextureConfig {
  enum class InterpMode : char {
    NEAREST,
    LINEAR,
  };

  enum class WrapMode : char {
    REPEAT,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
  };

  InterpMode interp_mode = InterpMode::NEAREST;
  WrapMode wrap_mode = WrapMode::REPEAT;
};

class Texture {
 public:
  explicit Texture(const std::string& path,
                   TextureConfig config = {TextureConfig::InterpMode::NEAREST,
                                           TextureConfig::WrapMode::REPEAT});

  Texture(const Texture& other);

  Texture& operator=(const Texture& other);

  Texture(Texture&& other);

  Texture& operator=(Texture&& other);

  ~Texture();

  void Bind(unsigned int slot) const;

  static void Unbind(unsigned int slot);

 private:
  std::string path_;
  unsigned int id_ = 0;
  unsigned int width_ = 0;
  unsigned int height_ = 0;
  TextureConfig config_;
};
}  // namespace engine::graphics
