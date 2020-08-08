#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>
#include <QtCore>

#include "graphicsview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , graphicsView(new GraphicsView(this))
    , ui(new Ui::MainWindow)
    , modeGroup(new QActionGroup(this))
{
    ui->setupUi(this);

    setCentralWidget(graphicsView);
    graphicsView->setRenderHint(QPainter::Antialiasing);

    graphicsView->addItem(QPoint(0, 0), 260, 0, 0, 100);
    graphicsView->addItem(QPoint(200, 0), 10, 0, 0.084, 10);
    graphicsView->addItem(QPoint(-250, 50), 8, 0, 0.085, 7);
//    graphicsView->addItem(QPoint(320, 43), 60, 0.05, 0, 34);

    ui->moveAction->setData(QVariant(GraphicsView::Move));
    ui->editorAction->setData(QVariant(GraphicsView::Edit));
    ui->rulerAction->setData(QVariant(GraphicsView::Measure));
    modeGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);
    modeGroup->addAction(ui->moveAction);
    modeGroup->addAction(ui->editorAction);
    modeGroup->addAction(ui->rulerAction);
    connect(modeGroup, SIGNAL(triggered(QAction*)), this, SLOT(modeChanged(QAction*)));

    connect(ui->speedDownDownAction, SIGNAL(triggered()), this, SLOT(onClickSpeedDownDown()));
    connect(ui->speedDownAction, SIGNAL(triggered()), this, SLOT(onClickSpeedDown()));
    connect(ui->stopAction, SIGNAL(triggered()), this, SLOT(onClickStop()));
    connect(ui->startAction, SIGNAL(triggered()), this, SLOT(onClickStart()));
    connect(ui->speedUpAction, SIGNAL(triggered()), this, SLOT(onClickSpeedUp()));
    connect(ui->speedUpUpAction, SIGNAL(triggered()), this, SLOT(onClickSpeedUpUp()));

    connect(ui->zoomInAction, SIGNAL(triggered()), graphicsView, SLOT(zoomIn()));
    connect(ui->zoomOutAction, SIGNAL(triggered()), graphicsView, SLOT(zoomOut()));
    connect(ui->vectorsAction, SIGNAL(toggled(bool)), graphicsView, SLOT(vectorChecked(bool)));
    connect(ui->trailAction, SIGNAL(toggled(bool)), this, SLOT(onToggleTrail(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onClickStop()
{
    graphicsView->timerEnable(false);
}

void MainWindow::onClickStart()
{
    graphicsView->timerEnable(true);
}

void MainWindow::onClickSpeedDownDown()
{
    int scale = graphicsView->timeScale();
    scale = (scale - 5 > 1) ? (scale - 5) : (1);
    if (scale < 6)
    {
        ui->speedDownDownAction->setEnabled(false);
        if (scale < 2)
            ui->speedDownAction->setEnabled(false);
    }
    graphicsView->setTimeScale(scale);
}

void MainWindow::onClickSpeedDown()
{
    int scale = graphicsView->timeScale();
    scale = (scale - 1 > 1) ? (scale - 1) : (1);
    if (scale < 6)
    {
        ui->speedDownDownAction->setEnabled(false);
        if (scale < 2)
            ui->speedDownAction->setEnabled(false);
    }
    graphicsView->setTimeScale(scale);
}

void MainWindow::onClickSpeedUp()
{
    int scale = graphicsView->timeScale();
    scale = scale + 1;
    if (scale > 1)
    {
        ui->speedDownAction->setEnabled(true);
        if (scale > 5)
            ui->speedDownDownAction->setEnabled(true);
    }
    graphicsView->setTimeScale(scale);
}

void MainWindow::onClickSpeedUpUp()
{
    int scale = graphicsView->timeScale();
    scale = scale + 5;
    if (scale > 1)
    {
        ui->speedDownAction->setEnabled(true);
        if (scale > 5)
            ui->speedDownDownAction->setEnabled(true);
    }
    graphicsView->setTimeScale(scale);
}

void MainWindow::onToggleTrail(bool checked)
{
    graphicsView->setTrailLength((checked)?(100):(0));
}

void MainWindow::modeChanged(QAction* act)
{
    graphicsView->setMouseMode(static_cast<GraphicsView::MouseMode>(act->data().toUInt()));
}
