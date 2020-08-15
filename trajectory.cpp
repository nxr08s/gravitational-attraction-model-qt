#include "trajectory.h"

#include <QGraphicsScene>
#include <QPainter>

#include "physvector.h"
#include "spacebody.h"

Trajectory::Trajectory(QGraphicsItem* parent, QList<SpaceBody*> items)
    : QGraphicsItem(parent)
    , _bodies(items)
{
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

        bool intersects = false;
        for (auto body : _bodies){
            if (body->contains(body->mapFromScene(tempPos))){
                intersects = true;
                break;
            }
        }
        if (intersects)
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
        painter->drawLine(mapFromScene(_points.at(i - 1)),
                          mapFromScene(_points.at(i)));
        painter->setOpacity(painter->opacity() - opacityDelta);
    }

}

QRectF Trajectory::boundingRect() const
{
    return _path.boundingRect();
}
