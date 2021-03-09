#include "game_of_life.h"

#include <signal.h>

#include <iostream>
#include <random>
#include <vector>

#include "led-matrix.h"

using rgb_matrix::RGBMatrix;
using rgb_matrix::RuntimeOptions;

std::random_device rd;
std::mt19937 gen(rd());
std::bernoulli_distribution random_bool(0.2);

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) { interrupt_received = true; }

GameOfLife::GameOfLife(const int w, const int h) {
  width = w;
  height = h;

  for (int i = 0; i < height * width; i++) {
    field.push_back(random_bool(gen));
  }
}

int GameOfLife::coord_index(const int x, const int y) { return y * width + x; }

void GameOfLife::set(const int x, const int y, const bool s) {
  field[coord_index(x, y)] = s;
}

bool GameOfLife::is_alive(int x, int y) {
  x += width;
  x %= width;
  y += height;
  y %= height;
  return field[coord_index(x, y)];
}

bool GameOfLife::next(int x, int y) {
  int alive = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if ((j != 0 || i != 0) && is_alive(x + i, y + j)) {
        alive++;
      }
    }
  }

  return alive == 3 || (alive == 2 && is_alive(x, y));
}

void GameOfLife::step() {
  std::vector<bool> buffer;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      buffer.push_back(next(x, y));
    }
  }
  field = buffer;
}

void GameOfLife::render() {
  for (int y = 0; y < height; y++) {
    std::cout << "\n";
    for (int x = 0; x < width; x++) {
      if (is_alive(x, y)) {
        std::cout << "*";
      } else {
        std::cout << " ";
      }
    }
  }
  std::cout << "\n----------";
}

GameOfLifeLedMatrix::GameOfLifeLedMatrix(const int w, const int h, RGBMatrix *m,
                                         Color c = Color::random())
    : GameOfLife(w, h) {
  matrix = m;
  offscreen_canvas = matrix->CreateFrameCanvas();
  color = c;
}

void GameOfLifeLedMatrix::render() {
  offscreen_canvas->Clear();
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (is_alive(x, y)) {
        offscreen_canvas->SetPixel(x, y, color.r, color.g, color.b);
      }
    }
  }

  offscreen_canvas = matrix->SwapOnVSync(offscreen_canvas);
}

int main(int argc, char *argv[]) {
  RGBMatrix::Options options;
  RuntimeOptions rt_options;

  if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv, &options, &rt_options))
    return 1;

  RGBMatrix *matrix = RGBMatrix::CreateFromOptions(options, rt_options);
  if (matrix == NULL) return 1;

  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  while (!interrupt_received) {
    GameOfLifeLedMatrix game = GameOfLifeLedMatrix(options.cols, options.rows,
                                                   matrix, Color::random());
    for (int i = 0; i < 500; i++) {
      game.render();
      game.step();
    }
  }

  delete matrix;
  return 0;
}
