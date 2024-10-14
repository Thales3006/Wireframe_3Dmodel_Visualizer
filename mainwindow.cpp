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

    Point point1(canvas.width()/2-100, canvas.height()/2, 10, 255, 0, 0);
    Point point2(canvas.width()/2+200, canvas.height()/2-145, 0, 0, 255, 0);
    Point point3(canvas.width()/2+100, canvas.height()/2+145, 0, 0, 0, 255);


    Line linha1(point1, point3);
    Line linha2(point1, point2);
    Line linha3(point3, point2);
    linha1.draw(canvas);
    linha2.draw(canvas);
    linha3.draw(canvas);
    // fim da brincadeira

    ui -> myFrame -> setPixmap( QPixmap::fromImage(canvas));
}

MainWindow::~MainWindow()
{
    delete ui;
}
