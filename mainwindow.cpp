#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "patient.h"
#include <QSettings>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("PWVi");
    resize(QDesktopWidget().availableGeometry(this).size() * 0.25); // Scale window to 25% screen size
    ui->pushButton->setDefault(true);
    ui->pushButton->setMinimumHeight(3*QFontMetrics(QFont("MS Shell Dlg 2", 9)).height());
    QSettings settings("config.ini", QSettings::IniFormat); // Read desired results folder from config file
    QString path = settings.value("mainwindow/path", QDir::homePath().append("/PWVi_Results")).toString(); // Use home/PWVi_Results if unset
    if (!QDir(path).exists())
        QDir().mkpath(path);
    dir.cd(path);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    p = new Patient(this, dir);
    p->setModal(true);
    p->setWindowTitle("Create new observation");
    p->show();
}
