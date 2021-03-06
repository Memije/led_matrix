#include <vector>

#include "color.h"
#include "led-matrix.h"

using rgb_matrix::FrameCanvas;
using rgb_matrix::RGBMatrix;

class GameOfLife {
 public:
  GameOfLife(const int, const int);
  void set(const int, const int, const bool);
  bool is_alive(int, int);
  bool next(int, int);
  int coord_index(const int, const int);
  void step();
  void render();

 protected:
  int width;
  int height;
  std::vector<bool> field;
};

class GameOfLifeLedMatrix : public GameOfLife {
 public:
  GameOfLifeLedMatrix(const int, const int, RGBMatrix *, Color);
  void render();

 private:
  RGBMatrix *matrix;
  FrameCanvas *offscreen_canvas;
  Color color;
};
