#include "ruler.h"

#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

Ruler::Ruler(QPoint point)
    : QGraphicsItem()
    , start(point)
{

}

void Ruler::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawLine(0,0,10,10);
}

QRectF Ruler::boundingRect() const
{
    return QRectF(0,0,0,0);
}

void Ruler::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << event->pos();
}
