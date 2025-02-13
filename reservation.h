#ifndef RESERVATION_H
#define RESERVATION_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Reservation
{
    int ID, CIN,MATRICULE,ID_emp;
    double duree;
    QString nom, prenom, mail,DateHeure;
public:
    void showQueryResultsInMessageBox();
    //Consteruteurs
    Reservation(){}
    Reservation(int ID, int CIN, int MATRICULE, int ID_emp, double duree,QString nom, QString prenom, QString mail, QString DateHeure)

    {
        this->ID = ID;
        this->CIN = CIN;
        this->MATRICULE = MATRICULE;
        this->ID_emp = ID_emp;
        this->duree = duree;
        this->nom = nom;
        this->prenom = prenom;
        this->mail = mail;
        this->DateHeure = DateHeure;
    }

     //Getters and Setters
     int GetID() { return ID; }
     void SetID(int val) { ID = val; }
     int GetCIN() { return CIN; }
     void SetCIN(int val) { CIN = val; }
     int GetMATRICULE() { return MATRICULE; }
     void SetMATRICULE(int val) { MATRICULE = val; }
     int GetID_emp() { return ID_emp; }
     void SetID_emp(int val) { ID_emp = val; }
     double Getduree() { return duree; }
     void Setduree(double val) { duree = val; }
     QString Getnom() { return nom; }
     void Setnom(QString val) { nom = val; }
     QString Getprenom() { return prenom; }
     void Setprenom(QString val) { prenom = val; }
     QString Getmail() { return mail; }
     void Setmail(QString val) { mail = val; }
     QString GetDateHeure() { return DateHeure; }
     void SetDateHeure(QString val) { DateHeure = val; }
     //Functions
     bool ajouter();
     QSqlQueryModel * afficher(QString cin = "",int trie = 0);
     QSqlQueryModel * afficher_historique();
     QSqlQueryModel * afficher_coordinates();
     bool supprimer(int);
     bool modifier(int id);
     double calculer_stat();
};

#endif // RESERVATION_H
