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

void ItemProperties::applyChanges()
{
    body->setMass(massEdit->text().toDouble());
    body->setRadius(radiusEdit->text().toDouble());
    body->setVelocity(velocityEdit->text().toDouble());
    body->setStatic(staticCheckBox->isChecked());
}

ItemProperties::ItemProperties(QGraphicsItem *item, QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

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
    mainLt->addLayout(lt);

    lt = new QHBoxLayout();
    lt->addWidget(new QLabel("Радиус",this));
    lt->addWidget(radiusEdit);
    mainLt->addLayout(lt);

    lt = new QHBoxLayout();
    lt->addWidget(new QLabel("Скорость",this));
    lt->addWidget(velocityEdit);
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
}

void ItemProperties::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Return:
        applyChanges();
        break;
    case Qt::Key_Escape:
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }
    close();
}

void ItemProperties::onClickApply()
{
    applyChanges();
    close();
}

void ItemProperties::onClickRemove()
{
    emit itemRemove(body);
    close();
}
