#include "spacebody.h"
#include <QPainter>

void SpaceBody::applyForce()
{
    totalForce = totalForce / mass;
    velocity = velocity + totalForce;

    setPos(pos().x() + velocity.getProjX(),
           pos().y() - velocity.getProjY());

    totalForce = PhysVector(0, 0, 0, 0);
}

void SpaceBody::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(Qt::black);
    painter->fillPath(shape(), QBrush(Qt::white));
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);

//    // draw velocity vector
    if (_drawVelocityVector)
    {
        QLineF line(0, 0, 1, 1);
        line.setLength(velocity.length() * 500);
        line.setAngle(velocity.angle());
        painter->drawLine(line);
    }

//    painter->drawRect(boundingRect());
//    painter->setPen(Qt::green);
//    painter->drawPath(shape());
}

QRectF SpaceBody::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(-radius - penWidth / 2, -radius - penWidth / 2,
                  radius * 2 + penWidth, radius * 2 + penWidth);

}

QPainterPath SpaceBody::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void SpaceBody::enableVector(bool enable)
{
    _drawVelocityVector = enable;
}
