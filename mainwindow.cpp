#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geometry.h"
#include "matrix.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    canvas = QImage(ui->visualizador->width(), ui->visualizador->height(), QImage::Format_RGB888);
    canvas.fill(Qt::white);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
    timer->start(16); // 16ms entre cada atualização (~60 FPS)

    // Criando a cena e a view
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, ui->visualizador);


    Point point1(canvas.width()/2-100, canvas.height()/2, 10, 255, 0, 0);
    Point point2(canvas.width()/2+200, canvas.height()/2-145, 0, 0, 255, 0);
    Point point3(canvas.width()/2+100, canvas.height()/2+145, 0, 0, 0, 255);

    Line linha1(Point(0,0,0, 255,0,0), Point(40,500,0, 0,255,0));
    Point point11(120,280,20, 0,0,255);
    Point point12(115,270,20, 0,0,255);
    Point point13(125,270,20, 0,0,255);

    Polygon poly(point1,point2,point3);

    displayFile.insert("triangulo", poly);
    displayFile.insert("linha", linha1);
    displayFile.insert("ponto_cima", point11);
    displayFile.insert("ponto_esquerda", point12);
    displayFile.insert("ponto_direita", point13);

    displayFile.drawAll(canvas);

    pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(canvas));
    scene->addItem(pixmapItem);  // Adicionando o item à cena
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    scene->removeItem(pixmapItem);

    canvas.fill(Qt::white);
    displayFile.drawAll(canvas);
    pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(canvas));

    scene->addItem(pixmapItem);
}
