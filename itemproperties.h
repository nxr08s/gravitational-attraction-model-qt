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

    void applyChanges();

public:
    ItemProperties(QGraphicsItem* item, QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void onClickApply();
    void onClickRemove();

signals:
    void itemRemove(QGraphicsItem*);

};

#endif // ITEMPROPERTIES_H
