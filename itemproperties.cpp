#include "itemproperties.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include <QGraphicsProxyWidget>

#include "graphicsview.h"
#include "spacebody.h"

ItemProperties::ItemProperties(QGraphicsItem *item, QWidget *parent)
    : QWidget(parent)
    , itm(item)
{
//    QString ss = "QLineEdit {border-bottom: 2px solid black;}";
//    this->setStyleSheet(ss);

    QVBoxLayout *mainLt = new QVBoxLayout(this);
    QHBoxLayout *lt;

    SpaceBody *body = dynamic_cast<SpaceBody*>(item);

    applyBtn = new QPushButton("Принять", this);
    cancelBtn = new QPushButton("Отмена", this);
    removeBtn = new QPushButton("Удалить", this);
    massEdit = new QLineEdit(QString::number(body->getMass()), this);
    radiusEdit = new QLineEdit(QString::number(body->getRadius()), this);
    velocityEdit = new QLineEdit(QString::number(body->getVelocity().length()), this);

    lt = new QHBoxLayout();
    lt->addWidget(new QLabel("Масса", this));
    lt->addWidget(massEdit);
    lt->addWidget(new QLabel("кг", this));
    mainLt->addLayout(lt);

    lt = new QHBoxLayout();
    lt->addWidget(new QLabel("Радиус",this));
    lt->addWidget(radiusEdit);
    lt->addWidget(new QLabel("м", this));
    mainLt->addLayout(lt);

    lt = new QHBoxLayout();
    lt->addWidget(new QLabel("Скорость",this));
    lt->addWidget(velocityEdit);
    lt->addWidget(new QLabel("м/с", this));
    mainLt->addLayout(lt);

    lt = new QHBoxLayout();
    lt->addWidget(applyBtn);
    lt->addWidget(cancelBtn);
    lt->addStretch();
    lt->addWidget(removeBtn);
    mainLt->addLayout(lt);

//    setLayout(mainLt);

    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(onClickCancel()));
    connect(applyBtn, SIGNAL(clicked()), this, SLOT(onClickApply()));
    connect(removeBtn, SIGNAL(clicked()), this, SLOT(onClickRemove()));
}

void ItemProperties::onClickCancel()
{
    deleteLater();
}

void ItemProperties::onClickApply()
{
    SpaceBody *body = dynamic_cast<SpaceBody*>(itm);

    body->setMass(massEdit->text().toDouble());
    body->setRadius(radiusEdit->text().toDouble());
    body->setVelocity(velocityEdit->text().toDouble());

    deleteLater();
}

void ItemProperties::onClickRemove()
{
    emit itemRemove(itm);
    deleteLater();
}
