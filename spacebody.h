#ifndef SPACEBODY_H
#define SPACEBODY_H

#include <QObject>
#include <QGraphicsItem>
#include "physvector.h"

class SpaceBody
        : public QObject
        , public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

    qreal _mass;
    qreal _radius;
    PhysVector _velocity;
    PhysVector _totalForce;
    bool _drawVelocityVector;
    bool _static;   // item not moving, but participate in calculations

public:
    SpaceBody(qreal xPos, qreal yPos,
              qreal mass, qreal radius,
              qreal velocity, qreal angle, int stat=false);
    SpaceBody(QPointF pos, qreal mass, qreal xvel, qreal yvel, qreal rad);
    void setMass(qreal newMass) { _mass = newMass; };
    void setRadius(qreal newRad) { _radius = newRad; };
    void setVelocity(PhysVector &newVel) { _velocity = PhysVector(newVel); };
    void setVelocity(qreal vel) { _velocity.setLength(vel); };
    bool isStatic() const { return _static; }
    void setStatic(bool enabled=true) { _static = enabled; }

    qreal getMass() const { return _mass; }
    qreal getRadius() const { return _radius; }
    PhysVector& getVelocity() /*const*/ { return _velocity; }

    void addForce(PhysVector force);
    void applyForce();

    int type() const override { return UserType; };
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

public slots:
    void enableVector(bool enable) { _drawVelocityVector = enable; };

};

#endif // SPACEBODY_H
