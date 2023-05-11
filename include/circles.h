#ifndef CIRCLES_H
#define CIRCLES_H

#include <tuple>

#include <SDL2/SDL.h>

#include "common.h"

namespace circles {

struct Circles {
  static constexpr double VELOCITY = 0.8;
  static constexpr uint16_t MAX_DISTANCE = 200;

  // x, y, dx, dy
  using CircleVector = typename std::tuple<double, double, double, double>;
  // (x1, y1), (x2, y2)
  using LineVector = typename std::pair<SDL_Point, SDL_Point>;

  Circles() = delete;
  Circles(uint16_t count, uint32_t width, uint32_t height);

public:
  auto step(double delta_time) -> void;
  auto render(SDL_Renderer* renderer) -> void;

private:
  // Circles(uint32_t, uint32_t, uint32_t)
  auto create_circles_and_lines(void) -> void;

  // step(double)
  auto move_circles(double delta_time) -> void;
  auto connect_circles(void) -> void;

  // render(SDL_Renderer *renderer)
  auto draw_lines(SDL_Renderer* renderer) -> void;
  auto draw_circles(SDL_Renderer* renderer) -> void;

  auto color(double distance, double max_distance) const -> std::tuple<uint8_t, uint8_t, uint8_t>;

// private:
public:
  uint16_t circle_count;
  std::pair<uint32_t, uint32_t> dimensions; // window dimensions

  // x, y
  std::pair<double, double> velocities;

  std::vector<CircleVector> circles;
  std::vector<LineVector> lines;
};

}

#endif // CIRCLES_H