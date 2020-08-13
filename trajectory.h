#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <QGraphicsItem>
#include <QDebug>

class SpaceBody;
class PhysVector;

class Trajectory : public QGraphicsItem
{
    const int __length = 500;

    QList<SpaceBody*> _bodies;
    QPainterPath _path;
    QList<QPointF> _points;

public:
    Trajectory(QGraphicsItem *parent, QList<SpaceBody*> &items);
    void updatePath();

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
    int type() const override { return UserType + 2; };
//    QPainterPath shape() const override;

};

#endif // TRAJECTORY_H
