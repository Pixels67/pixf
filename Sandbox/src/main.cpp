#include "Flock.hpp"

int main() {
    Flock::Debug::Logger &logger = Flock::Debug::Logger::Create("Sandbox", {.color = true});

    logger.LogInf("Hello, Sandbox!");
}