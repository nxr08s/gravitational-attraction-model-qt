#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "physvector.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SpaceBody;
class GraphicsView;
class QGraphicsScene;
class BodyTrail;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QList<SpaceBody*> bodies;
    QList<BodyTrail*> trails;

    QGraphicsScene *graphicsScene;
    GraphicsView *graphicsView;

    int timerId;

    static PhysVector calculateForce(SpaceBody *s1, SpaceBody *s2) ;
    static void processCollision(SpaceBody*, SpaceBody*);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void timerEvent(QTimerEvent*) override;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
