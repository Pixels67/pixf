#include "pixf.h"

int main() {
  pixf::Initialize(800, 600, "Hello Cubes");

  while (!pixf::ShouldClose()) {
    pixf::Update();
  }

  pixf::Terminate();
}
