#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <functional>
#include <utility>
#include <memory>

#include <SDL2/SDL.h>

#include "common.h"

namespace window_manager {

struct SDL2Destructor {
  auto operator()(SDL_Window *w)   const -> void;
  auto operator()(SDL_Renderer *r) const -> void;
};

struct WindowManager {
  static constexpr SDL2Destructor sdl2_destructor{};

  WindowManager(uint32_t width = 1024, uint32_t height = 512);
  ~WindowManager();

public:
  auto run(void) -> void;

private:
  auto free_resources(void) -> void;

public:
  std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>     window{nullptr, sdl2_destructor};
  std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer*)>> renderer{nullptr, sdl2_destructor};

  std::pair<uint32_t, uint32_t> dimensions;
};

}


#endif // WINDOW_MANAGER_H