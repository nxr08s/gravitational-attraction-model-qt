#ifndef DBHELPER_H
#define DBHELPER_H

#include <QSqlDatabase>

class SpaceBody;
class QGraphicsItem;

class DbHelper
{
    QSqlDatabase db;

public:
    DbHelper();
    ~DbHelper();

    void save(QString, QList<SpaceBody*>);
    QList<SpaceBody*> load(QString) const;
    void removeSave(QString);
    QStringList saveNames() const;
};

#endif // DBHELPER_H
