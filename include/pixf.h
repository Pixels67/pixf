#pragma once

#include "core/transform.h"
#include "graphics/camera.h"
#include "graphics/lighting/point_light.h"
#include "graphics/material.h"
#include "graphics/model.h"
#include "graphics/rendering.h"
#include "input/input_manager.h"
#include "instance.h"
#include "time/time.h"

#define INIT(config, init)                 \
  int main() {                             \
    pixf::Instance instance(config, init); \
    instance.Run();                        \
  }