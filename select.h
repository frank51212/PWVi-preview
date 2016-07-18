#ifndef SELECT_H
#define SELECT_H

#include <QDialog>
#include <next.h>
#include <wait.h>
#include <QDir>
#include <QStringList>
#include <mycamera.h>
#include <QProcess>

namespace Ui {
class Select;
}

class Select : public QDialog {
    Q_OBJECT

public:
    explicit Select(QWidget *parent = 0, QDir parentDir = QDir::currentPath());
    ~Select();
    void delay(int n);

protected:
    void closeEvent(QCloseEvent *);

public slots:
    void updateViewfinder(QPixmap image);

private slots:
    void on_pushButton_clicked();

    void on_previewButton_clicked();

    void captureFinished(int exitCode);

private:
    Ui::Select *ui;
    Next *n;
    Wait *f;
    QDir dir;
    QStringList args;
    MyCamera *camera;
    QThread *thread;
    bool play;
    QProcess *capture;
};

#endif // SELECT_H
