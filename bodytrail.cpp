#include "bodytrail.h"

#include <QPainter>
#include <QEasingCurve>
#include <QDebug>

#include <algorithm>

const int __base_length = 100;

BodyTrail::BodyTrail(QGraphicsItem *attachedTo)
    : QGraphicsItem(attachedTo)
    , _length(__base_length)
{
    _points << parentItem()->pos();
    setFlag(QGraphicsItem::ItemStacksBehindParent);
    setAcceptedMouseButtons(Qt::NoButton);
}

void BodyTrail::updateTrail()
{

    if (QLineF(parentItem()->pos(), _points.last()).length() < 2)
        return;

    if (_points.size() > _length)
        _points.removeFirst();

    _points.push_back(parentItem()->pos());

    _path = QPainterPath();
    for (auto pt : _points)
        _path.lineTo(mapFromScene(pt));
}

QRectF BodyTrail::boundingRect() const
{
    return _path.boundingRect();
}

void BodyTrail::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *)
{
    if (!_length)
        return;

    qreal fullOpacity = .5;

    qreal opacStep = fullOpacity / _points.size();
    painter->setOpacity(0);

    for (int i = 1; i < _points.size(); i++){
        painter->drawLine(mapFromScene(*(_points.begin() + i - 1)),
                          mapFromScene(*(_points.begin() + i)));
        painter->setOpacity(painter->opacity() + opacStep);
    }
}

void BodyTrail::setLength(int length)
{
    _length = length;
    if (length == 0){
        _points.clear();
        _points << parentItem()->pos();
    }
}
