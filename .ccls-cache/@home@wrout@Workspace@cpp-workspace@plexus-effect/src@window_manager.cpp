
#include <iostream>

#include <SDL2/SDL.h>

#include "window_manager.h"
#include "circles.h"

namespace window_manager {

auto SDL2Destructor::operator()(SDL_Window* w)   const -> void { SDL_DestroyWindow(w); }
auto SDL2Destructor::operator()(SDL_Renderer* r) const -> void { SDL_DestroyRenderer(r); }

WindowManager::WindowManager(uint32_t width, uint32_t height): dimensions({width, height}) {
  const uint32_t DEFAULT_INIT_FLAGS = 0;
  const uint32_t DEFAULT_WINDOW_FLAGS = 0;
  const char *TITLE = "Plexus Effect";

  if(SDL_Init(DEFAULT_INIT_FLAGS) < 0) goto fail;

  window = std::unique_ptr<SDL_Window, SDL2Destructor>(
    SDL_CreateWindow(
      TITLE, 
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
      width, height, 
      DEFAULT_WINDOW_FLAGS
    )
  );
  if(window == nullptr) goto fail;

  renderer = std::unique_ptr<SDL_Renderer, SDL2Destructor>(
    SDL_CreateRenderer(
      window.get(),
     -1, 0
    )
  );
  if(renderer == nullptr) goto fail;

  return; // return before fail

fail:
  free_resources();
  auto error_msg = std::string("Error - unable to initialize SDL2 in WindowManager(uint32_t, uint32_t) constructor, terminated with: ");
  throw std::runtime_error(error_msg + SDL_GetError());
}

WindowManager::~WindowManager() { free_resources(); }

auto WindowManager::run(void) -> void {
  constexpr uint32_t FPS = 60;
  constexpr uint32_t CIRCLE_COUNT = 110;
<<<<<<< Updated upstream
  constexpr double TUNING_PARAMETER = 5 * (double) FPS / 1000.0;
=======
  constexpr double TUNING_PARAMETER = (double) FPS / 1000.0;
>>>>>>> Stashed changes

  // generate circles
  auto [width, height] = dimensions;
  auto circles = circles::Circles(CIRCLE_COUNT, width, height);

  // delta time
  uint64_t curr_time = SDL_GetPerformanceCounter();
  uint64_t prev_time = 0;
  double delta_time = 0.0;

  auto keep_alive = true;
  while(keep_alive) {
    prev_time = curr_time;
    curr_time = SDL_GetPerformanceCounter();
    delta_time = ((double) (curr_time - prev_time) * 1000.0 / (double) SDL_GetPerformanceFrequency());

    circles.step(delta_time * TUNING_PARAMETER);

    SDL_Event event = {0};
    while(SDL_PollEvent(&event) > 0) {
      switch(event.type) {
      case SDL_QUIT:
        keep_alive = false;
        break;
      
      // TODO; keypresses

      default: {}
      }
    }

    // rendering
    (void) SDL_SetRenderDrawColor(renderer.get(), 0x00, 0x00, 0x00, 0xFF);
    (void) SDL_RenderClear(renderer.get());

    circles.render(renderer.get());

    (void) SDL_RenderPresent(renderer.get());

    (void) SDL_Delay(FPS / 1000.0);
  }
}

auto WindowManager::free_resources(void) -> void { SDL_Quit(); }

}

// file local functions
