#ifndef SRC_VIEW_VIEW_H
#define SRC_VIEW_VIEW_H

#include <QFileDialog>
#include <QGraphicsScene>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>

#include "../controller/controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class view;
}
QT_END_NAMESPACE

namespace s21 {

class view : public QMainWindow {
  Q_OBJECT
 public:
  view(s21::controller &control, QWidget *parent = nullptr);
  view(const view &) = delete;
  view(view &&) = delete;
  view &operator=(const view &) = delete;
  view &operator=(view &&) = delete;
  ~view();

 private slots:
  void on_genFileFromMaze_clicked();
  void on_openFileFromMaze_clicked();
  void on_findingTheWay_clicked();

 private:
  void printMaze();
  void printTheWay();
  void printLineMaze(const short int &i, short int &pxH, short int &pxW,
                     const short int &stepH, const short int &stepW);

  Ui::view *ui_;
  s21::controller &controller_;
  QGraphicsScene *scene_;
  QPen *paintbrush_;
};
}  // namespace s21

#endif  // SRC_VIEW_VIEW_H
