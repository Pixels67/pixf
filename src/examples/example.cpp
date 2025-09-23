#include "engine.h"

int main() {
	Engine::Initialize(800, 600, "Hello Clippy");

	while (!Engine::ShouldClose()) {
		Engine::Update();
	}

	Engine::Terminate();
}
