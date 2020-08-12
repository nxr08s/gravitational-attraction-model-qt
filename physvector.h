#ifndef PHYSVECTOR_H
#define PHYSVECTOR_H

#include <QtCore>

class PhysVector : public QLineF
{
public:
    static PhysVector reversed(PhysVector& vec){ return PhysVector(vec.p2(), vec.p1()); }

    PhysVector() {};
    PhysVector(qreal x2, qreal y2) : QLineF(qreal(0), qreal(0), x2, y2) {};
    PhysVector(qreal x1, qreal y1, qreal x2, qreal y2) : QLineF(x1, y1, x2, y2) {};
    PhysVector(QPointF p1, QPointF p2) : QLineF(p1, p2) {};

    qreal xProjection() const { return length() * cos(qDegreesToRadians(angle())); };
    qreal yProjection() const { return length() * sin(qDegreesToRadians(angle())); };

    PhysVector operator+(PhysVector& vec){ return PhysVector(
                    this->x1() + vec.x1(),
                    this->y1() + vec.y1(),
                    this->x2() + vec.x2(),
                    this->y2() + vec.y2());}

    PhysVector operator/(qreal num){
        PhysVector newVector(p1(), p2());
        newVector.setLength(newVector.length() / num);
        return newVector;
    }
};

#endif // PHYSVECTOR_H
