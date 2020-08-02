#include "graphicsview.h"
#include <QMouseEvent>

#include <QDebug>
#include <QPainter>

#define Key_Move Qt::Key_Space
#define Key_Zoom Qt::Key_Control

GraphicsView::GraphicsView(QGraphicsScene *scene)
    : QGraphicsView(scene)
    , ctrlPressed(false)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Key_Move:
        if (!event->isAutoRepeat())
            setDragMode(QGraphicsView::ScrollHandDrag);
        break;
    case Key_Zoom:
        if (!event->isAutoRepeat())
            ctrlPressed = true;
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Key_Move:
        if (!event->isAutoRepeat())
            setDragMode(QGraphicsView::RubberBandDrag);
        break;
    case Key_Zoom:
        if (!event->isAutoRepeat())
            ctrlPressed = false;
        break;
    default:
        QGraphicsView::keyReleaseEvent(event);
    }
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    if (ctrlPressed)
        (event->angleDelta().y() > 0) ? (scale(1.2, 1.2)) : (scale(1 / 1.2, 1/ 1.2));
    else
        QGraphicsView::wheelEvent(event);
}

void GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect)

    painter->setOpacity(0.5);
    painter->setPen(Qt::black);
    painter->drawLine(-100, 0, 100, 0);
    painter->drawLine(0, -100, 0, 100);
}
