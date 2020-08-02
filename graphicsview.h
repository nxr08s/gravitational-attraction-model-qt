#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

    bool ctrlPressed;

public:
    GraphicsView(QGraphicsScene* scene);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;
};

#endif // GRAPHICSSCENE_H
