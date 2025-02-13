#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "reservation.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void help();
    void on_btnAjouter_clicked();  // Ajouter
    void on_btnSupprimer_clicked(); // Supprimer
    void on_btnModifier_clicked();
    void on_afficher_reservation_viewportEntered(const QModelIndex &index);
    void on_lineEdit_CIN_textChanged(const QString &text);
    void on_pushButton_Telecharger_clicked();



    void on_radioButton_toggled(bool checked);

    void on_radioButton_2_toggled(bool checked);

    void on_radioButton_3_toggled(bool checked);

    void on_lcdNumber_overflow();

    void envoyer_mail(const QString& recipientEmail, const QString& subject, const QString& messageText);


    void on_envoyer_mail_clicked();

    void on_pushButton_TELECHARGER_clicked();

    void on_tabWidget_currentChanged();

    void chart();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();
    bool get_gps_coordinates(int id, QStringList &coordinates);
    void mise_a_jour(int id);
    void handleSerialData();
    QString read_from_arduino(){
return "";
    }
    int write_to_arduino(){
return 0;
}
private:
    Ui::MainWindow *ui;
    Reservation myReservation;
};

#endif // MAINWINDOW_H
