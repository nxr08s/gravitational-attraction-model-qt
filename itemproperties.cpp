#include "itemproperties.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include <QCheckBox>
#include <QGraphicsProxyWidget>
#include <QGroupBox>

#include "graphicsview.h"
#include "spacebody.h"

ItemProperties::ItemProperties(QGraphicsItem *item, QWidget *parent)
    : QWidget(parent)
{
    body = dynamic_cast<SpaceBody*>(item);

    QVBoxLayout *lt1 = new QVBoxLayout(this);
    QGroupBox *box = new QGroupBox("Свойства объекта", this);
    lt1->addWidget(box);

    QVBoxLayout *mainLt = new QVBoxLayout(box);
    QHBoxLayout *lt;

    applyBtn = new QPushButton("Принять", this);
    removeBtn = new QPushButton("Удалить", this);
    massEdit = new QLineEdit(QString::number(body->getMass()), this);
    radiusEdit = new QLineEdit(QString::number(body->getRadius()), this);
    velocityEdit = new QLineEdit(QString::number(body->getVelocity().length()), this);
    staticCheckBox = new QCheckBox("Статичный", this);
    staticCheckBox->setChecked(body->isStatic());

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

    mainLt->addWidget(staticCheckBox);
    mainLt->addWidget(new QFrame(this));

    lt = new QHBoxLayout();
    lt->addWidget(applyBtn);
    lt->addStretch();
    lt->addWidget(removeBtn);
    mainLt->addLayout(lt);

    connect(applyBtn, SIGNAL(clicked()), this, SLOT(onClickApply()));
    connect(removeBtn, SIGNAL(clicked()), this, SLOT(onClickRemove()));
    connect(staticCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onTriggerStatic(int)));
}

void ItemProperties::onClickApply()
{
    body->setMass(massEdit->text().toDouble());
    body->setRadius(radiusEdit->text().toDouble());
    body->setVelocity(velocityEdit->text().toDouble());

    deleteLater();
}

void ItemProperties::onClickRemove()
{
    emit itemRemove(body);
    deleteLater();
}

void ItemProperties::onTriggerStatic(int state)
{
    body->setStatic(state == Qt::Checked);
}
