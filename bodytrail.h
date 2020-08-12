#ifndef BODYTRAIL_H
#define BODYTRAIL_H

#include <QGraphicsItem>
#include <QObject>
#include <QLinkedList>
#include <QPainterPath>

class BodyTrail : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

    QPainterPath _path;
    QLinkedList<QPointF> _points;
    int _length;

public:
    BodyTrail(QGraphicsItem* attachedTo);

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    int type() const override { return UserType + 1; };

public slots:
    void setLength(int length);
    void updateTrail();

};

#endif // BODYTRAIL_H
