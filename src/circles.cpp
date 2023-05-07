
#include <algorithm>
#include <iostream>
#include <cassert>
#include <random>
#include <cmath>

#include <SDL2/SDL.h>

#include "circles.h"

static auto draw_circle(SDL_Renderer *renderer, uint32_t center_x, uint32_t center_y, uint32_t radius) -> void;
static inline auto rand_range(double a, double b) -> double;

namespace circles {

Circles::Circles(uint32_t count, uint32_t width, uint32_t height) {
  circle_count = count;
  velocities = {VELOCITY, VELOCITY}; // x and y have same velocity range for now
  dimensions = {width, height};
  // dimensions must be initialized before circles and lines are created
  create_circles_and_lines();
}

auto Circles::step(double delta_time) -> void {
  move_circles(delta_time);
  connect_circles();
}

auto Circles::render(SDL_Renderer *renderer) -> void {
  draw_lines(renderer);
  draw_circles(renderer);
}

auto Circles::create_circles_and_lines(void) -> void {
  lines.reserve(circle_count-1);

  circles.reserve(circle_count);
  for(uint32_t i = circle_count; i > 0; --i) {
    auto [width, height] = dimensions;
    auto [vx, vy] = velocities;

    uint32_t x = rand() % width;
    uint32_t y = rand() % height;
    double vel_x = rand_range(-vx, vx);
    double vel_y = rand_range(-vy, vy);

    circles.emplace_back(CircleVector{x, y, vel_x, vel_y});
  }
}

inline auto Circles::move_circles(double delta_time) -> void {
  auto [width, height] = dimensions;

  std::ranges::transform(circles, std::begin(circles), [delta_time, width, height](auto& circle) {
    auto [x, y, dx, dy] = circle;
    x += dx * delta_time;
    y += dy * delta_time;

    if(x >= width || x <= 0)
      dx *= -1;

    if(y >= height || y <= 0)
      dy *= -1;

    return CircleVector{x, y, dx, dy};
  });
}

auto Circles::connect_circles(void) -> void {
  lines.clear(); // does not deallocate memory; emplace back can be used

  for(uint32_t i = 0; i < circle_count-1; ++i) {
    for(uint32_t j = i + 1; j < circle_count; ++j){
      auto [xa, ya, _dxa, _dya] = circles[i];
      auto [xb, yb, _dxb, _dyb] = circles[j];

      auto pos_a = SDL_Point(xa, ya);
      auto pos_b = SDL_Point(xb, yb);

      lines.emplace_back(LineVector{pos_a, pos_b});
    }
  }
}

auto Circles::draw_lines(SDL_Renderer *renderer) -> void {
  for(auto& line : lines) {
    // bind start and end positions
    auto [start_pos, end_pos] = line;
    // bind x and y components of positions
    auto [start_x, start_y] = start_pos;
    auto [end_x, end_y] = end_pos;

    double distance = sqrt(pow(start_x - end_x, 2) + pow(start_y - end_y, 2));

    if(distance < MAX_DISTANCE) {
      auto [r, g, b] = color(distance, MAX_DISTANCE);

      (void) SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
      if(SDL_RenderDrawLine(renderer, start_x, start_y, end_x, end_y) < 0)
        throw std::runtime_error("Error - unable to draw line in Circles::draw_lines");
    }
  }
}

inline auto Circles::draw_circles(SDL_Renderer *renderer) -> void {
  for(auto& circle : circles) {
    auto [x, y, _dx, _dy] = circle;

    (void) SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    draw_circle(renderer, x, y, 2);
  }
}

inline auto Circles::color(double distance, double max_distance) const -> std::tuple<uint8_t, uint8_t, uint8_t> {
  uint8_t x = ((double) (max_distance - distance) * 255.0 / max_distance);
  return {static_cast<uint8_t>(x), 0, 0};
}

}

// file local functions

// Midpoint Circle Algorithm
static auto draw_circle(SDL_Renderer *renderer, uint32_t center_x, uint32_t center_y, uint32_t radius) -> void {
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y) {
      //  Each of the following renders an octant of the circle
      (void) SDL_RenderDrawPoint(renderer, center_x + x, center_y - y);
      (void) SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
      (void) SDL_RenderDrawPoint(renderer, center_x - x, center_y - y);
      (void) SDL_RenderDrawPoint(renderer, center_x - x, center_y + y);
      (void) SDL_RenderDrawPoint(renderer, center_x + y, center_y - x);
      (void) SDL_RenderDrawPoint(renderer, center_x + y, center_y + x);
      (void) SDL_RenderDrawPoint(renderer, center_x - y, center_y - x);
      (void) SDL_RenderDrawPoint(renderer, center_x - y, center_y + x);

      if(error <= 0) {
        ++y;
        error += ty;
        ty += 2;
      }

      if(error > 0) {
        --x;
        tx += 2;
        error += (tx - diameter);
      }
   }
}

static inline auto rand_range(double a, double b) -> double {
  static std::random_device rand_dev;
  static std::mt19937 generator(rand_dev());

  std::uniform_real_distribution<double> distr(a, b);

  return distr(generator);
}
