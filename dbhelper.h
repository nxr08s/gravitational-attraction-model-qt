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

    void save(const QString&, QList<SpaceBody*>);
    QList<SpaceBody*> load(const QString&) const;
    void removeSave(const QString&);
    QStringList saveNames() const;
};

#endif // DBHELPER_H
