#include "mainwindow.h"
#include "patient.h"
#include "ui_patient.h"
#include "select.h"
#include <QDateTime>
#include <QmessageBox>

Patient::Patient(QWidget *parent, QDir parentDir) : QDialog(parent), ui(new Ui::Patient) {
    ui->setupUi(this);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.2); // Scale window to 20% screen size
    ui->pushButton_2->setDefault(true);
    //ui->pushButton->setMinimumHeight(3*QFontMetrics(QFont("MS Shell Dlg 2", 9)).height());
    //ui->pushButton_2->setMinimumHeight(3*QFontMetrics(QFont("MS Shell Dlg 2", 9)).height());
    //ui->input_id->setMinimumHeight(3*QFontMetrics(QFont("MS Shell Dlg 2", 9)).height());

    dir = parentDir;
}

Patient::~Patient() {
    delete ui;
}

void Patient::on_pushButton_clicked() {
    this->close();
}

void Patient::on_pushButton_2_clicked() {
    QString patID = ui->input_id->text(); // Get patient ID input
    if (patID.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("No patient ID entered.");
        msgBox.exec();
        return;
    }
    QDateTime datetime = QDateTime::currentDateTime(); // Get current date and time
    QString text = datetime.toString("yyyy.MM.dd-hh.mm.ss");
    text = patID + "-" + text;
    dir.mkdir(text); // Create patient folder with date & time
    dir.cd(text);

    s = new Select(this, dir);
    s->setModal(true);
    s->setWindowTitle("Patient " + patID);
    s->show();
    this->close();
}

