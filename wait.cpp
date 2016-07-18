#include "wait.h"
#include "ui_wait.h"

Wait::Wait(QWidget *parent) : QDialog(parent), ui(new Ui::Wait) {
    ui->setupUi(this);
}

Wait::~Wait() {
    delete ui;
}

void Wait::recordStart() {
    ui->label->setText("Recording!");
    ui->label->repaint();
}