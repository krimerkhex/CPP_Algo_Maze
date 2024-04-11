#include "model.h"

s21::model &s21::model::getInstance() {
  static model Model;
  return Model;
}

s21::model::model() : height_(0), width_(0) {}

s21::model::~model() {}

bool s21::model::initMatrixForMaze(const std::string &patch) {
  std::ifstream fileStream(patch);
  bool error = false;
  if (maze_.size()) {
    cleanMaze();
  }
  if (!fileStream.is_open()) {
    error = true;
  } else {
    error = checkValidAtFile(fileStream);
    if (!error ||
        (height_ >= 1 && height_ <= 50 && width_ >= 1 && width_ <= 50)) {
      fileStream.clear();
      fileStream.seekg(0, std::ios_base::beg);
      parsFile(fileStream);
    } else {
      error = true;
      height_ = 0;
      width_ = 0;
    }
  }
  fileStream.close();
  return error;
}

bool s21::model::checkValidAtFile(std::ifstream &fileStream) {
  bool error = false;
  std::string stringFromFile = "";
  std::getline(fileStream, stringFromFile);
  std::string::iterator it = stringFromFile.begin(), end = stringFromFile.end();
  if (it == end || !isNum(*it)) {
    error = true;
  } else {
    error = checkFirstLineInFile(it, end);
  }
  if (!error) {
    short heightCount = 0, matrixCount = 0;
    while (std::getline(fileStream, stringFromFile) && !error &&
           matrixCount != 2) {
      it = stringFromFile.begin();
      end = stringFromFile.end();
      if (it == end || isEndLineSim(*it)) {
        if (heightCount == height_) {
          heightCount = 0;
          matrixCount += 1;
        } else {
          error = true;
        }
      } else {
        error = checkLineInFile(it, end);
        heightCount++;
      }
    }
  }
  return error;
}

bool s21::model::checkFirstLineInFile(std::string::iterator &it,
                                      std::string::iterator &end) {
  bool error = false;
  short countNum = 0;
  while (it != end && !error) {
    if (!isNum(*it) && *it != ' ' && !isEndLineSim(*it)) {
      error = true;
    } else if (isNum(*it)) {
      if (countNum <= 1) {
        if (!countNum) {
          height_ = std::stoi(&(*it));
        } else if (countNum == 1) {
          width_ = std::stoi(&(*it));
        }
        skipNum(it, end);
        countNum++;
      } else {
        error = true;
        height_ = 0;
        width_ = 0;
      }
    } else {
      it++;
    }
  }
  return error;
}

bool s21::model::checkLineInFile(std::string::iterator &it,
                                 std::string::iterator &end) {
  bool error = false;
  short widthCount = 0;
  if (it == end && !isBinNum(*it) && isEndLineSim(*it)) {
    error = true;
  }
  while (it != end && !error) {
    if (isBinNum(*it)) {
      widthCount += 1;
      it++;
      if (it != end) {
        *it != ' ' ? error = true : 0;
      }
    } else if (*it == ' ' || isEndLineSim(*it)) {
      it++;
    } else {
      error = true;
    }
  }
  if (widthCount != width_) {
    error = true;
  }
  return error;
}

void s21::model::skipNum(std::string::iterator &it,
                         std::string::iterator &end) {
  while (it != end && isNum(*it)) {
    it++;
  }
}

void s21::model::parsFile(std::ifstream &fileStream) {
  std::string stringFromFile = "";
  std::getline(fileStream, stringFromFile);
  std::string::iterator it = stringFromFile.begin(), end = stringFromFile.end();
  bool secondMatrix = false;
  short countHeight = 0, i = 0;
  while (std::getline(fileStream, stringFromFile)) {
    it = stringFromFile.begin();
    end = stringFromFile.end();
    if (it != end && !isEndLineSim(*it)) {
      if (!secondMatrix) {
        maze_.push_back(stringFileToLineVector(it, end));
      } else {
        stringFileAddToLineVector(it, end, maze_.at(i));
        i++;
      }
    }
    if (countHeight < height_) {
      countHeight++;
    } else if (countHeight == height_) {
      countHeight++;
      secondMatrix = true;
    }
  }
}

std::vector<std::pair<bool, bool>> s21::model::stringFileToLineVector(
    std::string::iterator &it, std::string::iterator &end) {
  std::vector<std::pair<bool, bool>> bufLine;
  while (it != end) {
    if (*it == ' ') {
      it++;
    } else {
      bufLine.push_back(std::make_pair(std::stoi(&(*it)), 0));
      it++;
    }
  }
  return bufLine;
}

void s21::model::stringFileAddToLineVector(
    std::string::iterator &it, std::string::iterator &end,
    std::vector<std::pair<bool, bool>> &mazeLine) {
  short i = 0;
  while (it != end) {
    if (*it == ' ') {
      it++;
    } else {
      mazeLine.at(i).second = std::stoi(&(*it));
      i++;
      it++;
    }
  };
}

std::pair<bool, bool> s21::model::getCellMaze(const short &i,
                                              const short &j) const {
  return maze_.at(i).at(j);
}

short s21::model::getHheight() const { return height_; }

short s21::model::getWidth() const { return width_; }

inline bool s21::model::isEndLineSim(const char &ch) {
  return ch == '\r' || ch == '\n';
}

inline bool s21::model::isNum(const char &ch) { return ch >= '0' && ch <= '9'; }

inline bool s21::model::isBinNum(const char &ch) {
  return ch == '0' || ch == '1';
}

void s21::model::cleanMaze() { maze_.resize(0); }

void s21::model::allerIndexMatrix() { allerIndexLine_.resize(0); }

void s21::model::genFileForMaze(const short height, const short width) {
  cleanMaze();
  allerIndexMatrix();
  counter_ = 1;
  height_ = height;
  width_ = width;
  initMazeForGen();
  createFirstLine();
  for (int j = 0; j < height_ - 1; j++) {
    assignUniqueSet();
    addingVerticalWalls(j);
    addingHorizontalWalls(j);
    checkedHorizontalWalls(j);
    preparatingNewLine(j);
  }
  addingEndLine();
}

void s21::model::initMazeForGen() {
  std::vector<std::pair<bool, bool>> initValue;
  for (int i = 0; i < width_; i++) {
    initValue.push_back(std::make_pair(false, false));
  }
  for (int i = 0; i < height_; i++) {
    maze_.push_back(initValue);
  }
}

void s21::model::addingEndLine() {
  assignUniqueSet();
  addingVerticalWalls(height_ - 1);
  checkedEndLine();
}

void s21::model::checkedEndLine() {
  for (int i = 0; i < width_ - 1; i++) {
    if (allerIndexLine_[i] != allerIndexLine_[i + 1]) {
      maze_.at(height_ - 1).at(i).first = false;
      mergeSet(i, allerIndexLine_[i]);
    }
    maze_.at(height_ - 1).at(i).second = true;
  }
  maze_.at(height_ - 1).at(width_ - 1).second = true;
}

void s21::model::preparatingNewLine(int row) {
  for (int i = 0; i < width_; i++) {
    if (maze_.at(row).at(i).second == true) {
      allerIndexLine_[i] = 0;
    }
  }
}

void s21::model::addingHorizontalWalls(int row) {
  for (int i = 0; i < width_; i++) {
    bool choise = arc4random() % 2;
    if (calculateUniqueSet(allerIndexLine_[i]) != 1 && choise == true) {
      maze_.at(row).at(i).second = true;
    }
  }
}

int s21::model::calculateUniqueSet(int element) {
  int countUniqSet = 0;
  for (int i = 0; i < width_; i++) {
    if (allerIndexLine_[i] == element) {
      countUniqSet++;
    }
  }
  return countUniqSet;
}

void s21::model::checkedHorizontalWalls(int row) {
  for (int i = 0; i < width_; i++) {
    if (calculateHorizontalWalls(allerIndexLine_[i], row) == 0) {
      maze_.at(row).at(i).second = false;
    }
  }
}

int s21::model::calculateHorizontalWalls(int element, int row) {
  int countHorizontalWalls = 0;
  for (int i = 0; i < width_; i++) {
    if (allerIndexLine_[i] == element && maze_.at(row).at(i).second == false) {
      countHorizontalWalls++;
    }
  }
  return countHorizontalWalls;
}

void s21::model::addingVerticalWalls(short row) {
  for (int i = 0; i < width_ - 1; i++) {
    bool choise = arc4random() % 2;
    if (choise == true || allerIndexLine_[i] == allerIndexLine_[i + 1]) {
      maze_.at(row).at(i).first = true;
    } else {
      mergeSet(i, allerIndexLine_[i]);
    }
  }
  maze_.at(row).at(width_ - 1).first = true;
}

void s21::model::mergeSet(short index, short element) {
  int mutableSet = allerIndexLine_[index + 1];
  for (int j = 0; j < width_; j++) {
    if (allerIndexLine_[j] == mutableSet) {
      allerIndexLine_[j] = element;
    }
  }
}

void s21::model::assignUniqueSet() {
  for (int i = 0; i < width_; i++) {
    if (allerIndexLine_[i] == 0) {
      allerIndexLine_[i] = counter_;
      counter_++;
    }
  }
}

void s21::model::createFirstLine() {
  for (int i = 0; i < width_; i++) {
    allerIndexLine_.push_back(0);
  }
}

bool s21::model::checkWall(const short height, const short width,
                           directions where) {
  bool res = false;
  switch (where) {
    case Up:
      if (height != 0) {
        if (!maze_.at(height - 1).at(width).second) {
          res = true;
        }
      }
      break;
    case Down:
      if (!maze_.at(height).at(width).second) {
        res = true;
      }
      break;
    case Left:
      if (width != 0) {
        if (!maze_.at(height).at(width - 1).first) {
          res = true;
        }
      }
      break;
    case Right:
      if (!maze_.at(height).at(width).first) {
        res = true;
      }
      break;
    case count:
      break;
  }
  return res;
}

bool s21::model::pathRepeatCheck(nodeGraf &node, vec &explored) {
  bool res = false;
  for (auto &it : explored) {
    if ((*it).indexCell == node.indexCell) {
      res = true;
      break;
    }
  }
  return res;
}

void s21::model::addAvailablePaths(nodeGraf node, vec &reachable, vec &explored,
                                   directions where) {
  switch (where) {
    case Up:
      node.indexCell.first -= 1;
      if (!pathRepeatCheck(node, explored)) {
        reachable.insert(reachable.begin(),
                         std::make_unique<nodeGraf>(node.indexCell));
      }
      break;
    case Down:
      node.indexCell.first += 1;
      if (!pathRepeatCheck(node, explored)) {
        reachable.insert(reachable.begin(),
                         std::make_unique<nodeGraf>(node.indexCell));
      }
      break;
    case Left:
      node.indexCell.second -= 1;
      if (!pathRepeatCheck(node, explored)) {
        reachable.insert(reachable.begin(),
                         std::make_unique<nodeGraf>(node.indexCell));
      }
      break;
    case Right:
      node.indexCell.second += 1;
      if (!pathRepeatCheck(node, explored)) {
        reachable.insert(reachable.begin(),
                         std::make_unique<nodeGraf>(node.indexCell));
      }
      break;
    case count:
      break;
  }
}

void s21::model::checkingAvailablePaths(vec &reachable, vec &explored) {
  directions where = Up;
  short i = 0;
  while (i != 4) {
    if (checkWall((*explored.back()).indexCell.first,
                  (*explored.back()).indexCell.second, where)) {
      addAvailablePaths(*explored.back(), reachable, explored, where);
    }
    i++;
    where = initWere(i);
  }
}

void s21::model::tieTheLastNode(short height, short width, vec &explored) {
  for (size_t i = 0; i < explored.size(); i++) {
    if ((*explored[i]).indexCell.first == height &&
        (*explored[i]).indexCell.second == width) {
      (*explored.back()).back = &(*explored[i]);
      break;
    }
  }
}

void s21::model::searchTheLastNodeInTheTraversedPath(vec &explored,
                                                     directions where) {
  switch (where) {
    case Up:
      tieTheLastNode((*explored.back()).indexCell.first - 1,
                     (*explored.back()).indexCell.second, explored);
      break;
    case Down:
      tieTheLastNode((*explored.back()).indexCell.first + 1,
                     (*explored.back()).indexCell.second, explored);
      break;
    case Left:
      tieTheLastNode((*explored.back()).indexCell.first,
                     (*explored.back()).indexCell.second - 1, explored);
      break;
    case Right:
      tieTheLastNode((*explored.back()).indexCell.first,
                     (*explored.back()).indexCell.second + 1, explored);
      break;
    case count:
      break;
  }
}

s21::model::directions s21::model::initWere(short i) {
  directions where;
  if (i == 0) {
    where = Up;
  } else if (i == 1) {
    where = Down;
  } else if (i == 2) {
    where = Left;
  } else {
    where = Right;
  }
  return where;
}

void s21::model::findeOldNode(vec &explored) {
  directions where = Up;
  short i = 0;
  while (i != 4) {
    if (checkWall((*explored.back()).indexCell.first,
                  (*explored.back()).indexCell.second, where)) {
      searchTheLastNodeInTheTraversedPath(explored, where);
    }
    i++;
    where = initWere(i);
  }
}

void s21::model::build_path(nodeGraf *node) {
  while (node != nullptr) {
    path_.emplace_back(node->indexCell);
    node = node->back;
  }
}

bool s21::model::findePath(const short heightStart, const short widthStart,
                           const short heightFinish, const short widthFinish) {
  bool error = true;
  path_.resize(0);
  vec reachable;
  vec explored;
  reachable.push_back(
      std::make_unique<nodeGraf>(nodeGraf({heightStart, widthStart})));
  while (reachable.size() && error) {
    explored.emplace_back(std::move(reachable[0]));
    findeOldNode(explored);
    reachable.erase(reachable.begin());
    if ((*explored.back()).indexCell.first == heightFinish &&
        (*explored.back()).indexCell.second == widthFinish) {
      error = false;
      build_path(&(*explored.back()));
    } else {
      checkingAvailablePaths(reachable, explored);
    }
  }
  return error;
}

const std::vector<std::pair<short, short>> &s21::model::getPath() const {
  return path_;
}

bool s21::model::saveGenMaze(const std::string &patch) {
  std::ofstream MyFile(patch);
  if (!MyFile.is_open()) {
    return true;
  } else {
    MyFile << height_ << ' ' << width_ << '\n';
    for (auto it : maze_) {
      for (size_t i = 0; i < it.size(); i++) {
        MyFile << it[i].first;
        if (i != it.size() - 1) {
          MyFile << ' ';
        }
      }
      MyFile << '\n';
    }
    MyFile << '\n';
    for (auto it : maze_) {
      for (size_t i = 0; i < it.size(); i++) {
        MyFile << it[i].second;
        if (i != it.size() - 1) {
          MyFile << ' ';
        }
      }
      MyFile << '\n';
    }
  }
  MyFile.close();
  return false;
}
