#include "mainwindow.h"
#include <QMessageBox>
#include <QApplication>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

        Connection c;
        bool test=c.createconnection();
        MainWindow w;
        if(test)
        {w.show();
            QMessageBox::information(nullptr, QObject::tr("database is open"),
                        QObject::tr("connection successful.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);

    }
        else
            QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                        QObject::tr("connection failed.\n""Click Cancel to exit."), QMessageBox::Cancel);



        return a.exec();

}
