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
#include <QElapsedTimer>

#include "physvector.h"
#include "spacebody.h"
#include "bodytrail.h"
#include "itemproperties.h"
#include "trajectory.h"

QList<SpaceBody *> GraphicsView::bodies()
{
    QList<SpaceBody*> returnList;
    for (auto item: items())
        if (item->type() == Type::Body)
            returnList << qgraphicsitem_cast<SpaceBody*>(item);

    return returnList;
}

PhysVector GraphicsView::calculateForce(SpaceBody *s1, SpaceBody *s2)
{
    PhysVector range(s1->pos() , s2->pos());
    qreal r = range.length();
    range.setLength(s1->getMass() * s2->getMass() / (r * r));

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
    if (!body1->isStatic()){
        qreal   tmp1 = (vel1 * cos(ang1 - contactAngle) * (m1 - m2)
                        + 2*m2*vel2*cos(ang2 - contactAngle)) / (m1 + m2);

        qreal   speedX1 = tmp1*cos(contactAngle) + vel1*sin(ang1 - contactAngle)
                          *cos(contactAngle + M_PI_2),
                speedY1 = -(tmp1*sin(contactAngle) + vel1*sin(ang1 - contactAngle)
                            *sin(contactAngle + M_PI_2));

        PhysVector velocity1(speedX1 * 1, speedY1 * 1);
        body1->setVelocity(velocity1);
    }

    //calculate body2's new velocity
    if (!body2->isStatic()){
        qreal   tmp2 = (vel2 * cos(ang2 - contactAngle) * (m2 - m1)
                        + 2*m1*vel1*cos(ang1 - contactAngle)) / (m1 + m2);

        qreal   speedX2 = tmp2*cos(contactAngle) + vel2*sin(ang2 - contactAngle)*
                          cos(contactAngle + M_PI_2),
                speedY2 = -(tmp2*sin(contactAngle) + vel2*sin(ang2 - contactAngle)*
                            sin(contactAngle + M_PI_2));

        PhysVector velocity2(speedX2 * 1, speedY2 * 1);
        body2->setVelocity(velocity2);
    }
}

QGraphicsItem *GraphicsView::getBodyUnderMouse(QPoint pos)
{
    QList<QGraphicsItem*> itemsOnPos = items(pos);
    QGraphicsItem *body = nullptr;
    for (auto item: itemsOnPos){
        if (item->type() == Body){
            body = item;
            break;
        }
    }
    return body;
}

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
    , _mode(None)
    , _scale(1.)
    , _ctrlPressed(false)
    , _spacePressed(false)
    , _frameRate(60)
    , _trailLength(100)
    , _timeScale(50)
    , _timerId(-1)
    , _tailEnabled(true)
    , _propertiesWgt(nullptr)
    , _currentVelocityItem(nullptr)
{
    setScene(new QGraphicsScene(this));

    setTransformationAnchor(AnchorUnderMouse);
    setRenderHint(QPainter::Antialiasing);
    //    setViewportUpdateMode(SmartViewportUpdate);
    //    setOptimizationFlag(DontAdjustForAntialiasing);
}

void GraphicsView::addItem(QPointF pos, qreal mass, qreal xVel, qreal yVel, qreal radius)
{
    SpaceBody *newBody = new SpaceBody(pos, mass, xVel, yVel, radius);
    BodyTrail *trail = new BodyTrail(newBody);
    scene()->addItem(newBody);

    trail->setLength(_trailLength);

    newBody->setFlag(QGraphicsItem::ItemIsMovable);

    connect(this, SIGNAL(updateTrail()), trail, SLOT(updateTrail()));
    connect(this, SIGNAL(trailLengthChanged(int)), trail, SLOT(setLength(int)));
    connect(this, SIGNAL(enableVector(bool)), newBody, SLOT(enableVector(bool)));

    emit enableVector(_vectorEnabled);
}

void GraphicsView::insertItems(QList<SpaceBody*> items)
{
    for (SpaceBody* item: items){
        BodyTrail* trail = new BodyTrail(item);
        scene()->addItem(item);

        trail->setLength(_trailLength);
        item->setFlag(QGraphicsItem::ItemIsMovable);

        connect(this, SIGNAL(updateTrail()), trail, SLOT(updateTrail()));
        connect(this, SIGNAL(trailLengthChanged(int)), trail, SLOT(setLength(int)));
        connect(this, SIGNAL(enableVector(bool)), item, SLOT(enableVector(bool)));
    }
    emit enableVector(_vectorEnabled);
    scene()->update();
}

void GraphicsView::timerEnable(bool enable)
{
    if (enable){
        if (_timerId == -1)
            _timerId = startTimer(1000 / _frameRate);
    }
    else{
        killTimer(_timerId);
        _timerId = -1;
    }
}

void GraphicsView::clearScene()
{
    scene()->deleteLater();
    setScene(new QGraphicsScene(this));
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Space:
        if (!event->isAutoRepeat()){
            setDragMode(QGraphicsView::ScrollHandDrag);
            _spacePressed = true;
        }
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
        if (!event->isAutoRepeat()){
            setDragMode(QGraphicsView::NoDrag);
            _spacePressed = false;
        }
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
    if (_ctrlPressed){
        (event->angleDelta().y() > 0) ? (zoomIn()) : (zoomOut());
    }
    else
        QGraphicsView::wheelEvent(event);
}

void GraphicsView::timerEvent(QTimerEvent *event)
{
    if (_timerId == event->timerId()){
        QList<SpaceBody*> items = bodies();
        for (int i = 0; i < _timeScale; i++){
//            QElapsedTimer timer;
//            timer.start();
//            qDebug() << " ------ ";

            //             calculate all gravitational forces
            for (auto b1 = items.begin(); b1 != items.end(); ++b1)
                for (auto b2 = b1 + 1; b2 != items.end(); ++b2){
                    PhysVector force = calculateForce(*b1, *b2);
                    (*b1)->addForce(force);
                    (*b2)->addForce(PhysVector::reversed(force));
                }
//            qDebug() << "forces:" << timer.nsecsElapsed();
//            timer.restart();

            // check for collisions
            for (auto b1 = items.begin(); b1 != items.end(); ++b1)
                for (auto b2 = b1 + 1; b2 != items.end(); ++b2)
                    if ((*b1)->collidesWithItem(*b2))
                        processCollision(*b1, *b2);
//            qDebug() << "collisions:" << timer.nsecsElapsed();
//            timer.restart();

            // apply forces to all space bodies
            for (auto b : items)
                b->applyForce();
//            qDebug() << "applying forces:" << timer.nsecsElapsed();
//            timer.restart();
        }

        if (_tailEnabled)
            emit updateTrail();
    }
}

//void GraphicsView::drawBackground(QPainter *painter, const QRectF&)
//{
//    painter->setOpacity(0.5);
////    painter->setPen(Qt::black);
//    painter->drawLine(-100, 0, 100, 0);
//    painter->drawLine(0, -100, 0, 100);
//}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (_spacePressed){
        QGraphicsView::mousePressEvent(event);
        return;
    }

    QGraphicsItem *itm = getBodyUnderMouse(event->pos());

    switch (_mode){
    case VectorEdit:
        if (itm && itm->type() == Body && event->button() == Qt::LeftButton){
            _currentVelocityItem = itm;

            QLineF line(itm->pos(), mapToScene(event->pos()));

            SpaceBody* item = qgraphicsitem_cast<SpaceBody*>(itm);

            if (item->getVelocity().isNull()){
                item->getVelocity().setP2(QPointF(qreal(1) / 500, qreal(1) / 500));
            }

            item->getVelocity().setLength(line.length() / 500);
            item->getVelocity().setAngle(line.angle());

            QList<SpaceBody*> items(bodies());
            items.removeOne(item);

            _trajectory = new Trajectory(_currentVelocityItem, items);
            _trajectory->updatePath();
        }
        break;
    case PropEdit:
        if (_propertiesWgt){
            if (itemAt(event->pos()) != _propertiesWgt){
                _propertiesWgt->deleteLater();
                _propertiesWgt = nullptr;
                return;
            }
        }else{
            if (itm && itm->type() == Body && event->button() == Qt::RightButton){
                ItemProperties *itemProp = new ItemProperties(itm);

                _propertiesWgt = scene()->addWidget(itemProp);

                _propertiesWgt->setPos(mapToScene(event->pos()));
                _propertiesWgt->setScale(1 / _scale);
                _propertiesWgt->setZValue(3);

                connect(_propertiesWgt, SIGNAL(destroyed()),
                        this, SLOT(propertiesWidnowDestroyed()));
                connect(itemProp, SIGNAL(itemRemove(QGraphicsItem*)),
                        this, SLOT(itemRemoved(QGraphicsItem*)));
                return;
            }
        }
        QGraphicsView::mousePressEvent(event);
        break;
    case NewItem:
        if (!itm || itm->type() != Body){
            addItem(mapToScene(event->pos()),10,0,0,10);
            setMouseMode(None);
            emit modeChanged(None);
            scene()->update();
        }
        break;
    case None:
        return;
    }
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (_spacePressed){
        QGraphicsView::mouseMoveEvent(event);
        return;
    }
    switch (_mode){
    case VectorEdit:
        if (_currentVelocityItem){
            QLineF line(_currentVelocityItem->pos(), mapToScene(event->pos()));

            SpaceBody* item = dynamic_cast<SpaceBody*>(_currentVelocityItem);
            if (item->getVelocity().isNull()){
                item->getVelocity().setP2(QPointF(qreal(1) / 500, qreal(1) / 500));
            }

            item->setVelocity(line.length() / 500);
            item->getVelocity().setAngle(line.angle());

            _trajectory->updatePath();

            scene()->update();
        }
        break;
    case PropEdit:
        scene()->update();
        break;
    case NewItem:
        break;
    case None:
        break;
    }
    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    switch (_mode){
    case VectorEdit:
        if (_currentVelocityItem){
            _currentVelocityItem = nullptr;
            scene()->removeItem(_trajectory);
            //      !!! MEMORY LEAK !!! \/---- fix it (crashes smt if delete this)
            //            delete _trajectory;
            _trajectory = nullptr;
        }
        break;
    case None:
    case NewItem:
    case PropEdit:
        break;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::setMouseMode(GraphicsView::MouseMode mode)
{
    switch (_mode){
    case None:
        break;
    case VectorEdit:
        _currentVelocityItem = nullptr;
        break;
    case PropEdit:
        if (_propertiesWgt){
            scene()->removeItem(_propertiesWgt);
            _propertiesWgt = nullptr;
        }
        break;
    case NewItem:
        break;
    }

    _mode = mode;
}

void GraphicsView::zoomIn()
{
    _scale *= 1.2;
    scale(1.2, 1.2);
    emit zoomChanged(1.2);
}

void GraphicsView::zoomOut()
{
    _scale /= 1.2;
    scale(1 / 1.2, 1 / 1.2);
    emit zoomChanged(1 / 1.2);
}

void GraphicsView::vectorChecked(bool enabled)
{
    _vectorEnabled = enabled;
    emit enableVector(enabled);
    scene()->update();
}

void GraphicsView::setTrailLength(int length)
{
    _tailEnabled = (length != 0);
    emit trailLengthChanged(length);
}

void GraphicsView::propertiesWidnowDestroyed()
{
    _propertiesWgt = nullptr;
    scene()->update();
}

void GraphicsView::itemRemoved(QGraphicsItem *item)
{
    scene()->removeItem(item);
    delete item;
}
