#ifndef SRC_MODEL_MODEL_H
#define SRC_MODEL_MODEL_H

#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <random>
#include <string>
#include <vector>

namespace s21 {

class model {
 public:
  static model &getInstance();
  model(const model &) = delete;
  model(model &&) = delete;
  model &operator=(const model &) = delete;
  model &operator=(model &&) = delete;

  ~model();

  bool initMatrixForMaze(const std::string &patch);
  void genFileForMaze(const short height, const short width);
  bool findePath(const short heightStart, const short widthStart,
                 const short heightFinish, const short widthFinish);
  bool saveGenMaze(const std::string &patch);

  const std::vector<std::pair<short, short>> &getPath() const;
  std::pair<bool, bool> getCellMaze(const short &i, const short &j) const;
  short getHheight() const;
  short getWidth() const;

  friend std::ostream &operator<<(std::ostream &out, model &other) {
    for (short i = 0; i < (other.height_ * 2) + 1; out << "_", ++i)
      ;
    out << "\n";
    for (short i = 0; i < other.height_; ++i) {
      out << "|";
      for (short j = 0; j < other.width_; ++j) {
        out << (other.maze_[i][j].second ? "_" : " ");
        out << (other.maze_[i][j].first ? "|" : ".");
      }
      out << "\n";
    }
    out << "\n";
    return out;
  }

 private:
  model();

  struct nodeGraf {
    std::pair<short, short> indexCell;
    nodeGraf *back;
    nodeGraf() {
      back = nullptr;
      indexCell = {-1, -1};
    }
    nodeGraf(const nodeGraf &other) {
      this->back = other.back;
      this->indexCell = other.indexCell;
    }
    nodeGraf(const std::pair<short, short> indexCell) {
      this->back = nullptr;
      this->indexCell = indexCell;
    }
    nodeGraf operator=(nodeGraf &other) {
      this->back = other.back;
      this->indexCell = other.indexCell;
      return other;
    }
    nodeGraf &operator=(nodeGraf &&other) {
      this->back = std::move(other.back);
      this->indexCell = std::move(other.indexCell);
      return *this;
    }
  };

 public:
  using vec = std::vector<std::unique_ptr<nodeGraf>>;

 private:
  enum directions { Up, Down, Left, Right, count };

  void parsFile(std::ifstream &fileStream);
  bool checkValidAtFile(std::ifstream &fileStream);
  bool checkFirstLineInFile(std::string::iterator &it,
                            std::string::iterator &end);
  bool checkLineInFile(std::string::iterator &it, std::string::iterator &end);
  void skipNum(std::string::iterator &it, std::string::iterator &end);

  bool checkWall(const short height, const short width, directions where);
  void checkingAvailablePaths(vec &reachable, vec &explored);
  void addAvailablePaths(nodeGraf node, vec &reachable, vec &explored,
                         directions where);
  bool pathRepeatCheck(nodeGraf &node, vec &explored);
  void findeOldNode(vec &explored);
  void searchTheLastNodeInTheTraversedPath(vec &explored, directions where);
  void tieTheLastNode(short height, short width, vec &explored);
  void build_path(nodeGraf *node);

  void createFirstLine();
  void assignUniqueSet();
  void addingVerticalWalls(short row);
  void mergeSet(short index, short element);
  void addingHorizontalWalls(int row);
  int calculateUniqueSet(int element);
  void checkedHorizontalWalls(int row);
  int calculateHorizontalWalls(int element, int row);
  void preparatingNewLine(int row);
  void addingEndLine();
  void checkedEndLine();

  void initMazeForGen();
  std::vector<std::pair<bool, bool>> stringFileToLineVector(
      std::string::iterator &it, std::string::iterator &end);
  void stringFileAddToLineVector(std::string::iterator &it,
                                 std::string::iterator &end,
                                 std::vector<std::pair<bool, bool>> &mazeLine);

  s21::model::directions initWere(short i);
  inline bool isNum(const char &ch);
  inline bool isBinNum(const char &ch);
  inline bool isEndLineSim(const char &ch);
  void cleanMaze();
  void allerIndexMatrix();

  std::vector<std::vector<std::pair<bool, bool>>> maze_;
  std::vector<std::pair<short, short>> path_;
  std::vector<short> allerIndexLine_;
  short height_;
  short width_;
  short counter_;
};
}  // namespace s21
#endif  // SRC_MODEL_MODEL_H
