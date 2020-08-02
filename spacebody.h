#ifndef SPACEBODY_H
#define SPACEBODY_H

#include <QtCore>
#include <QGraphicsItem>
#include "physvector.h"

class SpaceBody : public QGraphicsItem
{
    qreal mass;
    qreal radius;
    PhysVector velocity;
    PhysVector totalForce;

public:
    SpaceBody() : QGraphicsItem() { setPos(QPoint(10, 10)); };
    SpaceBody(QPointF _pos, qreal _mass, qreal xvel, qreal yvel, qreal rad)
        : QGraphicsItem()
        , mass(_mass)
        , radius(rad)
        , velocity(PhysVector(0, 0, xvel, yvel))
        , totalForce(PhysVector(0, 0, 0, 0))
    {
        setPos(_pos);
        setZValue(2);
    }

    void setMass(qreal newMass) { mass = newMass; };
    void setRadius(qreal newRad) { radius = newRad; };
    void setVelocity(PhysVector &newVel) { velocity = PhysVector(newVel); };

    qreal getMass() const { return mass; }
    qreal getRadius() const { return radius; }
    PhysVector getVelocity() const { return velocity; }

    void addForce(PhysVector force) { totalForce = PhysVector(totalForce + force); }
    void applyForce();

protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
};

#endif // SPACEBODY_H
