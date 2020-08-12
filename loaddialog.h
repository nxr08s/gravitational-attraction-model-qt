#ifndef LOADDIALOG_H
#define LOADDIALOG_H

#include <QDialog>

class QListWidgetItem;
class QListWidget;
class DbHelper;

class LoadDialog : public QDialog
{
    Q_OBJECT

    QPushButton* acceptBtn;
    QPushButton* removeBtn;
    QListWidget *listWgt;
    QString resultStr;
    DbHelper *db;

public:
    LoadDialog(QWidget *parent, DbHelper*);
    QString resultName() const;

public slots:
    void itemSelected(QListWidgetItem*);
    void onClickRemove();
    void onClickLoad();
    void onDblClick(QListWidgetItem*);
};

#endif // LOADDIALOG_H
