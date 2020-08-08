#include "bodytrail.h"

#include <QPainter>
#include <QDebug>
#include <QEasingCurve>

#include <algorithm>

BodyTrail::BodyTrail(QGraphicsItem *attachedTo)
    : _body(attachedTo)
    , QGraphicsItem(attachedTo)
    , _active(true)
    , _length(100)
{
    _path << QPointF(0, 0);
    setFlag(QGraphicsItem::ItemStacksBehindParent);
}

void BodyTrail::updateTrail()
{
    if (QLineF(_body->pos(), _path.last()).length() < 2)
    {
//        path.pop_front();
        return;
    }

    if (_path.size() > _length)
        _path.pop_front();

    _path.push_back(_body->pos());

//    update(QRectF(path.first(), path.last()).normalized());
}

QRectF BodyTrail::boundingRect() const
{
    return QRectF(mapFromScene(_path.first()), mapFromScene(_path.last()));
}

void BodyTrail::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *)
{
    if (!_length)
        return;

    qreal opacStep = .5 / _path.size();
    painter->setOpacity(0);

//    QEasingCurve curve(QEasingCurve::OutQuint);
//    painter->setPen(QPen(QBrush(Qt::black), 0));
    for (int i = 1; i < _path.size(); i++)
    {
//        painter->setPen(QPen(QBrush(Qt::black), curve.valueForProgress(qreal(i) / qreal(path.size())) * 10));

        painter->drawLine(mapFromScene(*(_path.begin() + i - 1)), mapFromScene(*(_path.begin() + i)));
        painter->setOpacity(painter->opacity() + opacStep);
    }

    //    painter->drawRect(boundingRect());
}

void BodyTrail::trailLengthChanged(int length)
{
    _length = length;
    if (length == 0)
    {
        _path.clear();
    }
}
