#include "reservation.h"

#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>

bool Reservation::ajouter()
{
    QSqlQuery query;

    // Your original insertion SQL command
    QString insertSql = QString("INSERT INTO Mohamed.RESERVATION(CIN, NOM, PRENOM, MAIL, DATE_HEURE, DUREE, MATRICULE, ID_EMPLYE) "
                          "VALUES ('%1', '%2', '%3', '%4', TO_TIMESTAMP('%5', 'YYYY-MM-DD HH24:MI:SS.FF6'), NUMTODSINTERVAL('%6', 'SECOND'), '%7', %8)")
                    .arg(this->GetCIN())
                    .arg(this->Getnom())
                    .arg(this->Getprenom())
                    .arg(this->Getmail())
                    .arg(this->GetDateHeure()) // Replace with actual date and time if available
                    .arg(this->Getduree()) // Replace with actual duration if available
                    .arg(this->GetMATRICULE())
                    .arg(this->GetID_emp());

    qDebug() << "Executing query:" << insertSql;
    bool success = query.exec(insertSql);

    if (success) {
        // Assuming you have a way to retrieve the ID of the newly added reservation.
        // If your database supports returning the last-inserted ID, fetch it here.
        int newReservationId = query.lastInsertId().toInt();

        // Now insert into history table
        QSqlQuery historyQuery;

        historyQuery.prepare("INSERT INTO Mohamed.historique(Reservation_ID, Action_Type, Changed_Data) VALUES (:ID, 'INSERT',:DATA)");
        historyQuery.bindValue(":ID", newReservationId);
        historyQuery.bindValue(":DATA", insertSql);
        if (!historyQuery.exec()) {
            qDebug() << "Error adding to history:" << historyQuery.lastError().text();
        }

        // ... Your email sending code ...

    } else {
        qDebug() << "Error adding reservation:" << query.lastError().text();
    }
    return success;
}




QSqlQueryModel *Reservation::afficher(QString cin, int trie) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    QString queryString;

    if(cin.isEmpty()) {
        queryString = "SELECT ID_RESERVATION,CIN,NOM,PRENOM,MAIL,DATE_HEURE FROM Mohamed.RESERVATION";
    } else {
        queryString = "SELECT ID_RESERVATION, CIN, NOM, PRENOM, MAIL, DATE_HEURE FROM Mohamed.RESERVATION WHERE ID_RESERVATION LIKE :CIN";
    }

    // Add ORDER BY clause based on the trie value
    switch(trie) {
        case 1:
            queryString += " ORDER BY CIN";
            break;
        case 2:
            queryString += " ORDER BY NOM DESC";
            break;
        case 3:
            queryString += " ORDER BY DATE_HEURE DESC";
            break;
        // Add more cases as needed for different sorting options
    }

    if(!cin.isEmpty()) {
        query.prepare(queryString);
        query.bindValue(":CIN", cin + '%');
        query.exec(); // Execute the query
        model->setQuery(query);
    } else {
        model->setQuery(queryString);
    }

    // Check for error
    QSqlError err = model->lastError();
    if (err.type() != QSqlError::NoError) {
        qDebug() << "SQL Query:" << model->query().lastQuery();
        qDebug() << "SQL Error:" << err.text();
        return nullptr;
    }

    // Set header data for each column in the model
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date_Heure"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Employe"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Matricule"));

    return model;
}



bool Reservation::supprimer(int id)
{
    QSqlQuery query;
    QString historySql = "DELETE FROM Mohamed.RESERVATION WHERE ID_RESERVATION = :ID";
    query.prepare(historySql);
    query.bindValue(":ID", id);
    if (!query.exec()) {
        qDebug() << "Error adding deletion to history:" << query.lastError().text();
        return false;


    }
    query.prepare("INSERT INTO Mohamed.historique(Reservation_ID, Action_Type, Changed_Data) VALUES (:ID, 'DELETE',:DATA)");
    query.bindValue(":ID", id);
    query.bindValue(":DATA", historySql);
    if (!query.exec()) {
        qDebug() << "Error adding deletion to history:" << query.lastError().text();

    }
        return true;

    // Proceed with the

}


bool Reservation::modifier(int id)
{
    QSqlQuery query;

    QString sql = QString("UPDATE Mohamed.RESERVATION SET "
                          "ID_RESERVATION=%1, "
                          "CIN='%2', "
                          "NOM='%3', "
                          "PRENOM='%4', "
                          "MAIL='%5', "
                          "DATE_HEURE=TO_TIMESTAMP('%6', 'YYYY-MM-DD HH24:MI:SS.FF6'), "
                          "DUREE=NUMTODSINTERVAL('%7', 'SECOND'), "
                          "MATRICULE='%8', "
                          "ID_EMPLYE=%9 "
                          "WHERE ID_RESERVATION=%10")
                    .arg(this->GetID())
                    .arg(this->GetCIN())
                    .arg(this->Getnom())
                    .arg(this->Getprenom())
                    .arg(this->Getmail())
                    .arg(this->GetDateHeure()) // Replace with actual date and time if available
                    .arg(this->Getduree()) // Replace with actual duration if available
                    .arg(this->GetMATRICULE())
                    .arg(this->GetID_emp())
                    .arg(id);

    qDebug() << "Error updating reservation:" << sql;
    bool success = query.exec(sql);
    if (success) {
        // If the update is successful, log it in the history table
        QSqlQuery historyQuery;
        historyQuery.prepare("INSERT INTO Mohamed.historique(Reservation_ID, Action_Type, Changed_Data) "
                             "VALUES (:ID, 'UPDATE', :Data)");
        historyQuery.bindValue(":ID", id);
        historyQuery.bindValue(":Data", sql); // You can also choose to log only the changed fields

        if (!historyQuery.exec()) {
            qDebug() << "Error adding update to history:" << historyQuery.lastError().text();
            // Decide if you want to fail the operation if history logging fails
            return false;
        }
    } else {
        qDebug() << "Error updating reservation:" << query.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel *Reservation::afficher_historique()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    QString sql = "SELECT * FROM Mohamed.HISTORIQUE ORDER BY ACTION_TIMESTAMP DESC";
    query.prepare(sql);

    if (!query.exec()) {
        // Only log the error if the query execution fails
        qDebug() << "Error executing history display query:" << query.lastError().text();
    } else {
        // If the query was successful, set the query to the model
        model->setQuery(query);

        // Set header data for each column
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("History ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Reservation ID"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Action Type"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Action Timestamp"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Changed Data"));
    }

    return model;
}


double Reservation::calculer_stat(){

    QSqlQuery queryClients;

    int totalDistinctClients = 0;
    if (queryClients.exec("SELECT COUNT(DISTINCT CIN) FROM reservation")) {
        if (queryClients.next()) {
            totalDistinctClients = queryClients.value(0).toInt();
        }
    } else {
        qDebug() << "Query failed:" << queryClients.lastError();
    }

    QSqlQuery queryReservations;
    int totalReservations = 0;
    if (queryReservations.exec("SELECT COUNT(*) FROM reservation")) {
        if (queryReservations.next()) {
            totalReservations = queryReservations.value(0).toInt();
        }
    } else {
        qDebug() << "Query failed:" << queryReservations.lastError();
    }

    // Assuming totalReservations is greater than zero to avoid division by zero
    double percentageOfClients = 0.0;
    if (totalReservations > 0) {
        percentageOfClients = (static_cast<double>(totalDistinctClients) / totalReservations) * 100;
        return percentageOfClients;
    }
    return 0.0;
}

QSqlQueryModel *Reservation::afficher_coordinates()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    QString sql = "SELECT * FROM Mohamed.GPS_COORDINATES";
    query.prepare(sql);

    if (!query.exec()) {
        // Only log the error if the query execution fails
        qDebug() << "Error executing history display query:" << query.lastError().text();
    } else {
        // If the query was successful, set the query to the model
        model->setQuery(query);

        // Set header data for each column
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("LATITUDE"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("LONGITUDE"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date de mise a jour"));
    }

    return model;
}
