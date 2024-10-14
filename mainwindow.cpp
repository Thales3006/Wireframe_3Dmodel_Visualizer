#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geometry.h"

QImage canvas;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    canvas = QImage(ui->myFrame->width(), ui->myFrame->height(), QImage::Format_RGB888);

    // brincando com pontos
    Point point(0, 0, 0, 255, 255, 255);
    for(int i=0; i < canvas.height(); i++){
        for(int j=0; j < canvas.width(); j++){
            point.x = (float)j;
            point.draw(canvas);
        }
        point.y = (float)i;
    }

    Point point1(canvas.width()/2, canvas.height()/2, 10, 255, 0, 0);
    Point point2(canvas.width()/2, canvas.height()/2+1, 10, 255, 255, 0);
    Point point3(canvas.width()/2+10, canvas.height()/2+45, 0, 20, 100, 244);
    point1.draw(canvas);
    point2.draw(canvas);
    point3.draw(canvas);
    // fim da brincadeira

    ui -> myFrame -> setPixmap( QPixmap::fromImage(canvas));
}

MainWindow::~MainWindow()
{
    delete ui;
}
