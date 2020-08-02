#ifndef BODYTRAIL_H
#define BODYTRAIL_H

#include <QGraphicsItem>
#include <QLinkedList>

class BodyTrail : public QGraphicsItem
{
    QGraphicsItem *body;
    QLinkedList<QPointF> path;
    bool active;
    int length;
    qreal minX, maxX, minY, maxY;


public:
    BodyTrail(QGraphicsItem* attachedTo, int length=100);

    int len() const { return length; }
    bool disabled() const { return !active; }
    void disable(bool disable=true) { active = !disable; }
    void setLen(int newLen) { length = newLen; }
    void updateTrail();

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // BODYTRAIL_H
