#include <QApplication>

#include "view/view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::model &Model = s21::model::getInstance();
  s21::controller &Controller = s21::controller::getInstance(Model);
  s21::view View(Controller);
  View.show();
  return a.exec();
}
