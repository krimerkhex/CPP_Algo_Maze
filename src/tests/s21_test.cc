#include <gtest/gtest.h>

#include "../Maze/model/model.h"

class MazeTest : public testing::Test {
 protected:
  s21::model &Model = s21::model::getInstance();
  void TearDown() {}
};

TEST_F(MazeTest, FileNotHavePath) {
  std::string path = "/Users/zenaluth/A1_Maze-1/src/tests/mazeAga.txt";

  Model.initMatrixForMaze(path);
  int test1[10][10] = {
      {1, 0, 1, 0, 0, 0, 0, 1, 0, 1}, {0, 1, 1, 1, 0, 0, 0, 1, 1, 1},
      {1, 0, 1, 0, 0, 1, 1, 1, 1, 1}, {1, 0, 0, 1, 0, 0, 1, 0, 1, 1},
      {0, 0, 1, 0, 1, 0, 1, 0, 1, 1}, {1, 0, 0, 0, 0, 1, 1, 0, 1, 1},
      {0, 0, 0, 1, 1, 0, 0, 1, 0, 1}, {0, 0, 0, 0, 1, 0, 1, 1, 0, 1},
      {1, 0, 0, 0, 1, 1, 1, 0, 0, 1}, {0, 1, 0, 1, 0, 1, 0, 0, 0, 1}};
  for (int i = 0; i < Model.getHheight(); i++) {
    for (int j = 0; j < Model.getWidth(); j++) {
      EXPECT_EQ(Model.getCellMaze(i, j).first, test1[i][j]);
    }
  }
  int test2[10][10] = {{1, 1, 0, 0, 0, 1, 1, 1, 0, 0},
                       {
                           1,
                           1,
                           0,
                           0,
                           1,
                           1,
                           1,
                           0,
                           0,
                           0,
                       },
                       {
                           0,
                           0,
                           1,
                           1,
                           1,
                           0,
                           0,
                           0,
                           0,
                           0,
                       },
                       {
                           0,
                           1,
                           1,
                           0,
                           0,
                           1,
                           0,
                           0,
                           1,
                           0,
                       },
                       {
                           1,
                           0,
                           1,
                           1,
                           1,
                           0,
                           1,
                           1,
                           1,
                           0,
                       },
                       {
                           0,
                           1,
                           1,
                           1,
                           0,
                           1,
                           0,
                           0,
                           0,
                           0,
                       },
                       {
                           1,
                           1,
                           0,
                           1,
                           0,
                           0,
                           1,
                           0,
                           1,
                           1,
                       },
                       {
                           0,
                           1,
                           1,
                           1,
                           1,
                           0,
                           0,
                           0,
                           1,
                           0,
                       },
                       {
                           1,
                           0,
                           1,
                           0,
                           0,
                           0,
                           0,
                           1,
                           1,
                           1,
                       },
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
  for (int i = 0; i < Model.getHheight(); i++) {
    for (int j = 0; j < Model.getWidth(); j++) {
      EXPECT_EQ(Model.getCellMaze(i, j).second, test2[i][j]);
    }
  }
  EXPECT_EQ(Model.findePath(0, 0, 9, 9), true);
}

TEST_F(MazeTest, GenAndFindePath) {
  Model.genFileForMaze(10, 10);
  EXPECT_EQ(Model.findePath(0, 0, 9, 9), false);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}