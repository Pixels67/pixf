#include "engine.h"

int main() {
  engine::Initialize(800, 600, "Hello Cubes");

  while (!engine::ShouldClose()) {
    engine::Update();
  }

  engine::Terminate();
}
