#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GraphicsView;
class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QActionGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum Theme { Light, Dark };

    GraphicsView *graphicsView;
    QActionGroup *modeGroup;
    QLabel *scaleLabel;
    QLabel *modeLabel;
    QLabel *timeScaleLabel;
    bool _running;
    Theme _currentTheme;

    void stop();
    void start();
    void setTheme(Theme);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onClickStartStop();
    void onClickSpeedDownDown();
    void onClickSpeedDown();
    void onClickSpeedUp();
    void onClickSpeedUpUp();
    void onToggleTrail(bool);
    void modeGroupTriggered(QAction *act);
    void onClickSave();
    void onClickLoad();
    void modeChanged(unsigned int);
    void zoomChanged(qreal newScale);
    void timeScaleChanged(int);
    void onClickTheme();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
