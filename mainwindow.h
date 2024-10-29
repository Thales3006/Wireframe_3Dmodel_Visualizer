#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "displayfile.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QTimer* timer;
    QGraphicsScene* scene;
    QGraphicsView* view;
    QGraphicsPixmapItem* pixmapItem;
    QImage canvas;

    DisplayFile displayFile;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void inicialSetupDisplayFile();
    void paintEvent(QPaintEvent *event);

private slots:

    void on_applyTransform_clicked();

    void on_applyTransform_scale_clicked();

    void on_applyTransform_translate_clicked();

    void on_scaleBar_valueChanged(int value);

    void on_angleDial_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
