#include "trajectory.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>

#include "physvector.h"
#include "spacebody.h"

Trajectory::Trajectory(QGraphicsItem* parent, QList<SpaceBody*> &items)
    : QGraphicsItem(parent)
    , _bodies(items)
{
    updatePath();
    setFlag(QGraphicsItem::ItemStacksBehindParent);
    setAcceptedMouseButtons(Qt::NoButton);
}

void Trajectory::updatePath()
{
    _path.clear();
    _points.clear();

    _points << parentItem()->pos();

    QPointF tempPos = parentItem()->pos();
    PhysVector velocity(qgraphicsitem_cast<SpaceBody*>(parentItem())->getVelocity());

    while (_points.length() < __length)
    {
        PhysVector force(0, 0);
        for (auto body : _bodies){
            PhysVector range(tempPos, body->pos());
            range.setLength(body->getMass() / (range.length() * range.length()));
            force = force + range;
        }

        velocity = velocity + force;
        tempPos = QPointF(tempPos.x() + velocity.xProjection(),
                          tempPos.y() - velocity.yProjection());

        QList<QGraphicsItem*> itemsUnderPointer = scene()->items(tempPos, Qt::ContainsItemShape);
        QGraphicsItem *intersectedItem = nullptr;
        for (auto item : itemsUnderPointer){
            if (item->type() == 65536){
                intersectedItem = item;
                break;
            }
        }

        if (intersectedItem &&
            intersectedItem != parentItem() &&
            intersectedItem->type() == 65536)   // 65536 - QGraphicsItem::UserType (SpaceBody)
            break;

        if (QLineF(tempPos, _points.last()).length() > 3)
        {
            _points.push_back(tempPos);
            _path.lineTo(mapFromScene(tempPos));
        }
    }
}

void Trajectory::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    qreal opacityDelta = qreal(1) / _points.size();

    painter->setOpacity(1);

    for (int i = 1; i < _points.size(); i++){
        painter->drawLine(mapFromScene(*(_points.begin() + i - 1)),
                          mapFromScene(*(_points.begin() + i)));
        painter->setOpacity(painter->opacity() - opacityDelta);
    }

}

QRectF Trajectory::boundingRect() const
{
    return _path.boundingRect();
}
