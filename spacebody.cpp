#include "spacebody.h"
#include <QPainter>
#include <QWidget>

SpaceBody::SpaceBody(qreal xPos, qreal yPos,
                     qreal mass, qreal radius,
                     qreal velocity,
                     qreal angle, int stat)
    : _mass(mass)
    , _radius(radius)
    , _totalForce(PhysVector(0, 0, 0, 0))
    , _drawVelocityVector(false)
    , _static(stat)
{
    setPos(QPointF(xPos, yPos));
    _velocity = PhysVector(0, 0, 1, 1);
    _velocity.setLength(velocity);
    _velocity.setAngle(angle);
    setZValue(2);
}

SpaceBody::SpaceBody(QPointF pos, qreal mass, qreal xvel, qreal yvel, qreal rad)
    : _mass(mass)
    , _radius(rad)
    , _velocity(PhysVector(0, 0, xvel, yvel))
    , _totalForce(PhysVector(0, 0, 0, 0))
    , _drawVelocityVector(false)
    , _static(false)
{
    setPos(pos);
    setZValue(1);
}

void SpaceBody::addForce(PhysVector force)
{
    if (!isStatic())
        _totalForce = PhysVector(_totalForce + force);
}

void SpaceBody::applyForce()
{
    if (isStatic()) return;

    _totalForce.setLength(_totalForce.length() / _mass);
    _velocity = _velocity + _totalForce;

    moveBy(_velocity.xProjection(), -_velocity.yProjection());

    _totalForce = PhysVector(0, 0, 0, 0);
}

void SpaceBody::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *wgt)
{
    painter->fillPath(shape(), wgt->palette().base());
    painter->drawEllipse(-_radius, -_radius, _radius * 2, _radius * 2);

    // draw velocity vector
    if (_drawVelocityVector && !isStatic()){
        QLineF line(0, 0, 1, 1);
        line.setLength(_velocity.length() * 500);
        line.setAngle(_velocity.angle());
        painter->drawLine(line);
        QLineF arrow(line.p2(), line.p1());
        arrow.setLength(7);
        arrow.setAngle(arrow.angle() - 30);
        painter->drawLine(arrow);
        arrow.setAngle(arrow.angle() + 60);
        painter->drawLine(arrow);
    }
//    painter->setPen(Qt::red);
//    painter->drawRect(boundingRect());
}

QRectF SpaceBody::boundingRect() const
{    
//    QPainterPath path;
//    path.addRect(-_radius - 0.5, -_radius - 0.5,
//                 _radius * 2 + 0.5, _radius * 2 + 0.5);

//    if (_drawVelocityVector){
//        QLineF line(0, 0, 1, 1);
//        line.setLength(_velocity.length() * 500);
//        line.setAngle(_velocity.angle());
//        path.lineTo(line.p2());

//        QLineF arrow(line.p2(), line.p1());
//        arrow.setLength(7);
//        arrow.setAngle(arrow.angle() - 30);
//        path.lineTo(arrow.p2());
//        arrow.setAngle(arrow.angle() + 60);
//        path.lineTo(arrow.p2());
//    }

//    return path.boundingRect();

        return QRectF(-_radius - 0.5, -_radius - 0.5,
                      _radius * 2 + 0.5, _radius * 2 + 0.5);
}

QPainterPath SpaceBody::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}
