#ifndef SRC_CONTROLLER_CONTROLLER_H
#define SRC_CONTROLLER_CONTROLLER_H

#include "../model/model.h"

namespace s21 {

class controller {
 public:
  static controller &getInstance(s21::model &Model);
  controller(const controller &) = delete;
  controller(controller &&) = delete;
  controller &operator=(const controller &) = delete;
  controller &operator=(controller &&) = delete;
  ~controller();

  bool findePath(const short heightStart, const short widthStart,
                 const short heightFinish, const short widthFinish);
  bool genFileForMaze(const short height, const short width) noexcept;
  bool loadFileForMaze(const std::string &patch) noexcept;
  const std::vector<std::pair<short, short>> &getTheWay() const;
  std::pair<bool, bool> getCellMaze(const short &i, const short &j) const;
  bool saveMazeToFile(const std::string &patch) const;
  short getHeight() const;
  short getWidth() const;

 private:
  controller(s21::model &Model);
  s21::model &model_;
};
}  // namespace s21

#endif  // SRC_CONTROLLER_CONTROLLER_H
