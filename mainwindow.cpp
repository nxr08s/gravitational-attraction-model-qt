#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>
#include <QtCore>
#include <QFrame>

#include "graphicsview.h"
#include "dbhelper.h"
#include "loaddialog.h"

void MainWindow::stop()
{
    if (_running){
        graphicsView->timerEnable(false);
        _running = false;
        ui->startStopAction->setIcon(QIcon(":/resources/start.svg"));
    }
}

void MainWindow::start()
{
    if (!_running){
        graphicsView->timerEnable(true);
        _running = true;
        ui->startStopAction->setIcon(QIcon(":/resources/stop.svg"));
        modeChanged(GraphicsView::None);
        graphicsView->setMouseMode(GraphicsView::None);
    }
}

void MainWindow::setTheme(MainWindow::Theme theme)
{
    if (theme == Light){
        qApp->setStyleSheet("");
        ui->themeAction->setIcon(QIcon(":/resources/themeDark.svg"));
    }else{
        QFile f(":qdarkstyle/style.qss");

        if (!f.exists())   {
            return;
        }
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
        ui->themeAction->setIcon(QIcon(":/resources/themeLight.svg"));
    }
    _currentTheme = theme;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , graphicsView(new GraphicsView(this))
    , modeGroup(new QActionGroup(this))
    , _running(false)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Гравитационная модель");

    setTheme(Dark);

    ui->statusBar->addWidget(scaleLabel = new QLabel("Масштаб: 100%", this));
    ui->statusBar->addWidget(modeLabel = new QLabel("Режим: Просмотр", this));
    ui->statusBar->addWidget(timeScaleLabel =
            new QLabel("Множитель времени: " + QString::number(graphicsView->timeScale()),
                       this));


    setCentralWidget(graphicsView);
    graphicsView->setRenderHint(QPainter::Antialiasing);

    graphicsView->addItem(QPoint(0, 0), 2.1, 0, 0, 100);
    graphicsView->addItem(QPoint(200, 0), 0.1, 0, 0.084, 10);
    graphicsView->addItem(QPoint(-250, 50), 0.08, 0, 0.085, 7);

    ui->propEditAction->setData(QVariant(GraphicsView::PropEdit));
    ui->velEditAction->setData(QVariant(GraphicsView::VectorEdit));
    ui->addItemAction->setData(QVariant(GraphicsView::NewItem));

    modeGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);
    modeGroup->addAction(ui->propEditAction);
    modeGroup->addAction(ui->velEditAction);
    modeGroup->addAction(ui->addItemAction);
    connect(modeGroup, SIGNAL(triggered(QAction*)), this, SLOT(modeGroupTriggered(QAction*)));

    connect(ui->speedDownDownAction, SIGNAL(triggered()), this, SLOT(onClickSpeedDownDown()));
    connect(ui->speedDownAction, SIGNAL(triggered()), this, SLOT(onClickSpeedDown()));
    connect(ui->startStopAction, SIGNAL(triggered()), this, SLOT(onClickStartStop()));
    connect(ui->speedUpAction, SIGNAL(triggered()), this, SLOT(onClickSpeedUp()));
    connect(ui->speedUpUpAction, SIGNAL(triggered()), this, SLOT(onClickSpeedUpUp()));

    connect(ui->zoomInAction, SIGNAL(triggered()), graphicsView, SLOT(zoomIn()));
    connect(ui->zoomOutAction, SIGNAL(triggered()), graphicsView, SLOT(zoomOut()));
    connect(ui->vectorsAction, SIGNAL(toggled(bool)), graphicsView, SLOT(vectorChecked(bool)));
    connect(ui->trailAction, SIGNAL(toggled(bool)), this, SLOT(onToggleTrail(bool)));

    connect(ui->saveAction, SIGNAL(triggered()), this, SLOT(onClickSave()));
    connect(ui->loadAction, SIGNAL(triggered()), this, SLOT(onClickLoad()));

    connect(graphicsView, SIGNAL(modeChanged(unsigned int)),
            this, SLOT(modeChanged(unsigned int)));
    connect(graphicsView, SIGNAL(zoomChanged(qreal)), this, SLOT(zoomChanged(qreal)));
    connect(graphicsView, SIGNAL(timeScaleChanged(int)), this, SLOT(timeScaleChanged(int)));
    connect(ui->themeAction, SIGNAL(triggered()), this, SLOT(onClickTheme()));

    modeChanged(GraphicsView::None);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onClickStartStop()
{
    if (_running)
        stop();
    else{
        start();
        modeChanged(GraphicsView::None);
    }
}

void MainWindow::onClickSpeedDownDown()
{
    int scale = graphicsView->timeScale();
    scale = (scale - 5 > 1) ? (scale - 5) : (1);
    if (scale < 6){
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
    if (scale < 6){
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
    if (scale > 1){
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
    if (scale > 1){
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

void MainWindow::modeGroupTriggered(QAction* act)
{
    stop();

    GraphicsView::MouseMode mode = (modeGroup->checkedAction() == nullptr)?
                                       (GraphicsView::None):
                                       static_cast<GraphicsView::MouseMode>(act->data().toUInt());

    graphicsView->setMouseMode(mode);
    modeChanged(mode);
}

void MainWindow::onClickSave()
{
    stop();

    bool ok;
    QString saveName = QInputDialog::getText(nullptr, "Сохранить",
                                             "Введите имя сохраения",
                                             QLineEdit::Normal, "",
                                             &ok);
    if (!ok) return;

    if (saveName.isEmpty()){
        QMessageBox::warning(nullptr, "Сохранить", "Имя не должно быть пустым.");
        return;
    }

    DbHelper db;

    QStringList names = db.saveNames();
    if (names.contains(saveName)){
        QMessageBox::warning(nullptr,
                             "Сохранить", "Не удалось сохранить сцену. '"
                             + saveName + "' уже существует.");
        return;
    }
    db.save(saveName, graphicsView->bodies());
}

void MainWindow::onClickLoad()
{
    stop();

    DbHelper db;
    LoadDialog diag(this, &db);
    diag.exec();

    if (diag.result() == QDialog::Accepted){
        QList<SpaceBody*> items = db.load(diag.resultName());
        graphicsView->clearScene();
        graphicsView->insertItems(items);
    }
}

void MainWindow::modeChanged(unsigned int mode)
{
    switch (mode){
    case GraphicsView::None:
        ui->vectorsAction->setChecked(false);
        ui->velEditAction->setChecked(false);
        ui->propEditAction->setChecked(false);
        ui->addItemAction->setChecked(false);
        modeLabel->setText("Режим: Просмотор");
        break;
    case GraphicsView::NewItem:
        modeLabel->setText("Режим: Новый элемент");
        break;
    case GraphicsView::VectorEdit:
        ui->vectorsAction->setChecked(true);
        modeLabel->setText("Режим: Редактор скорости");
        break;
    case GraphicsView::PropEdit:
        modeLabel->setText("Режим: Редактор свойств");
        break;
    }
}

void MainWindow::zoomChanged(qreal)
{
    scaleLabel->setText("Масштаб: " + QString::number(100*graphicsView->sceneScale()) + "%");
}

void MainWindow::timeScaleChanged(int newTimeScale)
{
    timeScaleLabel->setText("Множитель времени: " + QString::number(newTimeScale));
}

void MainWindow::onClickTheme()
{
    if (_currentTheme == Light)
        setTheme(Dark);
    else
        setTheme(Light);
}
