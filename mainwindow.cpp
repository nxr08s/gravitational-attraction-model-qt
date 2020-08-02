#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>
#include <QDebug>

#include "physvector.h"
#include "spacebody.h"
#include "graphicsview.h"
#include "bodytrail.h"

#define Rad(x) qDegreesToRadians(x)

const qreal G = 6.667E-3;

PhysVector MainWindow::calculateForce(SpaceBody *s1, SpaceBody *s2)
{
    PhysVector range(s1->pos().x(), s1->pos().y(),
                     s2->pos().x(), s2->pos().y());

    qreal magnitude = G * s1->getMass() * s2->getMass() / (range.length() * range.length());
    range.setLength(magnitude);
    return range;
}

void MainWindow::processCollision(SpaceBody *body1, SpaceBody *body2)
{
    qreal   contactAngle = Rad(QLineF(body1->pos(), body2->pos()).angle()),
            ang1 = Rad(body1->getVelocity().angle()),
            ang2 = Rad(body2->getVelocity().angle()),
            vel1 = body1->getVelocity().length(),
            vel2 = body2->getVelocity().length(),
            m1 = body1->getMass(),
            m2 = body2->getMass();

    //calculate body1's new velocity
    qreal   tmp1 = (vel1 * cos(ang1 - contactAngle) * (m1 - m2) + 2*m2*vel2*cos(ang2 - contactAngle)) / (m1 + m2);

    qreal   speedX1 = tmp1*cos(contactAngle) + vel1*sin(ang1 - contactAngle)*cos(contactAngle + M_PI_2),
            speedY1 = -(tmp1*sin(contactAngle) + vel1*sin(ang1 - contactAngle)*sin(contactAngle + M_PI_2));


    //calculate body1's new velocity
    qreal   tmp2 = (vel2 * cos(ang2 - contactAngle) * (m2 - m1) + 2*m1*vel1*cos(ang1 - contactAngle)) / (m1 + m2);

    qreal   speedX2 = tmp2*cos(contactAngle) + vel2*sin(ang2 - contactAngle)*cos(contactAngle + M_PI_2),
            speedY2 = -( tmp2*sin(contactAngle) + vel2*sin(ang2 - contactAngle)*sin(contactAngle + M_PI_2));

    //apply new velocities to space bodies
    PhysVector velocity1(speedX1 * 0.95, speedY1 * 0.95);
    PhysVector velocity2(speedX2 * 0.95, speedY2 * 0.95);

    body1->setVelocity(velocity1);
    body2->setVelocity(velocity2);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , graphicsScene(new QGraphicsScene)
    , graphicsView(new GraphicsView(graphicsScene))
    , timerId(startTimer(1000 / 75))
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(graphicsView);
    graphicsView->setRenderHint(QPainter::Antialiasing);

    SpaceBody *newBody;
    BodyTrail *trail;
    newBody = new SpaceBody(QPoint(0, 0), 260, 0, 0, 100);
    graphicsScene->addItem(newBody);
    bodies << newBody;
    trail = new BodyTrail(newBody);
    graphicsScene->addItem(trail);
    trails << trail;

    newBody = new SpaceBody(QPoint(200, 0), 10, 0, 0.05, 10);
    graphicsScene->addItem(newBody);
    bodies << newBody;
    trail = new BodyTrail(newBody);
    graphicsScene->addItem(trail);
    trails << trail;

    newBody = new SpaceBody(QPoint(-250, 50), 20, 0, -0.078, 7);
    graphicsScene->addItem(newBody);
    bodies << newBody;
    trail = new BodyTrail(newBody);
    graphicsScene->addItem(trail);
    trails << trail;

    newBody = new SpaceBody(QPoint(320, 43), 60, 0.05, 0, 34);
    graphicsScene->addItem(newBody);
    bodies << newBody;
    trail = new BodyTrail(newBody);
    graphicsScene->addItem(trail);
    trails << trail;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (timerId == event->timerId())
    {
        for (int i = 0; i < 100; i++)
        {
            // calculate all gravitational forces
            for (auto b1 = bodies.begin(); b1 != bodies.end(); ++b1)
                for (auto b2 = b1 + 1; b2 != bodies.end(); ++b2)
                {
                    PhysVector force = calculateForce(*b1, *b2);
                    (*b1)->addForce(force);
                    (*b2)->addForce(PhysVector::reversed(force));
                }

            // check for collisions
            for (auto b1 = bodies.begin(); b1 != bodies.end(); ++b1)
                for (auto b2 = b1 + 1; b2 != bodies.end(); ++b2)
                    if ((*b1)->collidesWithItem(*b2))
                        processCollision(*b1, *b2);

            // apply forces to all space bodies
            for (auto b : bodies)
                b->applyForce();

        }
        for (auto t : trails)
            t->updateTrail();
        graphicsScene->update(graphicsScene->sceneRect());
    }
}
