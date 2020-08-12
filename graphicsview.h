#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsView>

class PhysVector;
class SpaceBody;
class BodyTrail;
class QGraphicsProxyWidget;
class QGraphicsItem;
class Trajectory;

class GraphicsView : public QGraphicsView
{   
    Q_OBJECT

public:
    enum MouseMode { None, NewItem, VectorEdit, PropEdit };
    enum Type { Body = 65536, Trail, Traectory };

    QList<SpaceBody*> bodies();

private:
    MouseMode _mode;
    qreal _scale;
    bool _ctrlPressed;
    bool _spacePressed;
    int _frameRate;
    int _trailLength;
    int _timeScale;
    int _timerId;
    bool _tailEnabled;
    bool _vectorEnabled;

    QGraphicsProxyWidget *_propertiesWgt;   // PropEdit : prop widget
    QGraphicsItem *_currentVelocityItem;    // VectorEdit : item, which velocity editing now
    Trajectory *_trajectory;                // VectorEdit : draws approx. body's trajectory

    static PhysVector calculateForce(SpaceBody *s1, SpaceBody *s2) ;
    static void processCollision(SpaceBody*, SpaceBody*);
    QGraphicsItem* getBodyUnderMouse(QPoint);

public:
    GraphicsView(QWidget *parent);
    void addItem(QPointF pos, qreal mass, qreal xVel, qreal yVel, qreal radius);
    void insertItems(QList<SpaceBody*>);
    void timerEnable(bool enable);
    int trailLength() const {
        return _trailLength;
    };
    int timeScale() const {
        return _timeScale;
    };
    inline void setTimeScale(int scale) {
        _timeScale = scale;
        emit timeScaleChanged(timeScale());
    };
    void clearScene();    
    void setMouseMode(MouseMode _mode);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void timerEvent(QTimerEvent* event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public slots:
    void vectorChecked(bool enabled);
    void propertiesWidnowDestroyed();
    void itemRemoved(QGraphicsItem*);
    void zoomIn();
    void zoomOut();
    void setTrailLength(int length);

signals:
    void trailLengthChanged(int length);
    void enableVector(bool enabled);
    void modeChanged(unsigned int);
    void updateTrail();
    void zoomChanged(qreal);
    void timeScaleChanged(int);
};

#endif // GRAPHICSSCENE_H
