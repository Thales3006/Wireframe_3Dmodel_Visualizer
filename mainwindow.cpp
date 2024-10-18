#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geometry.h"
#include <math.h>

QImage canvas;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    canvas = QImage(ui->myFrame->width(), ui->myFrame->height(), QImage::Format_RGB888);

    // brincando com pontos
    Point corner1(0,0,0, 255,255,255);
    Point corner2(0,canvas.height(),0, 255,255,255);
    Point corner3(canvas.width(),0,0, 255,255,255);
    Point corner4(canvas.width(),canvas.height(),0, 255,255,255);

    Polygon back1(corner2, corner1, corner3);
    Polygon back2(corner2, corner4, corner3);
    // back1.draw(canvas);
    // back2.draw(canvas);

    Point point1(canvas.width()/2-100, canvas.height()/2, 10, 255, 0, 0);
    Point point2(canvas.width()/2+200, canvas.height()/2-145, 0, 0, 255, 0);
    Point point3(canvas.width()/2+100, canvas.height()/2+145, 0, 0, 0, 255);

    /*
    Line linha1(point1, point3);
    Line linha2(point1, point2);
    Line linha3(point3, point2);
    linha1.draw(canvas);
    linha2.draw(canvas);
    linha3.draw(canvas);
    */

    Polygon poly(point1,point2,point3);
    // poly.draw(canvas);

    displayFile.push_back(back1);
    displayFile.push_back(back2);
    displayFile.push_back(poly);
    // fim da brincadeira

    ui -> myFrame -> setPixmap( QPixmap::fromImage(canvas));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawPoint(QPoint(point.x,point.y));

    for(int i=0; i<displayFile.size();i++){
        if(typeof(displayFile[i])==Polygon){
            Polygon poly = displayFile[i];
            poly.draw(canvas);
        }
    }

}
