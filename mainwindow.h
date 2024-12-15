#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QSizeGrip>
#include <QVBoxLayout>

#include "displayfile.h"
#include "window.h"

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
    Window camera;
    QSet<int> keysPressed;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void inicialSetupDisplayFile();
    void paint();
    void updateFrame();
    void keyHandler();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    bool eventFilter(QObject *obj, QEvent *event);

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
