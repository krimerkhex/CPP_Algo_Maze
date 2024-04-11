#include "view.h"

#include "./ui_view.h"

s21::view::view(s21::controller &control, QWidget *parent)
    : QMainWindow(parent),
      ui_(new Ui::view),
      controller_(control),
      scene_(new QGraphicsScene()),
      paintbrush_(new QPen()) {
  ui_->setupUi(this);
  ui_->windowPrintMaze->setScene(scene_);
  ui_->windowPrintMaze->centerOn(0, 0);
  scene_->setSceneRect(0, 0, 499, 499);
  ui_->windowPrintMaze->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui_->windowPrintMaze->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  paintbrush_->setWidth(2);
}

s21::view::~view() {
  delete scene_;
  delete paintbrush_;
  delete ui_;
}

void s21::view::on_genFileFromMaze_clicked() {
  if (controller_.genFileForMaze(ui_->spinBoxForHeight->text().toShort(),
                                 ui_->spinBoxForWidth->text().toShort())) {
    std::string textToHeight =
        "𝑯𝒆𝒊𝒈𝒉𝒕: " + std::to_string(controller_.getHeight());
    std::string textToWidth =
        "𝑾𝒊𝒅𝒕𝒉: " + std::to_string(controller_.getWidth());
    ui_->labelHeightAtModel->setText(QString::fromStdString(textToHeight));
    ui_->labelWidthAtModel->setText(QString::fromStdString(textToWidth));
    printMaze();
    std::string patch =
        QFileDialog::getSaveFileName(this, tr("Save maze to file"), ".",
                                     tr("Text files (*.txt)"))
            .toStdString();
    bool error = false;
    if (patch.size()) {
      error = controller_.saveMazeToFile(patch);
    }
    if (error) {
      QMessageBox::warning(this, "Warning",
                           "Unexpected error when "
                           "saving file.");
    }
  } else {
    QMessageBox::warning(this, "Warning",
                         "Incorrect data for "
                         "maze generation.");
  }
}

void s21::view::on_openFileFromMaze_clicked() {
  std::string patch =
      QFileDialog::getOpenFileName(this, "File selection", "/Users", "*.txt")
          .toStdString();
  if (controller_.loadFileForMaze(patch) || !patch.size()) {
    QMessageBox::warning(this, "Warning",
                         "Failed to read the file, "
                         "please check the correctness "
                         "of the data in the file.");
  } else {
    std::string textToHeight =
        "𝑯𝒆𝒊𝒈𝒉𝒕: " + std::to_string(controller_.getHeight());
    std::string textToWidth =
        "𝑾𝒊𝒅𝒕𝒉: " + std::to_string(controller_.getWidth());
    ui_->labelHeightAtModel->setText(QString::fromStdString(textToHeight));
    ui_->labelWidthAtModel->setText(QString::fromStdString(textToWidth));
    printMaze();
  }
}

void s21::view::on_findingTheWay_clicked() {
  if (!controller_.getHeight() || !controller_.getWidth()) {
    QMessageBox::warning(this, "Warning",
                         "It is not possible to find "
                         "a solution to the path until "
                         "the maze is created.");
  } else {
    if (ui_->spinBoxForHeightStart->value() ==
            ui_->spinBoxForHeightFinish->value() &&
        ui_->spinBoxForWidthStart->value() ==
            ui_->spinBoxForWidthFinish->value()) {
      QMessageBox::warning(this, "Warning",
                           "The starting point and the "
                           "finishing point cannot be "
                           "the same point.");
    } else {
      if (controller_.findePath(ui_->spinBoxForHeightStart->value(),
                                ui_->spinBoxForWidthStart->value(),
                                ui_->spinBoxForHeightFinish->value(),
                                ui_->spinBoxForWidthFinish->value())) {
        QMessageBox::warning(this, "Warning",
                             "The path to the point "
                             "cannot be found.");
      } else {
        printTheWay();
      }
    }
  }
}

void s21::view::printTheWay() {
  printMaze();
  paintbrush_->setColor(QColor("#FF0000"));
  const short stepW = 500 / controller_.getWidth();
  const short stepH = 500 / controller_.getHeight();
  short pxHNext, pxWNext, pxW, pxH;
  std::vector<std::pair<short, short>> way = controller_.getTheWay();
  auto it = way.begin();
  pxH = (it->first + 1) * stepH - (stepH / 2);
  pxW = (it->second + 1) * stepW - (stepW / 2);
  while (it != way.end()) {
    pxHNext = (it->first + 1) * stepH - (stepH / 2);
    pxWNext = (it->second + 1) * stepW - (stepW / 2);
    scene_->addLine(pxW, pxH, pxWNext, pxHNext, *paintbrush_);
    pxH = pxHNext;
    pxW = pxWNext;
    it++;
  }
}

void s21::view::printMaze() {
  scene_->clear();
  paintbrush_->setColor(QColor("#000000"));
  short pxH = 0, pxW = 0;
  const short stepW = 500 / controller_.getWidth();
  const short stepH = 500 / controller_.getHeight();
  for (short i = 0; i < controller_.getHeight(); i++) {
    printLineMaze(i, pxH, pxW, stepH, stepW);
    pxW = 0;
    pxH += stepH;
  }
  scene_->addRect(0, 0, 497, 497, *paintbrush_);
}

void s21::view::printLineMaze(const short &i, short &pxH, short &pxW,
                              const short &stepH, const short &stepW) {
  for (int j = 0; j < controller_.getWidth(); j++) {
    std::pair<bool, bool> cell = controller_.getCellMaze(i, j);
    if (cell.first) {
      scene_->addLine(pxW + stepW, pxH, pxW + stepW, pxH + stepH, *paintbrush_);
    }
    if (cell.second) {
      scene_->addLine(pxW, pxH + stepH, pxW + stepW, pxH + stepH, *paintbrush_);
    }
    pxW += stepW;
  }
}
