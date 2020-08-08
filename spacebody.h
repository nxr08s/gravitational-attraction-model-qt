#ifndef SPACEBODY_H
#define SPACEBODY_H

#include <QtCore>
#include <QGraphicsItem>
#include "physvector.h"

class SpaceBody
        : public QObject
        , public QGraphicsItem
{
    Q_OBJECT

    qreal mass;
    qreal radius;
    PhysVector velocity;
    PhysVector totalForce;
    bool _drawVelocityVector;

public:
    SpaceBody() : QGraphicsItem() { setPos(QPoint(10, 10)); };
    SpaceBody(QPointF _pos, qreal _mass, qreal xvel, qreal yvel, qreal rad)
        : QGraphicsItem()
        , mass(_mass)
        , radius(rad)
        , velocity(PhysVector(0, 0, xvel, yvel))
        , totalForce(PhysVector(0, 0, 0, 0))
        , _drawVelocityVector(false)
    {
        setPos(_pos);
        setZValue(2);
    }

    void setMass(qreal newMass) { mass = newMass; };
    void setRadius(qreal newRad) { radius = newRad; };
    void setVelocity(PhysVector &newVel) { velocity = PhysVector(newVel); };
    void setVelocity(qreal vel) { velocity.setLength(vel); };

    qreal getMass() const { return mass; }
    qreal getRadius() const { return radius; }
    PhysVector getVelocity() const { return velocity; }

    void addForce(PhysVector force) { totalForce = PhysVector(totalForce + force); }
    void applyForce();

protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

public slots:
    void enableVector(bool enable);
};

#endif // SPACEBODY_H
