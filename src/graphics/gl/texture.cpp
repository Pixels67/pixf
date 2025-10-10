#include "texture.h"

#include <iostream>
#include <ostream>
#include <string>

#include "glad/glad.h"
#include "stb_image.h"

namespace pixf::graphics::gl {
bool TextureConfig::operator==(const TextureConfig& config) const {
  return this->interp_mode == config.interp_mode && this->wrap_mode == config.wrap_mode;
}

bool TextureConfig::operator!=(const TextureConfig& config) const {
  return this->interp_mode != config.interp_mode || this->wrap_mode != config.wrap_mode;
}

Texture::Texture(const std::string& path, const TextureConfig config) { Init(path, config); }

Texture::Texture(const Texture& other) { Init(other.path_, other.config_); }

Texture& Texture::operator=(const Texture& other) {
  if (this == &other) {
    return *this;
  }

  if (id_ != 0) {
    this->~Texture();
  }

  Init(other.path_, other.config_);

  return *this;
}

Texture::Texture(Texture&& other) noexcept {
  if (this == &other) {
    return;
  }

  path_ = std::move(other.path_);
  config_ = other.config_;
  id_ = other.id_;
  other.id_ = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  if (id_ != 0) {
    this->~Texture();
  }

  path_ = std::move(other.path_);
  config_ = other.config_;
  id_ = other.id_;
  other.id_ = 0;

  return *this;
}

Texture::~Texture() { glDeleteTextures(1, &id_); }

void Texture::Bind(const unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::Unbind(const unsigned int slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, 0);
}

std::string Texture::GetPath() const { return path_; }
TextureConfig Texture::GetConfig() const { return config_; }

void Texture::Init(const std::string& path, const TextureConfig config) {
  path_ = path;
  config_ = config;

  stbi_set_flip_vertically_on_load(1);
  int width = 0;
  int height = 0;
  int channels = 0;

  unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  if (data == nullptr) {
    std::cerr << "Failed to load texture: " << path << '\n';
    std::exit(EXIT_FAILURE);
  }

  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);

  unsigned int wrap_mode = 0;
  switch (config.wrap_mode) {
    case TextureConfig::WrapMode::MIRRORED_REPEAT:
      wrap_mode = GL_MIRRORED_REPEAT;
      break;
    case TextureConfig::WrapMode::CLAMP_TO_EDGE:
      wrap_mode = GL_CLAMP_TO_EDGE;
      break;
    default:
      wrap_mode = GL_REPEAT;
      break;
  }

  unsigned int interp_mode = 0;
  switch (config.interp_mode) {
    case TextureConfig::InterpMode::LINEAR:
      interp_mode = GL_LINEAR;
      break;
    default:
      interp_mode = GL_NEAREST;
      break;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interp_mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interp_mode);

  unsigned int format = 0;

  switch (channels) {
    case 4:
      format = GL_RGBA;
      break;
    default:
      format = GL_RGB;
      break;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);
}
}  // namespace pixf::graphics::gl
