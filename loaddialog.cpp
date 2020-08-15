#include "loaddialog.h"

#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

#include "dbhelper.h"

LoadDialog::LoadDialog(QWidget* parent, DbHelper* database)
    : QDialog(parent)
    , db(database)
{
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowTitle("Загрузить");

    auto mainLt = new QVBoxLayout;
    setLayout(mainLt);

    listWgt = new QListWidget(this);
    listWgt->addItems(db->saveNames());
    mainLt->addWidget(listWgt);

    auto lt = new QHBoxLayout;
    mainLt->addLayout(lt);

    acceptBtn = new QPushButton("Загрузить", this);
    removeBtn = new QPushButton("Удалить", this);

    acceptBtn->setEnabled(false);
    removeBtn->setEnabled(false);

    lt->addWidget(acceptBtn);
    lt->addStretch();
    lt->addWidget(removeBtn);

    connect(listWgt, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(itemSelected(QListWidgetItem*)));
    connect(removeBtn, SIGNAL(clicked()), this, SLOT(onClickRemove()));
    connect(acceptBtn, SIGNAL(clicked()), this, SLOT(onClickLoad()));
    connect(listWgt, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(onDblClick(QListWidgetItem*)));

    connect(listWgt, SIGNAL(itemSelectionChanged()),
            this, SLOT(itemSelectionChanged()));
}

QString LoadDialog::resultName() const
{
    return resultStr;
}

void LoadDialog::itemSelected(QListWidgetItem*)
{
    acceptBtn->setEnabled(true);
    removeBtn->setEnabled(true);
}

void LoadDialog::onClickRemove()
{
    QMessageBox::StandardButton result =
            QMessageBox::question(
                nullptr, "Удалить сохранения",
                "Удалить сохранение '" + listWgt->currentItem()->text() + "'?");

    if (result == QMessageBox::Yes){
        db->removeSave(listWgt->currentItem()->text());
        listWgt->takeItem(listWgt->currentRow());
    }
}

void LoadDialog::onClickLoad()
{
    resultStr = listWgt->currentItem()->text();
    accept();
}

void LoadDialog::onDblClick(QListWidgetItem *item)
{
    resultStr = item->text();
    accept();
}

void LoadDialog::itemSelectionChanged()
{
    if (listWgt->currentItem() == nullptr){
        acceptBtn->setEnabled(false);
        removeBtn->setEnabled(false);
    }
}

