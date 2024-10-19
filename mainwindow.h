#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
//#include <memory>
#include "geometry.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    std::vector<std::unique_ptr<Geometry>> displayFile;
    Point point = Point(10,10,0, 0,0,0);

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);

private slots:

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
