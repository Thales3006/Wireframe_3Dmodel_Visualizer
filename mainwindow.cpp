#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geometry.h"
#include "matrix4x4.h"

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

    //configurando o displayFile
    inicialSetupDisplayFile();

    for(std::string& name : displayFile.names)
        ui->objList->addItem(QString::fromStdString(name));

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

    static Matrix4x4 m = Matrix4x4::identity();
    m.rotate(-0.05, 0,1);

    Matrix4x4 ma = m;

    ma.transform(200,200,200);

    //displayFile.setMatrix("triangulo", ma);

    ma.transform(-100,20,-100);

    displayFile.setMatrix("linha", ma);

    ma.transform(200,120,-100);

    displayFile.setMatrix("ponto_direita", ma);


    canvas.fill(Qt::white);
    displayFile.drawAll(canvas);
    pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(canvas));

    scene->addItem(pixmapItem);
}

void MainWindow::inicialSetupDisplayFile(){
    Point point1(canvas.width()/2-100, canvas.height()/2, 10, 255, 0, 0);
    Point point2(canvas.width()/2+200, canvas.height()/2-145, 0, 0, 255, 0);
    Point point3(canvas.width()/2+100, canvas.height()/2+145, 0, 0, 0, 255);

    Line linha1(Point(-100,-100,0, 255,0,0), Point(40,300,0, 0,255,0));
    Point point11(0,20,20, 0,0,255);
    Point point12(20,-20,20, 0,0,255);
    Point point13(-20,-20,20, 0,0,255);

    Polygon poly(point1,point2,point3);

    displayFile.insert("triangulo", poly);
    displayFile.insert("linha", linha1);
    displayFile.insert("ponto_cima", point11);
    displayFile.insert("ponto_esquerda", point12);
    displayFile.insert("ponto_direita", point13);

    displayFile.drawAll(canvas);
}

void MainWindow::on_applyTransform_clicked()
{
    std::string objName = ui->objList->currentText().toStdString();

    Matrix4x4 matrix = Matrix4x4::identity();

    float angle = ((float)ui->angleDial->value()/100)*3.14*2;
    float rotX = ui->rotateCoordX->text().toFloat();
    float rotY = ui->rotateCoordY->text().toFloat();

    //float escalar = ui->escaleBar->value();

    //float newX = ui->translateCoordX->text().toFloat();
    //float newY = ui->translateCoordX->text().toFloat();

    //ROTAÇÃO
    matrix.transform(-rotX,-rotY,0);
    matrix.rotate(angle, 0, 1);
    matrix.transform(rotX,rotY,0);

    //ESCALA
    //QPoint center = displayFile.getMean(objName);

    //matrix.transform(-center.x(),-center.y(),0);
    //matrix*escalar;
    //matrix.transform(center.x(),center.y(),0);


    //TRANSLAÇÃO
    //matrix.transform(-center.x(),-center.y(),0);
    //matrix.transform(newX,newY,0);

    //atribui nova matriz ao objeto
    displayFile.setMatrix(objName, matrix);

    std::cout << angle << std::endl;
}

