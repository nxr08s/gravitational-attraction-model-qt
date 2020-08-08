#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphicsview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//class GraphicsView;
class QActionGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    GraphicsView *graphicsView;
    QActionGroup *modeGroup;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onClickStop();
    void onClickStart();
    void onClickSpeedDownDown();
    void onClickSpeedDown();
    void onClickSpeedUp();
    void onClickSpeedUpUp();
    void onToggleTrail(bool);
    void modeChanged(QAction *act);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
