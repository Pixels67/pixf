#ifndef PIXF_APPLICATION_HPP
#define PIXF_APPLICATION_HPP

#include "Graphics/Gl/Window.hpp"
#include "Pipeline.hpp"
#include "State.hpp"

namespace Pixf::Core::Application {
    struct PIXF_API ApplicationConfig {
        Graphics::Gl::WindowConfig windowConfig;
    };

    class PIXF_API Application {
    public:
        explicit Application(const ApplicationConfig &config = {});

        Application(const Application &) = delete;
        Application(Application &&) noexcept = delete;
        Application &operator=(const Application &) = delete;
        Application &operator=(Application &&) noexcept = delete;

        virtual ~Application() = default;

        void Run();

        State &GetState();

        template<typename T>
        void AttachStage(T stage = {}) {
            m_Pipeline.Attach<T>(m_State, stage);
        }

        template<typename T>
        void DetachStage() {
            m_Pipeline.Detach<T>(m_State);
        }

    protected:
        virtual void Awake();
        virtual void Update(double deltaTime);
        virtual void Render(double deltaTime);
        virtual void RenderGui(double deltaTime);
        virtual void Shutdown();

    private:
        State m_State;
        Pipeline m_Pipeline;
        ApplicationConfig m_Config;
    };
} // namespace Pixf::Core::Application

#define PIXF_RUN_APPLICATION(application)                                                                              \
    int main() {                                                                                                       \
        std::set_terminate(Error::HandleTerminate);                                                                    \
        application app{};                                                                                             \
        app.Run();                                                                                                     \
    }

#define PIXF_CREATE_APPLICATION(application)                                                                           \
    extern "C" {                                                                                                       \
    Pixf::Core::Application::Application *CreateApplication() { return new application(); }                            \
    void DestroyApplication(Pixf::Core::Application::Application *app) { delete app; }                                 \
    }

#endif // PIXF_APPLICATION_HPP
