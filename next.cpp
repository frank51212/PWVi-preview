#include "next.h"
#include "ui_next.h"

Next::Next(QWidget *parent) : QDialog(parent), ui(new Ui::Next), flag(false) {
    ui->setupUi(this);
    ui->widget->hide();
    resize(QDesktopWidget().availableGeometry(this).size() * 0.1); // Scale window to 10% screen size
    ui->pushButton->setDefault(true);
    ui->pushButton->setMinimumHeight(3*QFontMetrics(QFont("MS Shell Dlg 2", 9)).height());
    ui->label->setMinimumHeight(3*QFontMetrics(QFont("MS Shell Dlg 2", 9)).height());
}

Next::~Next() {
    delete ui;
}

void Next::on_pushButton_clicked() {
    this->close();
    this->parentWidget()->activateWindow();
}

void Next::on_pushButton_2_clicked() {
    this->close();
    this->parentWidget()->close();
    this->parentWidget()->parentWidget()->parentWidget()->activateWindow();
}

void Next::setText(QString exitText) {
    ui->label->setText(exitText);
}

void Next::setDetailedText(QString detailedText) {
    ui->textBrowser->setText(detailedText);
}

void Next::on_detailsButton_clicked()
{
    if (!flag) {
        ui->widget->setVisible(true);
        ui->detailsButton->setText("Hide Details...");
        flag = true;
    }
    else {
        ui->widget->setVisible(false);
        ui->detailsButton->setText("Show Details...");
        flag = false;
    }
}

