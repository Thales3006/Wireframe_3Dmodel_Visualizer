#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geometry.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //canvas
    canvas = QImage(ui->visualizador->width(), ui->visualizador->height(), QImage::Format_RGB888);
    canvas.fill(Qt::white);

    //atualiza a tela
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
    timer->start(16); // 16ms entre cada atualização (~60 FPS)

    // Criando a cena e a view
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, ui->visualizador);

//*************

    // ADIÇÃO DE ELEMENTOS PARA MOSTRAR NA TELA

    //pontos são declarados com coords x,y,z e r,g,b
    Point point1(canvas.width()/2-100, canvas.height()/2, 10, 255, 0, 0);
    Point point2(canvas.width()/2+200, canvas.height()/2-145, 0, 0, 255, 0);
    Point point3(canvas.width()/2+100, canvas.height()/2+145, 0, 0, 0, 255);

    //criando um poligono a partir de pontos ja existentes
    Polygon poly(point1,point2,point3);

    //criando linhas
    Line linha1(Point(20,20,0, 255,0,0), Point(60,300,0, 0,255,0));
    Line linha2(Point(10,200,0, 0,0,0), Point(100,150,0, 0,255,255));

    //pontos independentes na tela
    Point point11(120,280,20, 0,0,255);
    Point point12(115,270,20, 0,0,255);
    Point point13(125,270,20, 0,0,255);
    Point point14(123,265,20, 0,0,255);
    Point point15(121,270,20, 0,0,255);
    Point point16(122,272,20, 0,0,255);

    //adicionando ao display file
    displayFile.insert("triangulo", poly);
    displayFile.insert("linha_horizontal", linha1);
    displayFile.insert("linha_vertical", linha2);
    displayFile.insert("ponto_cima", point11);
    displayFile.insert("ponto_esquerda", point12);
    displayFile.insert("ponto_direita", point13);
    displayFile.insert("pontinho", point14);
    displayFile.insert("ponto_mais_um", point15);
    displayFile.insert("outro_ponto", point16);

    //Configurações de display do QtCreator
    pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(canvas));
    scene->addItem(pixmapItem);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){
    //Função desenha os objetos do display file a cada 16ms
    Q_UNUSED(event);
    scene->removeItem(pixmapItem);

    canvas.fill(Qt::white);
    displayFile.drawAll(canvas);
    pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(canvas));

    scene->addItem(pixmapItem);
}
