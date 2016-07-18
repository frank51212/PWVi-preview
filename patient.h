#ifndef PATIENT_H
#define PATIENT_H

#include <QDialog>
#include <select.h>

extern Select *s;

namespace Ui {
class Patient;
}

class Patient : public QDialog {
    Q_OBJECT

public:
    explicit Patient(QWidget *parent = 0, QDir parentDir = QDir::currentPath());
    ~Patient();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Patient *ui;
    Select *s;
    QDir dir;
};

#endif // PATIENT_H
