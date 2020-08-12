#include "dbhelper.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

#include "spacebody.h"
#include "physvector.h"

DbHelper::DbHelper()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("saves.db3");
    if (!db.open()){
        QMessageBox::critical(nullptr,"Ошибка SQlite", db.lastError().text());
        return;
    }

    if (db.tables().size() == 0){
        db.exec("create table saves ("
                "save_name text, "
                "mass real, "
                "radius real, "
                "velocity real, "
                "velocity_angle real, "
                "pos_x real, "
                "pos_y real,"
                "static integer)");
    }
}

DbHelper::~DbHelper()
{
    db.close();
}

QStringList DbHelper::saveNames() const
{
    QStringList returnList;

    QSqlQuery query = db.exec("select distinct save_name from saves");
    while(query.next()){
        returnList << query.value(0).toString();
    }
    return returnList;
}

void DbHelper::save(QString save_name, QList<SpaceBody*> items)
{
    for (SpaceBody* item : items){
        QSqlQuery query(db);
        query.prepare("insert into saves values("
                      ":name,"
                      ":mass,"
                      ":radius,"
                      ":velocity,"
                      ":angle,"
                      ":x,"
                      ":y,"
                      ":static)");
        query.bindValue(0, save_name);
        query.bindValue(1, item->getMass());
        query.bindValue(2, item->getRadius());
        query.bindValue(3, item->getVelocity().length());
        query.bindValue(4, item->getVelocity().angle());
        query.bindValue(5, item->pos().x());
        query.bindValue(6, item->pos().y());
        query.bindValue(7, item->isStatic());
        query.exec();
    }
}

QList<SpaceBody*> DbHelper::load(QString saveName) const
{
    QList<SpaceBody*> items;

    QSqlQuery query(db);
    query.prepare("select * from saves where save_name=:name");
    query.bindValue(":name", saveName);
    query.exec();

    while (query.next()){
        SpaceBody* body = new SpaceBody(
                              query.value(5).toDouble(),
                              query.value(6).toDouble(),
                              query.value(1).toDouble(),
                              query.value(2).toDouble(),
                              query.value(3).toDouble(),
                              query.value(4).toDouble(),
                              query.value(7).toBool());
        items << body;
    }

    return items;
}

void DbHelper::removeSave(QString saveName)
{
    QSqlQuery query(db);
    query.prepare("delete from saves where save_name = :name");
    query.bindValue(":name", saveName);
    query.exec();
}
