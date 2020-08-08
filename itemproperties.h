#ifndef ITEMPROPERTIES_H
#define ITEMPROPERTIES_H

#include <QWidget>

class QGraphicsItem;
class SpaceBody;
class QPushButton;
class QLineEdit;

class ItemProperties : public QWidget
{
    Q_OBJECT

    QGraphicsItem *itm;

    QPushButton *applyBtn;
    QPushButton *cancelBtn;
    QPushButton *removeBtn;
    QLineEdit *massEdit;
    QLineEdit *radiusEdit;
    QLineEdit *velocityEdit;

public:
    explicit ItemProperties(QGraphicsItem* item, QWidget *parent = nullptr);

public slots:
    void onClickCancel();
    void onClickApply();
    void onClickRemove();

signals:
    void itemRemove(QGraphicsItem*);
    void itemChanged(SpaceBody*);
};

#endif // ITEMPROPERTIES_H
