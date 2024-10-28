#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geometry.h"
#include "matrix4x4.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //configurações iniciais
    canvas = QImage(ui->visualizador->width(), ui->visualizador->height(), QImage::Format_RGB888);
    canvas.fill(Qt::white);

    ui->scaleBar->setValue(25);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
    timer->start(16); // 16ms entre cada atualização (~60 FPS)

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

//FUNÇÃO DESENHO
// redesenha a tela a cada 16ms
void MainWindow::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    scene->removeItem(pixmapItem);

    canvas.fill(Qt::white);
    displayFile.drawAll(canvas);
    pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(canvas));

    scene->addItem(pixmapItem);
}

//adiciona alguns objetos iniciais para o display file
void MainWindow::inicialSetupDisplayFile(){
    Point point1(canvas.width()/2, canvas.height()/2+145, 10, 255, 0, 0);
    Point point2(canvas.width()/2-100, canvas.height()/2-45, 0, 0, 255, 0);
    Point point3(canvas.width()/2+100, canvas.height()/2-45, 0, 0, 0, 255);

    Line linha1(Point(20,-50,0, 255,0,0), Point(250,400,0, 0,255,0));
    Point point11(120,120,20, 0,0,255);
    Point point12(120,80,20, 0,0,255);
    Point point13(80,80,20, 0,0,255);

    Polygon poly(point1,point2,point3);
    Polygon poly1(Point(0,0,0, 255,0,0), Point(50,50,0, 255,255,0), Point(50,0,0, 0,255,200));
    Polygon poly2(Point(300,100,0, 0,255,0), Point(280,80,0, 0,255,0), Point(320,80,0, 0,255,0));

    displayFile.insert("triangulo colorido rgb", poly);
    displayFile.insert("triangulo retangulo", poly1);
    displayFile.insert("triangulo verde", poly2);
    displayFile.insert("linha", linha1);
    displayFile.insert("ponto1", point11);
    displayFile.insert("ponto2", point12);
    displayFile.insert("ponto3", point13);

    displayFile.drawAll(canvas);
}


// Botão que aplica a transformação inserida pelo usuário
void MainWindow::on_applyTransform_clicked()
{
    std::string objName = ui->objList->currentText().toStdString();

    Matrix4x4 matrix = Matrix4x4::identity();

    Vector3<float> center = displayFile.getMean(objName);

    float angle = (float)ui->angleDial->value()/100.0 * M_PI * 2;
    float rotX = center.x + ui->rotateCoordX->text().toFloat();
    float rotY = center.y + ui->rotateCoordY->text().toFloat();

    float escalar = (float)ui->scaleBar->value()/100.0*4;

    float newX = ui->translateCoordX->text().toFloat();
    float newY = ui->translateCoordX->text().toFloat();

    //ESCALA
    matrix.translate(-center.x,-center.y,0);
    matrix.scale(escalar,escalar,escalar);
    matrix.translate(center.x,center.y,0);

    //ROTAÇÃO
    matrix.translate(-rotX,-rotY,0);
    matrix.rotate(angle, 0, 1);
    matrix.translate(rotX,rotY,0);

    center = displayFile.getMean(objName);

    //TRANSLAÇÃO
    matrix.translate(-center.x,-center.y,0);
    matrix.translate(newX,newY,0);

    //atribui nova matriz ao objeto
    displayFile.setMatrix(objName, matrix);
}

