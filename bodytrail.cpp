#include "bodytrail.h"

#include <QPainter>
#include <QDebug>
#include <algorithm>

BodyTrail::BodyTrail(QGraphicsItem *attachedTo, int len)
    : QGraphicsItem()
    , body(attachedTo)
    , active(true)
    , length(len)
    , minX(body->x())
    , maxX(minX)
    , minY(body->y())
    , maxY(minY)
{

}

void BodyTrail::updateTrail()
{
    if (QLineF(body->pos(), path.last()).length() < 2)
    {
        path.removeFirst();
        return;
    }

    if (path.size() > length)
        path.pop_front();

    path.push_back(body->pos());

//    update(QRectF(path.first(), path.last()).normalized());
}

QRectF BodyTrail::boundingRect() const
{
//    return QRectF(minX, minY, maxX, maxY).normalized();
    return QRectF(path.first(), path.last()).normalized();
}

void BodyTrail::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal opacStep = .5 / path.size();

    painter->setOpacity(0);
    for (auto p = path.begin() + 1; p != path.end(); ++p)
    {
        painter->drawLine(*(p - 1), *(p));
        painter->setOpacity(painter->opacity() + opacStep);
    }

//    painter->drawRect(boundingRect());
}
