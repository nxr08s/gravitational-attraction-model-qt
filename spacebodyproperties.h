#ifndef SPACEBODYPROPERTIES_H
#define SPACEBODYPROPERTIES_H

#include <QtCore>
#include "physvector.h"

struct BodyProperties
{
    qreal mass;
    qreal radius;
    PhysVector velocity;
    QPointF pos;
};

#endif // SPACEBODYPROPERTIES_H
