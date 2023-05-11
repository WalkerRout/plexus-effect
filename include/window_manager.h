#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <functional>
#include <utility>
#include <memory>

#include <SDL2/SDL.h>

#include "common.h"

namespace window_manager {

struct SDL2Destructor {
  auto operator()(SDL_Window* w) const -> void;
  auto operator()(SDL_Renderer* r) const -> void;
};

struct WindowManager {
  WindowManager(uint32_t width = 1024, uint32_t height = 512);
  ~WindowManager();

public:
  auto run(void) -> void;

private:
  auto free_resources(void) -> void;

public:
  std::unique_ptr<SDL_Window, SDL2Destructor> window{nullptr};
  std::unique_ptr<SDL_Renderer, SDL2Destructor> renderer{nullptr};

  std::pair<uint32_t, uint32_t> dimensions;
};

}


#endif // WINDOW_MANAGER_H