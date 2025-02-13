#include "connection.h"
#include <QSqlDatabase>
#include <QStringList>
#include <QDebug>
#include <QSqlError>

Connection::Connection(){}

bool Connection::createconnection()
{
    db = QSqlDatabase::addDatabase("QODBC");
    bool test =false;
    db.setDatabaseName("test");
    db.setUserName("Mohamed");
    db.setPassword("lmok");

    if(db.open())
    {test = true;
        QSqlDatabase db = QSqlDatabase::database();  // Assuming you have already added and opened a database


    }return test;
}

void Connection::closeConnection(){db.close();}
