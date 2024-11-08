#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geometry.h"
#include "matrix4x4.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    camera(Vector3<float>(0.0, 0.0, 0.0), Vector3<float>(0.0, 250.0, 0.0), Vector3<float>(0.0, 0.0, 1.0), 250.0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFocusPolicy(Qt::StrongFocus);

    //configurações iniciais
    QSize canvasSize = ui->visualizador->size();
    canvas = QImage(canvasSize.width(),canvasSize.height(), QImage::Format_RGB888);
    canvas.fill(Qt::white);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::updateFrame));
    timer->start(16); // 16ms entre cada atualização (~60 FPS)

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, ui->visualizador);


    //configurando o displayFile
    inicialSetupDisplayFile();

    for(std::string& name : displayFile.names)
        ui->objList->addItem(QString::fromStdString(name));

    pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(canvas));
    scene->addItem(pixmapItem);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateFrame() {
    paint();
    keyHandler();

    if (!hasFocus()) {
        setFocus();  // Teste configurar o foco novamente se estiver perdido
    }
}

// desenha os items na tela
void MainWindow::paint(){
    scene->removeItem(pixmapItem);

    canvas.fill(Qt::white);
    displayFile.drawAll(canvas, camera.getMatrix(), Vector3<float>(ui->visualizador->width(),ui->visualizador->height(),1.0) );
    pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(canvas));

    scene->addItem(pixmapItem);
}

//adiciona alguns objetos iniciais para o display file
void MainWindow::inicialSetupDisplayFile(){
    Point point1(ui->visualizador->width()/2, ui->visualizador->height()/2+145, 10, 255, 0, 0);
    Point point2(ui->visualizador->width()/2-100, ui->visualizador->height()/2-45, 0, 0, 255, 0);
    Point point3(ui->visualizador->width()/2+100, ui->visualizador->height()/2-45, 0, 0, 0, 255);

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
    displayFile.insert("eixo X", Line(Point(-300,0,0, 0,0,0),Point(300,0,0 ,0,0,0)));
    displayFile.insert("eixo y", Line(Point(0,-300,0 ,0,0,0),Point(0,300,0 ,0,0,0)));
}


// Botão que aplica a transformação inserida pelo usuário
void MainWindow::on_applyTransform_clicked()
{
    std::string objName = ui->objList->currentText().toStdString();
    Matrix4x4 matrix = displayFile.getMatrix(objName);

    float angle = (float)ui->angleDial->value()/100.0 * M_PI * 2;
    Vector3<float> rot = Vector3<float>(ui->rotateCoordX->text().toFloat(), ui->rotateCoordY->text().toFloat(), 0);

    //ROTAÇÃO
    matrix.translate(-rot);
    matrix.rotate(angle, 0, 1);
    matrix.translate(rot);

    //atribui nova matriz ao objeto
    displayFile.setMatrix(objName, matrix);
}


void MainWindow::on_applyTransform_scale_clicked()
{
    std::string objName = ui->objList->currentText().toStdString();
    Matrix4x4 matrix = displayFile.getMatrix(objName);

    Vector3<float> center = displayFile.getMean(objName);
    Vector3<float> scalar = Vector3<float>(ui->scaleBar->value()/100.0*4, ui->scaleBar->value()/100.0*4, ui->scaleBar->value()/100.0*4);
    //ESCALA
    matrix.translate(-center);
    matrix.scale(scalar);
    matrix.translate(center);

    displayFile.setMatrix(objName, matrix);
}


void MainWindow::on_applyTransform_translate_clicked()
{
    std::string objName = ui->objList->currentText().toStdString();
    Matrix4x4 matrix = displayFile.getMatrix(objName);

    Vector3<float> newCoord = Vector3<float>(ui->translateCoordX->text().toFloat(), ui->translateCoordY->text().toFloat(), 0);

    //TRANSLAÇÃO
    matrix.translate(newCoord);

    displayFile.setMatrix(objName, matrix);
}

void MainWindow::on_scaleBar_valueChanged(int value)
{
    float escalar = value/100.0*4;
    QString numero = "x" + QString::number(escalar);
    ui->scaleBarSubtitle->setText(numero);
}


void MainWindow::on_angleDial_valueChanged(int value)
{
    ui->angleLabel->setText(QString::number(value/100.0*360)+"º");
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    keysPressed.insert(event->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    keysPressed.remove(event->key());
}

void MainWindow::keyHandler(){
    Matrix4x4 m = Matrix4x4::identity();

    Vector3<float> vert = camera.getUp().normalize()*3;
    Vector3<float> hori = camera.getRight().normalize()*3;
    //rotation
    if (keysPressed.contains(Qt::Key_Q))
        m.rotate(0.01,0,1);
    if (keysPressed.contains(Qt::Key_E))
        m.rotate(-0.01,0,1);
    camera.setUp(m*camera.getUp());

    //moviment
    if (keysPressed.contains(Qt::Key_A))
        m.translate(-hori);
    if (keysPressed.contains(Qt::Key_W))
        m.translate(vert);
    if (keysPressed.contains(Qt::Key_S))
        m.translate(-vert);
    if (keysPressed.contains(Qt::Key_D))
        m.translate(hori);
    camera.setPos(m*camera.getPos());

    //zoom
    if (keysPressed.contains(Qt::Key_Z)){
        camera.setUp(camera.getUp()+camera.getUp().normalize()*1.2);
        camera.setRightScale(camera.getRightScale()+1.2);
    }
    if (keysPressed.contains(Qt::Key_X)){
        camera.setUp(camera.getUp()-camera.getUp().normalize()*1.2);
        camera.setRightScale(camera.getRightScale()-1.2);
    }

    //resizeVIEWPORT
    if (keysPressed.contains(Qt::Key_Right))
        ui->visualizador->resize(ui->visualizador->width() + 5, ui->visualizador->height());
    if (keysPressed.contains(Qt::Key_Left))
        ui->visualizador->resize(ui->visualizador->width() - 5, ui->visualizador->height());
    if (keysPressed.contains(Qt::Key_Up))
        ui->visualizador->resize(ui->visualizador->width(), ui->visualizador->height() - 5);
    if (keysPressed.contains(Qt::Key_Down))
        ui->visualizador->resize(ui->visualizador->width(), ui->visualizador->height() + 5);

    QSize canvasSize = ui->visualizador->size();
    canvas = QImage(canvasSize.width(),canvasSize.height(), QImage::Format_RGB888);
}
