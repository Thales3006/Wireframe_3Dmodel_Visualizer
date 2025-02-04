#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geometry.h"
#include "matrix4x4.h"
#include "object.h"
#include "enums.h"

#include <iostream>
#include <filesystem>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QElapsedTimer>

float calcAngle(float value){
    return value/100.0*360;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    camera(Vector3<float>(0,100,-500), Vector3<float>(1000,250,250), Vector3<float>(0,0,0)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //configurações iniciais
    QSize canvasSize = ui->visualizador->size();
    canvas = QImage(canvasSize.width(),canvasSize.height(), QImage::Format_RGB888);
    canvas.fill(Qt::white);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::updateFrame));
    timer->start(16); // 16ms entre cada atualização (~60 FPS)

    qApp->installEventFilter(this);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, ui->visualizador);

    //configurações da camera

    camera.setProjection(true, 120, 0.01, 1);

    //configurando o displayFile
    inicialSetupDisplayFile();

    for(const auto& pair: displayFile.objects)
        ui->objList->addItem(QString::fromStdString(pair.first));

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
}

// desenha os items na tela
void MainWindow::paint(){
    scene->removeItem(pixmapItem);
    delete pixmapItem;

    canvas.fill(Qt::white);
    displayFile.drawAll(canvas, camera.getView(), Vector3<float>(ui->visualizador->width(),ui->visualizador->height(),1.0) );
    pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(canvas));

    scene->addItem(pixmapItem);
}

//adiciona alguns objetos iniciais para o display file
void MainWindow::inicialSetupDisplayFile(){
    Point point1(ui->visualizador->width()/2, ui->visualizador->height()/2+145, 10, 255, 0, 0);
    Point point2(ui->visualizador->width()/2-100, ui->visualizador->height()/2-45, 0, 0, 255, 0);
    Point point3(ui->visualizador->width()/2+100, ui->visualizador->height()/2-45, 0, 0, 0, 255);

    Line linha1(Point(20,-50,0, 255,0,0), Point(250,400,0, 0,255,0));
    Point point11(120,120,40, 0,0,255);
    Point point12(120,80,40, 0,0,255);
    Point point13(80,80,40, 0,0,255);

    Polygon poly(point1,point2,point3);
    Polygon poly1(Point(0,0,100, 255,0,0), Point(50,50,100, 255,255,0), Point(50,0,100, 0,255,200));
    Polygon poly2(Point(300,100,300, 0,255,0), Point(280,80,250, 0,255,0), Point(320,80,250, 0,255,0));

    displayFile.insert("triangulo colorido rgb", poly);
    displayFile.insert("triangulo retangulo", poly1);
    displayFile.insert("triangulo verde", poly2);
    displayFile.insert("linha", linha1);
    displayFile.insert("ponto1", point11);
    displayFile.insert("ponto2", point12);
    displayFile.insert("ponto3", point13);
    displayFile.insert("eixo X", Line(Point(-300,0,0, 0,0,0),Point(300,0,0 ,0,0,0)));
    displayFile.insert("eixo y", Line(Point(0,-300,0 ,0,0,0),Point(0,300,0 ,0,0,0)));

    //TESTING ============
    Object charizard;
    Object umbreon;
    if(charizard.loadObj("../../objs/charizard.obj")==ALL_GOOD) {
        Matrix4x4 m = Matrix4x4::identity();
        m.scale(Vector3<float>(10.0,10.0,10.0));
        m.translate(Vector3<float>(100,00,0));
        displayFile.insert("charizard", charizard.multiply(m));
    }
    if(umbreon.loadObj("../../objs/umbreon.obj")==ALL_GOOD) {
        Matrix4x4 m = Matrix4x4::identity();
        m.scale(Vector3<float>(60.0,60.0,60.0));
        m.translate(Vector3<float>(-100, 50, 0));
        displayFile.insert("umbreon", umbreon.multiply(m));
    }
    //===================
}


// Botão que aplica a transformação inserida pelo usuário
void MainWindow::on_applyTransform_clicked()
{
    std::string objName = ui->objList->currentText().toStdString();

    if(!displayFile.exist(objName)){
        QMessageBox::information(this, tr("Erro"), "Objeto não existente ou não selecionado!");
        return;
    }

    Matrix4x4 matrix = Matrix4x4::identity();

    float roll = (float)ui->angleDial_R->value()/100.0 * M_PI * 2;
    float yaw = (float)ui->angleDial_Y->value()/100.0 * M_PI * 2;
    float pitch = (float)ui->angleDial_P->value()/100.0 * M_PI * 2;
    Vector3<float> rot = Vector3<float>(ui->rotateCoordX->text().toFloat(), ui->rotateCoordY->text().toFloat(), ui->rotateCoordZ->text().toFloat());

    //ROTAÇÃO
    matrix.translate(-rot);
    matrix.rotateY(yaw);
    matrix.rotateX(pitch);
    matrix.rotateZ(roll);
    matrix.translate(rot);

    //atribui nova matriz ao objeto
    displayFile.multiply(objName, matrix);
}


void MainWindow::on_applyTransform_scale_clicked()
{
    std::string objName = ui->objList->currentText().toStdString();

    if(!displayFile.exist(objName)){
        QMessageBox::information(this, tr("Erro"), "Objeto não existente ou não selecionado!");
        return;
    }

    Matrix4x4 matrix = Matrix4x4::identity();

    Vector3<float> center = displayFile.getMean(objName);
    Vector3<float> scalar = Vector3<float>(ui->scaleBar->value()/100.0*4, ui->scaleBar->value()/100.0*4, ui->scaleBar->value()/100.0*4);
    //ESCALA
    matrix.translate(-center);
    matrix.scale(scalar);
    matrix.translate(center);

    displayFile.multiply(objName, matrix);
}


void MainWindow::on_applyTransform_translate_clicked()
{
    std::string objName = ui->objList->currentText().toStdString();

    if(!displayFile.exist(objName)){
        QMessageBox::information(this, tr("Erro"), "Objeto não existente ou não selecionado!");
        return;
    }

    Matrix4x4 matrix = Matrix4x4::identity();

    Vector3<float> newCoord = Vector3<float>(ui->translateCoordX->text().toFloat(), ui->translateCoordY->text().toFloat(), ui->translateCoordZ->text().toFloat());

    //TRANSLAÇÃO
    matrix.translate(newCoord);

    displayFile.multiply(objName, matrix);
}

void MainWindow::on_scaleBar_valueChanged(int value)
{
    float escalar = value/100.0*4;
    QString numero = "x" + QString::number(escalar);
    ui->scaleBarSubtitle->setText(numero);
}


void MainWindow::on_angleDial_R_valueChanged(int value)
{
    ui->angleLabel_R->setText(QString::number(calcAngle(value))+"º");
}

void MainWindow::on_angleDial_Y_valueChanged(int value)
{
    ui->angleLabel_Y->setText(QString::number(calcAngle(value))+"º");
}

void MainWindow::on_angleDial_P_valueChanged(int value)
{
    ui->angleLabel_P->setText(QString::number(calcAngle(value))+"º");
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    keysPressed.insert(event->key());

    switch(event->key()) {
    case Qt::Key_P:
        Vector3<float> up = camera.getUp();
        Vector3<float> right = camera.getRight();

        std::cout << "up = (" << up.x << ", " << up.y << ", " << up.z << ")" << std::endl;
        std::cout << "|up| = " << up.length() << std::endl;
        std::cout << "right = (" << right.x << ", " << right.y << ", " << right.z << ")" << std::endl;
        std::cout << "|right| = " << right.length() << std::endl;
        break;
    }

}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    keysPressed.remove(event->key());
}

void MainWindow::keyHandler(){
    Matrix4x4 position = Matrix4x4::identity();

    float w = 0.03;
    float vel = 10;

    Vector3<float> angle = camera.getAngle();
    float yaw = angle.x;
    float pitch = angle.y;
    float roll = angle.z;

    //rotation
    if (keysPressed.contains(Qt::Key_U))
        roll -= w;
    if (keysPressed.contains(Qt::Key_O))
        roll += w;
    if (keysPressed.contains(Qt::Key_J))
        yaw -= w;
    if (keysPressed.contains(Qt::Key_L))
        yaw += w;
    if (keysPressed.contains(Qt::Key_I))
        pitch -= w;
    if (keysPressed.contains(Qt::Key_K))
        pitch += w;
    camera.setAngle(Vector3<float>(yaw, pitch, roll));

    Vector3<float> up = camera.getUp().normalize()*vel;
    Vector3<float> side = camera.getRight().normalize()*vel;
    Vector3<float> front = camera.getDir().normalize()*vel;
    //movement
    if (keysPressed.contains(Qt::Key_A))
        position.translate(-side);
    if (keysPressed.contains(Qt::Key_W))
        position.translate(front);
    if (keysPressed.contains(Qt::Key_S))
        position.translate(-front);
    if (keysPressed.contains(Qt::Key_D))
        position.translate(side);
    if (keysPressed.contains(Qt::Key_Shift))
        position.translate(-up);
    if (keysPressed.contains(Qt::Key_Space))
        position.translate(up);
    camera.setPos(position*camera.getPos());

    //zoom
    const float zoomChange = 1.02;

    if (keysPressed.contains(Qt::Key_Z)){
        camera.setUp(camera.getUpScale()*zoomChange);
        camera.setRight(camera.getRightScale()*zoomChange);
    }
    if (keysPressed.contains(Qt::Key_X)){
        if(camera.getUpScale()>=0.02*zoomChange&&camera.getRightScale()>=0.02*zoomChange) {
            camera.setUp(camera.getUpScale()/zoomChange);
            camera.setRight(camera.getRightScale()/zoomChange);
        }
    }

    //FOV
    int FOV = camera.getFOV();
    float minDrawDistance = camera.getMinDrawDistance();
    float maxDrawDistance = camera.getMaxDrawDistance();
    const float drawDistanceUnit = 0.01;

    if (keysPressed.contains(Qt::Key_N) && FOV < 180)
        FOV++;
    if (keysPressed.contains(Qt::Key_M) && FOV > 0)
        FOV--;
    if (keysPressed.contains(Qt::Key_Y) && minDrawDistance < maxDrawDistance)
        minDrawDistance += drawDistanceUnit;
    if (keysPressed.contains(Qt::Key_H) && minDrawDistance > 0)
        minDrawDistance -= drawDistanceUnit;
    if (keysPressed.contains(Qt::Key_BracketLeft))
        maxDrawDistance += drawDistanceUnit;
    if (keysPressed.contains(Qt::Key_BracketRight) && maxDrawDistance > minDrawDistance)
        maxDrawDistance -= drawDistanceUnit;
    camera.setProjection(true, FOV, minDrawDistance, maxDrawDistance);


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

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        keyReleaseEvent(keyEvent);
        return true;
    }

    return QObject::eventFilter(obj, event);
}

void MainWindow::on_objloadbutton_clicked() {
    const QString fileName= QFileDialog::getOpenFileName(
        this,
        tr("Abrir"),
        QString::fromStdString(std::filesystem::current_path().string() + "/../../objs"),
        "obj files (*.obj);;All files (*.*)"
        );

    if(fileName.isEmpty())
        return;

    const QString suffix = QString::fromStdString(".obj");

    if(!fileName.endsWith(suffix)){
        QMessageBox::information(this, tr("Erro"), "Arquivo não é compatível (Apenas arquivos com extensão .obj)");
        return;
    }

    Object obj;
    QString newName = QFileInfo(fileName).fileName().chopped(suffix.size());

    if(obj.loadObj(fileName.toStdString()) == ALL_GOOD){
        int count = 0;
        while(displayFile.exist(newName.toStdString())){
            newName = newName + QString::fromStdString(std::to_string(++count));
        }

        displayFile.insert(newName.toStdString(), obj);
        ui->objList->addItem(newName);
        return;
    }

    QMessageBox::information(this, tr("Erro"), "Não foi possível carregar o arquivo.");
}


void MainWindow::on_objdestroybutton_clicked()
{
    std::string objName = ui->objList->currentText().toStdString();

    if(displayFile.empty()){
        QMessageBox::information(this, tr("Erro"), "Não há mais objetos no display file!");
        return;
    }

    ui->objList->removeItem(ui->objList->currentIndex());
    displayFile.remove(objName);

    ui->objList->setCurrentIndex(0);
}

