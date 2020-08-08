#ifndef RULER_H
#define RULER_H

#include <QGraphicsItem>
#include <QPoint>

class Ruler : public QGraphicsItem
{
    QPoint start;

public:
    Ruler(QPoint point);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // RULER_H
