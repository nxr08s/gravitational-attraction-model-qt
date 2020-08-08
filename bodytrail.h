#ifndef BODYTRAIL_H
#define BODYTRAIL_H

#include <QGraphicsItem>
#include <QLinkedList>
#include <QObject>

class BodyTrail : public QObject, public QGraphicsItem
{
    Q_OBJECT

    QGraphicsItem *_body;
    QLinkedList<QPointF> _path;
    bool _active;
    int _length;

public:
    BodyTrail(QGraphicsItem* attachedTo);

    int len() const { return _length; }
    bool disabled() const { return !_active; }
    void disable(bool disable=true) { _active = !disable; }
    void setLen(int newLen) { _length = newLen; }
    void updateTrail();

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

public slots:
    void trailLengthChanged(int length);

};

#endif // BODYTRAIL_H
