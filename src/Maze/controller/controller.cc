#include "controller.h"

s21::controller &s21::controller::getInstance(s21::model &Model) {
  static controller Controller(Model);
  return Controller;
}

s21::controller::controller(s21::model &Model) : model_(Model) {}

s21::controller::~controller() {}

bool s21::controller::findePath(const short heightStart, const short widthStart,
                                const short heightFinish,
                                const short widthFinish) {
  return model_.findePath(heightStart - 1, widthStart - 1, heightFinish - 1,
                          widthFinish - 1);
}

bool s21::controller::genFileForMaze(const short height,
                                     const short width) noexcept {
  if (height < 1 || height > 50 || width < 1 || width > 50) {
    return false;
  }
  model_.genFileForMaze(height, width);
  return true;
}

bool s21::controller::saveMazeToFile(const std::string &patch) const {
  return model_.saveGenMaze(patch);
}

bool s21::controller::loadFileForMaze(const std::string &patch) noexcept {
  return model_.initMatrixForMaze(patch);
}

std::pair<bool, bool> s21::controller::getCellMaze(const short &i,
                                                   const short &j) const {
  return model_.getCellMaze(i, j);
}

const std::vector<std::pair<short, short>> &s21::controller::getTheWay() const {
  return model_.getPath();
}

short s21::controller::getHeight() const { return model_.getHheight(); }

short s21::controller::getWidth() const { return model_.getWidth(); }
