
#include <exception>
#include <iostream>
#include <ctime>

#include <SDL2/SDL.h>

#include "window_manager.h"

static const uint16_t WIDTH = 1320, HEIGHT = 740;

static auto run_plexus_effect(void) -> void {
  auto window_manager = window_manager::WindowManager(WIDTH, HEIGHT); // raii handles resources
  window_manager.run();
}

auto main() -> int {
  srand(time(NULL));

  try {
    run_plexus_effect();
  } catch(const std::exception &e) {
    std::cout << e.what() << '\n';
  }

  return 0;
}
