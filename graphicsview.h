#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsView>

class Ruler;
class PhysVector;
class SpaceBody;
class BodyTrail;
class QGraphicsProxyWidget;

class GraphicsView : public QGraphicsView
{   
    Q_OBJECT
public:
    enum MouseMode { Edit, Measure, Move };

private:
    MouseMode _mode;
    QGraphicsScene *_graphicsScene;
    QList<SpaceBody*> _bodies;
    QList<BodyTrail*> _trails;
    qreal _scale;
    bool _ctrlPressed;
    int _frameRate;
    int _trailLength;
    int _timeScale;
    int _timerId;
    bool _tailEnabled;
//    Ruler* _ruler;

    QGraphicsItem *_currentItem;
    QGraphicsProxyWidget *_propertiesWgt;

    static PhysVector calculateForce(SpaceBody *s1, SpaceBody *s2) ;
    static void processCollision(SpaceBody*, SpaceBody*);

public:
    GraphicsView(QWidget *parent);
    SpaceBody* addItem(QPoint pos, qreal mass, qreal xVel, qreal yVel, qreal radius);
    void timerEnable(bool enable);
    int frameRate() const { return _frameRate; };
    int trailLength() const { return _trailLength; };
    int timeScale() const { return _timeScale; };
    void setTimeScale(int scale) { _timeScale = scale; };

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void timerEvent(QTimerEvent* event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mousePressEvent(QMouseEvent *event) override;

public slots:
    void setFrameRate(int rate);
    void setMouseMode(MouseMode _mode);
    void zoomIn();
    void zoomOut();
    void vectorChecked(bool enabled);
//    void gridChecked(bool enabled);
    void setTrailLength(int length);
    void propertiesWidnowDestroyed();
    void itemRemoved(QGraphicsItem*);

signals:
    void trailLengthChanged(int length);
    void vectorEnabled(bool enabled);
};

#endif // GRAPHICSSCENE_H
