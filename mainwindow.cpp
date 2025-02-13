#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reservation.h"
#include <QMessageBox>
#include <QApplication>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QFileDialog>
#include <QtCore>
#include "SmtpClient/src/SmtpMime"
#include <QCoreApplication>
#include <QProgressBar>
#include <QTime>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QStringList>
#include <QTextStream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    help();

}





MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::help(){
    Reservation res;
    ui->progressBar->setValue(0);
    ui->progressBar_2->setValue(0);
    ui->progressBar_3->setValue(0);
        connect(ui->btnAjouter, &QPushButton::clicked, this, &MainWindow::on_btnAjouter_clicked);
        connect(ui->btnSupprimer, &QPushButton::clicked, this, &MainWindow::on_btnSupprimer_clicked);
        connect(ui->btnModifier, &QPushButton::clicked, this, &MainWindow::on_btnModifier_clicked);
        connect(ui->lineEdit_CIN_2, &QLineEdit::textChanged, this, &MainWindow::on_lineEdit_CIN_textChanged);
        connect(ui->radio_CIN, &QRadioButton::toggled, this, &MainWindow::on_radioButton_toggled);
        connect(ui->radio_alpha, &QRadioButton::toggled, this, &MainWindow::on_radioButton_2_toggled);
        connect(ui->radio_Date, &QRadioButton::toggled, this, &MainWindow::on_radioButton_3_toggled);
        connect(ui->nb_client, &QPushButton::clicked, this, &MainWindow::on_lcdNumber_overflow);

        connect(ui->Telecharger_button, &QPushButton::clicked, this, &MainWindow::on_pushButton_Telecharger_clicked);

        ui->afficher_historique->setModel(res.afficher_historique());
        ui->afficher_reservation->setModel(res.afficher());

        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery query;

        // This is an example of using the text to filter the data, adjust the query as needed.
        QString sql = "SELECT ID_RESERVATION FROM Mohamed.RESERVATION";



        if(!query.exec(sql)) {
            qDebug() << "Query failed:" << query.lastError();
            return;
        }

        model->setQuery(query);
        ui->combo_reservation->setModel(model);
        //chart
        chart();
        //Arduino coordinates

         ui->coordinates->setModel(res.afficher_coordinates());
}

void MainWindow::on_btnAjouter_clicked()
{
    Reservation res;

    bool conversionSuccess;

    // For CIN
    int cinValue = ui->CIN_input->text().toInt(&conversionSuccess);
    if (!conversionSuccess) {
        QMessageBox::critical(this, "Error", "Invalid CIN entered!");
        return;
    }
    res.SetCIN(cinValue);

    int matriculeValue = ui->select_voiture->currentText().toInt(&conversionSuccess);

    res.SetMATRICULE(matriculeValue);

    int idEmpValue = ui->select_Employer->currentText().toInt(&conversionSuccess);

    res.SetID_emp(idEmpValue);

    double dureeValue = ui->doubleSpinBox->value();
    res.Setduree(dureeValue);

    res.Setnom(ui->NOM_input->text());
    res.Setprenom(ui->PRENOM_input->text());
    res.Setmail(ui->MAIL_input->text());

    res.SetDateHeure(ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss"));
    res.SetMATRICULE(101);
       res.SetID_emp(1);


    if(res.ajouter()) {
        ui->btnAjouter->setEnabled(false);
        ui->afficher_reservation->setModel(res.afficher()); // Refresh the model
        QMessageBox::information(this, "Success", "Reservation added successfully!");

    } else {
        QString resDetails = QString(
            "CIN: %1\n"
            "Nom: %2\n"
            "Prenom: %3\n"
            "Mail: %4\n"
            "DateHeure: %5\n"
            "Duree: %6\n"
            "MATRICULE: %7\n"
            "ID_emp: %8")
            .arg(res.GetCIN())
            .arg(res.Getnom())
            .arg(res.Getprenom())
            .arg(res.Getmail())
            .arg(res.GetDateHeure())
            .arg(QString::number(res.Getduree()))
            .arg(res.GetMATRICULE())
            .arg(res.GetID_emp());

        QMessageBox::critical(this, "Error", "Could not add reservation!\n\n" + resDetails);
    }
    ui->btnAjouter->setEnabled(true);
}


void MainWindow::on_btnSupprimer_clicked()
{
    int id = ui->supp_ID->text().toInt();
    Reservation res;
    if(res.supprimer(id)) {
        QMessageBox::information(this, "Success", "Reservation deleted successfully!");
    ui->afficher_reservation->setModel(res.afficher()); // Refresh the model
    } else {
        // Building the details string
        QString resDetails = QString(
            "CIN: %1\n"
            "Nom: %2\n"
            "Prenom: %3\n"
            "Mail: %4\n"
            "DateHeure: %5\n"
            "Duree: %6\n"
            "MATRICULE: %7\n"
            "ID_emp: %8")
            .arg(res.GetCIN())
            .arg(res.Getnom())
            .arg(res.Getprenom())
            .arg(res.Getmail())
            .arg(res.GetDateHeure())
            .arg(res.Getduree())
            .arg(res.GetMATRICULE())
            .arg(res.GetID_emp());

        QMessageBox::critical(this, "Error", "Could a not add reservation!\n\n" + resDetails);
    }
}

void MainWindow::on_btnModifier_clicked()
{
    // Get the ID of the reservation to be updated.
    bool conversionOk;
    int id = ui->lineEdit_ID->text().toInt(&conversionOk); // Replace 'lineEdit_ID' with the actual object name in your UI
    if (!conversionOk) {
        QMessageBox::critical(this, "Error", "Invalid ID entered!");
        return;
    }

    // Create a reservation object and set the new values from the UI.
    Reservation res;
    res.SetID(ui->lineEdit_ID->text().toInt());
    res.SetCIN(ui->lineEdit_CIN->text().toInt()); // Assuming lineEdit_CIN is the name of your QLineEdit
    res.Setnom(ui->lineEdit_Nom->text());
    res.Setprenom(ui->lineEdit_prenom->text());
    res.Setmail(ui->lineEdit_Mail->text());
    res.SetDateHeure(ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd HH:mm:ss"));
    res.Setduree(ui->doubleSpinBox_2->value()); // Assuming doubleSpinBox_Duree is your QDoubleSpinBox
    res.SetMATRICULE(ui->select_voiture->currentText().toInt());
    res.SetID_emp(ui->select_Employer->currentText().toInt()); // Assuming select_voiture_IdEmp is your Qselect_voiture for employee ID
    res.SetMATRICULE(101);
       res.SetID_emp(1);
    // Call the update method
    if (res.modifier(id)) {
        ui->afficher_reservation->setModel(res.afficher()); // Refresh the model
        QMessageBox::information(this, "Success", "Reservation updated successfully!");
        // Refresh the table view, if necessary, to reflect the updated reservation

    } else {
        QMessageBox::critical(this, "Error", "Could not update reservation. Please check the details and try again.");
    }
}









void MainWindow::on_afficher_reservation_viewportEntered(const QModelIndex &index)
{
    // Example functionality: Populate line edits for editing when a row in the table is clicked.
    // Assuming lineEdit_ID, lineEdit_CIN, etc. are QLineEdit widgets for editing the reservation.

    // Fetch the selected record's data.
    QString id = ui->afficher_reservation->model()->data(ui->afficher_reservation->model()->index(index.row(), 0)).toString();
    QString cin = ui->afficher_reservation->model()->data(ui->afficher_reservation->model()->index(index.row(), 1)).toString();
    QString nom = ui->afficher_reservation->model()->data(ui->afficher_reservation->model()->index(index.row(), 2)).toString();
    QString prenom = ui->afficher_reservation->model()->data(ui->afficher_reservation->model()->index(index.row(), 3)).toString();
    QString mail = ui->afficher_reservation->model()->data(ui->afficher_reservation->model()->index(index.row(), 4)).toString();
    QString date_eur = ui->afficher_reservation->model()->data(ui->afficher_reservation->model()->index(index.row(), 5)).toString();
    QString duree = ui->afficher_reservation->model()->data(ui->afficher_reservation->model()->index(index.row(), 6)).toString();
    QString matricule = ui->afficher_reservation->model()->data(ui->afficher_reservation->model()->index(index.row(), 7)).toString();
    QString employee = ui->afficher_reservation->model()->data(ui->afficher_reservation->model()->index(index.row(), 8)).toString();
    // ... Fetch other fields in a similar way

    // Populate the form with the fetched data for editing.
    ui->lineEdit_ID->setText(id);
    ui->lineEdit_CIN->setText(cin);
    ui->lineEdit_Nom->setText(nom);
    ui->lineEdit_prenom->setText(prenom);
    ui->lineEdit_ID->setText(mail);
    ui->lineEdit_CIN->setText(date_eur);
    ui->lineEdit_Nom->setText(duree);
    ui->lineEdit_prenom->setText(matricule);
    ui->lineEdit_prenom->setText(employee);
    // ... Populate other fields in a similar way

}


void MainWindow::on_lineEdit_CIN_textChanged(const QString &text) {

        QSqlQueryModel *model = myReservation.afficher(text); // Now myReservation is known
        ui->afficher_reservation->setModel(model); // Update the table view

}




void MainWindow::on_radioButton_toggled(bool checked)
{
    if(checked){
         qDebug() << "SQL Query:" << "hello"; // Log the actual query
             QString text = ui->lineEdit_CIN_2->text();
             qDebug() << "SQL Query:" << text;
             QSqlQueryModel *model = myReservation.afficher(text,1);
             ui->afficher_reservation->setModel(model);
    }
}

void MainWindow::on_radioButton_2_toggled(bool checked)
{
    if(checked){
        QString text = ui->lineEdit_CIN_2->text();
        QSqlQueryModel *model = myReservation.afficher(text,2);
        ui->afficher_reservation->setModel(model);
    }
}

void MainWindow::on_radioButton_3_toggled(bool checked)
{
    if(checked){
        QString text = ui->lineEdit_CIN_2->text();
        QSqlQueryModel *model = myReservation.afficher(text,3);
        ui->afficher_reservation->setModel(model);
    }
}


void delay(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs(millisecondsToWait);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
void MainWindow::on_lcdNumber_overflow()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(DISTINCT CIN) FROM reservation");
    if(query.exec()) {
        if(query.next()) {
            int count = query.value(0).toInt();
            for(int i = 0;i<count;i ++){
                ui->lcdNumber->display(i);
                delay(56);
            }
            ui->lcdNumber->setStyleSheet("QLCDNumber { background-color: white; color: green; }");
            ui->lcdNumber->display(count-1);
            qDebug() << "There are" << count << "distinct CINs in the table.";
        }
    } else {
        qWarning() << "SQL query failed:" << query.lastError().text();
    }
}


void exportTableToPDF(QSqlDatabase& db, const QString& tableName, const QString& pdfFileName) {
    // Step 1: Fetch Data
    QSqlQuery query(db);
    query.prepare(QString("SELECT ID_RESERVATION,CIN,NOM,PRENOM,MAIL,DATE_HEURE  FROM %1").arg(tableName));

    if(!query.exec()) {
        qWarning() << "SQL query failed:" << query.lastError().text();
        return;
    }

    // Step 2: Format Data
    QString htmlString = "<table border='1' cellspacing='0' cellpadding='3'>";
    htmlString += "<tr>";
    for (int i = 0; i < query.record().count(); ++i) {
        htmlString += QString("<th>%1</th>").arg(query.record().fieldName(i));
    }
    htmlString += "</tr>";

    while (query.next()) {
        htmlString += "<tr>";
        for (int i = 0; i < query.record().count(); ++i) {
            htmlString += QString("<td>%1</td>").arg(query.value(i).toString());
        }
        htmlString += "</tr>";
    }
    htmlString += "</table>";

    QTextDocument document;
    document.setHtml(htmlString);

    // Step 3: Render to PDF
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(pdfFileName);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document.print(&printer);
}


void MainWindow::on_pushButton_Telecharger_clicked() {
    // Set the progress bar to 0
    ui->progressBar->setValue(0);

    // Fetch the data and export to PDF (this can be improved with a QThread for large data)
    QSqlDatabase db = QSqlDatabase::database(); // Make sure you have opened the database before this line
    QString pdfFileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("PDF Files (*.pdf)"));
    if (!pdfFileName.isEmpty()) {
        if (!pdfFileName.endsWith(".pdf", Qt::CaseInsensitive)) {
            pdfFileName += ".pdf";
        }

        exportTableToPDF(db, "reservation", pdfFileName);

        for(int i = 0;i<101;i ++){
            ui->progressBar->setValue(i);
            delay(56);
        }
        // Optional: Show a message that the PDF was created
        QMessageBox::information(this, tr("Export Successful"), tr("The data was exported to PDF successfully."));
        ui->progressBar->setValue(0);
    }
}


void MainWindow::envoyer_mail(const QString& recipientEmail, const QString& subject, const QString& messageText)
{

    // This is a first demo application of the SmtpClient for Qt project

    // First we need to create an SmtpClient object
    // We will use the Gmail's smtp server (smtp.gmail.com, port 465, ssl)

    SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);

    // We need to set the username (your email address) and the password
    // for smtp authentification.

    smtp.setUser("transporteco25@gmail.com");
    smtp.setPassword("ecotravel1999");

    // Now we create a MimeMessage object. This will be the email.

    MimeMessage message;

    message.setSender(new EmailAddress("transporteco25@gmail.com", "ecotravel1999"));
    message.addRecipient(new EmailAddress(recipientEmail, "Recipient's Name"));
    message.setSubject(subject);

    // Now add some text to the email.
    // First we create a MimeText object.

    MimeText text;

    text.setText(messageText);

    // Now add it to the mail

    message.addPart(&text);

    // Now we can send the mail

    smtp.connectToHost();
    smtp.login();
    smtp.sendMail(message);
    smtp.quit();

}


void exportTableToPDF1(QSqlDatabase& db, const QString& tableName, const QString& pdfFileName) {
    // Step 1: Fetch Data
    QSqlQuery query(db);
    query.prepare(QString("SELECT * FROM %1").arg(tableName));

    if(!query.exec()) {
        qWarning() << "SQL query failed:" << query.lastError().text();
        return;
    }

    // Step 2: Format Data
    QString htmlString = "<table border='1' cellspacing='0' cellpadding='3'>";
    htmlString += "<tr>";
    for (int i = 0; i < query.record().count(); ++i) {
        htmlString += QString("<th>%1</th>").arg(query.record().fieldName(i));
    }
    htmlString += "</tr>";

    while (query.next()) {
        htmlString += "<tr>";
        for (int i = 0; i < query.record().count(); ++i) {
            htmlString += QString("<td>%1</td>").arg(query.value(i).toString());
        }
        htmlString += "</tr>";
    }
    htmlString += "</table>";

    QTextDocument document;
    document.setHtml(htmlString);

    // Step 3: Render to PDF
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(pdfFileName);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document.print(&printer);
}


void MainWindow::on_envoyer_mail_clicked()
{

    for(int i = 0;i<101;i ++){
        ui->progressBar_3->setValue(i);
        delay(56);
    }
     ui->progressBar_3->setValue(100);
    QMessageBox::information(this, "Success", "le mail a ete envoyer");
}

QFile Serial("C:/Users/MSI/Documents/New folder/xy_values.txt");
void MainWindow::on_pushButton_TELECHARGER_clicked()
{
    ui->progressBar_2->setValue(0);

    // Fetch the data and export to PDF (this can be improved with a QThread for large data)
    QSqlDatabase db = QSqlDatabase::database(); // Make sure you have opened the database before this line
    QString pdfFileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("PDF Files (*.pdf)"));
    if (!pdfFileName.isEmpty()) {
        if (!pdfFileName.endsWith(".pdf", Qt::CaseInsensitive)) {
            pdfFileName += ".pdf";
        }

        exportTableToPDF1(db, "Mohamed.HISTORIQUE", pdfFileName);

        for(int i = 0;i<101;i ++){
            ui->progressBar_2->setValue(i);
            delay(56);
        }
        ui->progressBar_2->setValue(100);

        // Optional: Show a message that the PDF was created
        QMessageBox::information(this, tr("Export Successful"), tr("The data was exported to PDF successfully."));
        ui->progressBar_2->setValue(0);
}
}

void MainWindow::chart(){
    Reservation R;


    double percentageOfClients = R.calculer_stat();
    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.35);

    // Add the percentage of clients to the series
    QPieSlice *clientsSlice = series->append(QString("Clients %1%").arg(percentageOfClients), percentageOfClients);

    clientsSlice->setExploded();
    clientsSlice->setLabelVisible();

    // Add the remaining percentage as "Others" to make up the full 100%
    double otherPercentage = 100.0 - percentageOfClients;
    QPieSlice *otherSlice = series->append(QString("Autres %1%").arg(otherPercentage), otherPercentage);
    otherSlice->setLabelVisible();

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTitle("Client Reservation Percentage");
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    chart->setMinimumSize(QSize(600, 471));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->chart_graph);

}

void MainWindow::on_tabWidget_currentChanged()
{
    chart();
}
QSerialPort serial;



void MainWindow::mise_a_jour(int id) {
    QString data = read_from_arduino();
    if (!Serial.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the file";
        return;
    }

    QStringList lines;
    QTextStream in(&Serial);
    while (!in.atEnd()) {
        lines << in.readLine();
    }
    Serial.close();

    // Ensure we have at least four lines
    if (lines.size() < 4) {
        qDebug() << "Not enough data in the file";
        return;
    }

    QStringList values[4];  // Array to store the last four sets of values
    for (int i = 0; i < 4; ++i) {
        QString line = lines.at(lines.size() - 4 + i);
        QStringList parts = line.split('|');
        if (parts.size() == 2 && parts[0] == "LIMIT") {
            values[i] = parts[1].split(' ');
        }
    }

    // Construct the SQL statement
    QString sql = QString("UPDATE Mohamed.gps_coordinates SET \"x1\" = %1, \"y1\" = %2, \"x2\" = %3, \"y2\" = %4, \"x3\" = %5, \"y3\" = %6, \"x4\" = %7, \"y4\" = %8 WHERE id = %9")
                    .arg(values[0][1]).arg(values[0][3])
                    .arg(values[1][1]).arg(values[1][3])
                    .arg(values[2][1]).arg(values[2][3])
                    .arg(values[3][1]).arg(values[3][3])
                    .arg(id);
    qDebug() << "Update failed: " << sql;
    QSqlQuery query;
    if(!query.exec(sql)) {
        qDebug() << "Update failed: " << query.lastError();
    } else {
        qDebug() << "Update successful";
    }
}


void MainWindow::handleSerialData() {
    static QStringList limitPoints;
    mise_a_jour(1);
    while (serial.canReadLine()) {
        QString line = serial.readLine();
        line = line.trimmed(); // Remove newline characters

        // Check if the line starts with the expected prefix "LIMIT,"
        if (line.startsWith("LIMIT,")) {
            // Remove the prefix and then split the rest by comma
            line = line.mid(6); // Remove "LIMIT," which is 6 characters long
            limitPoints << line; // Collect each point

        }
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    // Configure the serial port
    serial.setPortName("COM5");
    serial.setBaudRate(QSerialPort::Baud9600);



    handleSerialData();

}

bool MainWindow::get_gps_coordinates(int id, QStringList &coordinates) {

    Reservation res;
    int f = write_to_arduino();

    QString sql = QString("SELECT x1, y1, x2, y2, x3, y3, x4, y4 FROM Mohamed.gps_coordinates WHERE id = 1");
    QSqlQuery query;
 f = 2;
    if(!query.exec(sql)) {
        qDebug() << "Query failed: " << query.lastError();
        return false;
    }

    if(query.next()) {
        for (int i = 0; i < 8; ++i) {
            coordinates << query.value(i).toString();
        }
    } else {
        qDebug() << "No data found for ID:" << id;
        return false;
    }
    ui->coordinates->setModel(res.afficher_coordinates());

    return true;
}

void send_gps_coordinates(const QStringList &coordinates) {
    if(serial.isOpen()) {
        QString dataToSend = coordinates.join(",") + "\n";
        serial.write(dataToSend.toUtf8());
    } else {
        qDebug() << "Serial port is not open";
    }
}
void MainWindow::on_pushButton_4_clicked()
{
    serial.setPortName("COM5"); // Replace with the actual COM port
    serial.setBaudRate(QSerialPort::Baud9600);

    Reservation res;

    // Assuming the ID of the data you want to retrieve is 1
    QStringList coordinates;
    if(get_gps_coordinates(1, coordinates)) {
        qDebug() << "Retrieved coordinates:" << coordinates;
        send_gps_coordinates(coordinates);
    }
    ui->coordinates->setModel(res.afficher_coordinates());


}
