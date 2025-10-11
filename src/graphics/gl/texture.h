#pragma once

#include <string>

namespace pixf::graphics::gl {
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

  InterpMode interp_mode = InterpMode::LINEAR;
  WrapMode wrap_mode = WrapMode::REPEAT;
  bool operator==(const TextureConfig& config) const;
  bool operator!=(const TextureConfig& config) const;
};

class Texture {
 public:
  explicit Texture(const std::string& path, TextureConfig config = {});

  Texture(const Texture& other);
  Texture& operator=(const Texture& other);

  Texture(Texture&& other) noexcept;
  Texture& operator=(Texture&& other) noexcept;

  ~Texture();

  void Bind(unsigned int slot) const;
  static void Unbind(unsigned int slot);

  std::string GetPath() const;
  TextureConfig GetConfig() const;

 private:
  std::string path_;
  unsigned int id_ = 0;
  TextureConfig config_;

  void Init(const std::string& path, TextureConfig config = {TextureConfig::InterpMode::NEAREST,
                                                             TextureConfig::WrapMode::REPEAT});
};
}  // namespace pixf::graphics::gl
