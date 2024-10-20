#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include "geometry.h"
#include "displayfile.h"
#include <math.h>

QImage canvas;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    canvas = QImage(ui->myFrame->width(), ui->myFrame->height(), QImage::Format_RGB888);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
    timer->start(16); // 16ms entre cada atualização (~60 FPS)

    // brincando com formas geometricas
    Point corner1(0,0,0, 255,255,255);
    Point corner2(0,canvas.height()-1,0, 255,255,255);
    Point corner3(canvas.width()-1,0,0, 255,255,255);
    Point corner4(canvas.width(),canvas.height(),0, 255,255,255);

    Polygon back1(corner2, corner1, corner3);
    Polygon back2(corner2, corner4, corner3);

    Point point1(canvas.width()/2-100, canvas.height()/2, 10, 255, 0, 0);
    Point point2(canvas.width()/2+200, canvas.height()/2-145, 0, 0, 255, 0);
    Point point3(canvas.width()/2+100, canvas.height()/2+145, 0, 0, 0, 255);

    Line linha1(Point(0,0,0, 255,0,0), Point(40,500,0, 0,255,0));
    Point point(120,80,20, 0,0,255);

    Polygon poly(point1,point2,point3);

    //adicionando no displayFile
    displayFile.insert(back1);
    displayFile.insert(back2);
    displayFile.insert(poly);
    displayFile.insert(linha1);
    displayFile.insert(point);


    ui -> myFrame -> setPixmap( QPixmap::fromImage(canvas));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){
    displayFile.drawAll(canvas);

    ui -> myFrame -> setPixmap( QPixmap::fromImage(canvas));

}
