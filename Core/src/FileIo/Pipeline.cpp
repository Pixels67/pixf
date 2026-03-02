#include "Pipeline.hpp"

#include <sstream>

#include "File.hpp"

namespace Flock::FileIo {
    static constexpr auto s_PositionPrepend  = "layout(location = 0) ";
    static constexpr auto s_NormalPrepend    = "layout(location = 1) ";
    static constexpr auto s_TexCoordsPrepend = "layout(location = 2) ";
    static constexpr auto s_TangentPrepend   = "layout(location = 3) ";
    static constexpr auto s_BitangentPrepend = "layout(location = 4) ";

    std::optional<Graphics::Pipeline> ReadPipeline(const std::filesystem::path &filePath) {
        using namespace Flock::Graphics;

        std::optional<std::string> result = ReadTextFile(filePath);
        if (!result.has_value()) {
            return std::nullopt;
        }

        std::istringstream text(result.value());

        std::string vertex;
        std::string fragment;

        std::string               line;
        std::optional<ShaderType> current = std::nullopt;
        while (std::getline(text, line)) {
            if (line == "#pragma vertex") {
                current = VertexShader;
                continue;
            }

            if (line == "#pragma fragment") {
                current = FragmentShader;
                continue;
            }

            if (line == "in vec3 aPosition;" && current.has_value() && current.value() == VertexShader) {
                line = s_PositionPrepend + line;
            }

            if (line == "in vec3 aNormal;" && current.has_value() && current.value() == VertexShader) {
                line = s_NormalPrepend + line;
            }

            if (line == "in vec2 aTexCoords;" && current.has_value() && current.value() == VertexShader) {
                line = s_TexCoordsPrepend + line;
            }

            if (line == "in vec3 aTangent;" && current.has_value() && current.value() == VertexShader) {
                line = s_TangentPrepend + line;
            }

            if (line == "in vec3 aBitangent;" && current.has_value() && current.value() == VertexShader) {
                line = s_BitangentPrepend + line;
            }

            if (current) {
                switch (current.value()) {
                    case VertexShader:
                        vertex += line + '\n';
                        break;
                    case FragmentShader:
                        fragment += line + '\n';
                        break;
                }
            } else {
                vertex   += line + '\n';
                fragment += line + '\n';
            }
        }

        auto vertShader = Shader::Create(VertexShader, vertex);
        auto fragShader = Shader::Create(FragmentShader, fragment);
        if (!vertShader || !fragShader) {
            return std::nullopt;
        }

        auto pipeline = Pipeline::Create(vertShader.value(), fragShader.value());
        return pipeline;
    }
}
