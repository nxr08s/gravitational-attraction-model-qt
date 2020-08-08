#include "graphicsview.h"

#include <QObject>
#include <QMouseEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QGraphicsScene>
#include <QLabel>
#include <QGraphicsWidget>
#include <QPushButton>
#include <QGraphicsProxyWidget>

#include "ruler.h"
#include "physvector.h"
#include "spacebody.h"
#include "bodytrail.h"
#include "itemproperties.h"

PhysVector GraphicsView::calculateForce(SpaceBody *s1, SpaceBody *s2)
{
    const qreal G = 6.667E-3;

    PhysVector range(s1->pos().x(), s1->pos().y(),
                     s2->pos().x(), s2->pos().y());

    qreal r = range.length();
    qreal magnitude = G * s1->getMass() * s2->getMass() / (r * r);
    range.setLength(magnitude);
    return range;
}

void GraphicsView::processCollision(SpaceBody *body1, SpaceBody *body2)
{
    qreal   contactAngle = qDegreesToRadians(QLineF(body1->pos(), body2->pos()).angle()),
            ang1 = qDegreesToRadians(body1->getVelocity().angle()),
            ang2 = qDegreesToRadians(body2->getVelocity().angle()),
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
    PhysVector velocity1(speedX1 * 1, speedY1 * 1);
    PhysVector velocity2(speedX2 * 1, speedY2 * 1);

    body1->setVelocity(velocity1);
    body2->setVelocity(velocity2);
}

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
    , _mode(Measure)
    , _graphicsScene(new QGraphicsScene)
    , _scale(1.)
    , _ctrlPressed(false)
    , _frameRate(60)
    , _trailLength(100)
    , _timeScale(50)
    , _timerId(-1)
    , _tailEnabled(true)
    , _currentItem(nullptr)
    , _propertiesWgt(nullptr)
{
    _graphicsScene = new QGraphicsScene;

    setScene(_graphicsScene);

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setRenderHint(QPainter::Antialiasing);
}

SpaceBody *GraphicsView::addItem(QPoint pos, qreal mass, qreal xVel, qreal yVel, qreal radius)
{
    SpaceBody *newBody = new SpaceBody(pos, mass, xVel, yVel, radius);
    BodyTrail *trail = new BodyTrail(newBody);
    _graphicsScene->addItem(newBody);
    _graphicsScene->addItem(trail);
    _bodies << newBody;
    _trails << trail;

    newBody->setFlag(QGraphicsItem::ItemIsMovable);
    newBody->setFlag(QGraphicsItem::ItemIsSelectable);

    connect(this, SIGNAL(trailLengthChanged(int)), trail, SLOT(trailLengthChanged(int)));
    connect(this, SIGNAL(vectorEnabled(bool)), newBody, SLOT(enableVector(bool)));

    return newBody;
}

void GraphicsView::timerEnable(bool enable)
{
    if (enable)
    {
        if (_timerId == -1)
            _timerId = startTimer(1000 / _frameRate);
    }
    else
    {
        killTimer(_timerId);
        _timerId = -1;
    }
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Space:
        if (!event->isAutoRepeat())
            setDragMode(QGraphicsView::ScrollHandDrag);
        break;
    case Qt::Key_Control:
        if (!event->isAutoRepeat())
            _ctrlPressed = true;
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Space:
        if (!event->isAutoRepeat())
            setDragMode(QGraphicsView::RubberBandDrag);
        break;
    case Qt::Key_Control:
        if (!event->isAutoRepeat())
            _ctrlPressed = false;
        break;
    default:
        QGraphicsView::keyReleaseEvent(event);
    }
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    if (_ctrlPressed)
        (event->angleDelta().y() > 0) ? (zoomIn()) : (zoomOut());
    else
        QGraphicsView::wheelEvent(event);
}

void GraphicsView::timerEvent(QTimerEvent *event)
{
    if (_timerId == event->timerId())
    {
        for (int i = 0; i < _timeScale; i++)
        {
            // calculate all gravitational forces
            for (auto b1 = _bodies.begin(); b1 != _bodies.end(); ++b1)
                for (auto b2 = b1 + 1; b2 != _bodies.end(); ++b2)
                {
                    PhysVector force = calculateForce(*b1, *b2);
                    (*b1)->addForce(force);
                    (*b2)->addForce(PhysVector::reversed(force));
                }

            // check for collisions
            for (auto b1 = _bodies.begin(); b1 != _bodies.end(); ++b1)
                for (auto b2 = b1 + 1; b2 != _bodies.end(); ++b2)
                    if ((*b1)->collidesWithItem(*b2))
                        processCollision(*b1, *b2);

            // apply forces to all space bodies
            for (auto b : _bodies)
                b->applyForce();
        }

        if (_tailEnabled)
        {
            for (auto t : _trails)
                t->updateTrail();
        }

        _graphicsScene->update();
    }
}

void GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect)

    painter->setOpacity(0.5);
    painter->setPen(Qt::black);
    painter->drawLine(-100, 0, 100, 0);
    painter->drawLine(0, -100, 0, 100);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    qDebug() << _graphicsScene->itemAt(mapToScene(event->pos()),transform());


    QGraphicsItem *itm = _graphicsScene->itemAt(mapToScene(event->pos()),transform());

    if (_propertiesWgt && itm != _propertiesWgt)
    {
        _propertiesWgt->deleteLater();
        _propertiesWgt  = nullptr;
        _currentItem    = nullptr;
        return;
    }
    if (_graphicsScene->itemAt(mapToScene(event->pos()),transform()) == _currentItem)
    {
        QGraphicsView::mousePressEvent(event);
        return;
    }

    if (event->button() == Qt::RightButton)
    {
        _currentItem = _graphicsScene->itemAt(mapToScene(event->pos()),transform());
        ItemProperties *itemProp = new ItemProperties(_currentItem);

        _propertiesWgt = _graphicsScene->addWidget(itemProp);

        _propertiesWgt->setPos(mapToScene(event->pos()));
        _propertiesWgt->setScale(1 / _scale);
        _propertiesWgt->setZValue(3);

        connect(_propertiesWgt, SIGNAL(destroyed()), this, SLOT(propertiesWidnowDestroyed()));
        connect(itemProp, SIGNAL(itemRemove(QGraphicsItem*)), this, SLOT(itemRemoved(QGraphicsItem*)));
    }
    else
    {
        QGraphicsView::mousePressEvent(event);
    }
}

void GraphicsView::setFrameRate(int rate)
{

}

void GraphicsView::setMouseMode(GraphicsView::MouseMode mode)
{
    qDebug() << mode;
    _mode = mode;
}

void GraphicsView::zoomIn()
{
    _scale *= 1.2;
    scale(1.2, 1.2);
    qDebug() << _scale;
}

void GraphicsView::zoomOut()
{
    _scale /= 1.2;
    scale(1 / 1.2, 1 / 1.2);
    qDebug() << _scale;
}

void GraphicsView::vectorChecked(bool enabled)
{
    emit vectorEnabled(enabled);
    _graphicsScene->update();
}

void GraphicsView::setTrailLength(int length)
{
    qDebug() << length;
    _tailEnabled = (length != 0);
    emit trailLengthChanged(length);
}

void GraphicsView::propertiesWidnowDestroyed()
{
    qDebug() << "destroyed";
    _propertiesWgt = nullptr;
    _currentItem = nullptr;
    _graphicsScene->update();
}

void GraphicsView::itemRemoved(QGraphicsItem *item)
{
    _graphicsScene->removeItem(item);
    _bodies.removeOne(dynamic_cast<SpaceBody*>(item));
}
