#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStringList>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include "connection.h" // Make sure this is the correct path to your Connection class



int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);



    return app.exec();
}
