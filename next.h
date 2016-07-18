#ifndef NEXT_H
#define NEXT_H

#include <QDialog>
#include <QDesktopWidget>
#include <QTextBrowser>

namespace Ui {
class Next;
}

class Next : public QDialog {
    Q_OBJECT

public:
    explicit Next(QWidget *parent = 0);
    ~Next();
    void setText(QString exitText);
    void setDetailedText(QString detailedText);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_detailsButton_clicked();

private:
    Ui::Next *ui;
    bool flag;
};

#endif // NEXT_H
