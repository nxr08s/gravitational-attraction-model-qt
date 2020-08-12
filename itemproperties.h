#ifndef ITEMPROPERTIES_H
#define ITEMPROPERTIES_H

#include <QWidget>

class QGraphicsItem;
class SpaceBody;
class QPushButton;
class QLineEdit;
class QCheckBox;

class ItemProperties : public QWidget
{
    Q_OBJECT

    SpaceBody *body;

    QPushButton *applyBtn;
    QPushButton *removeBtn;
    QLineEdit *massEdit;
    QLineEdit *radiusEdit;
    QLineEdit *velocityEdit;
    QCheckBox *staticCheckBox;

public:
    ItemProperties(QGraphicsItem* item, QWidget *parent = nullptr);

public slots:
    void onClickApply();
    void onClickRemove();
    void onTriggerStatic(int);

signals:
    void itemRemove(QGraphicsItem*);

};

#endif // ITEMPROPERTIES_H
