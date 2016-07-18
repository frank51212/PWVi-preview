#include "mylineedit.h"
//#include <QProcess>
//#include <QDir>

MyLineEdit::MyLineEdit(QWidget *parent) : QLineEdit(parent)
{}

MyLineEdit::~MyLineEdit()
{}

// For auto pop-up keyboard
//void MyLineEdit::focusInEvent(QFocusEvent *e) {
//    QLineEdit::focusInEvent(e);
//    qDebug("focus is on");
//    // open keyboard
//    //QDir dir(getenv("windir"));
//    //QString path = dir.absoluteFilePath("../Program Files/Common Files/microsoft shared/ink/TabTip.exe");
//    QString path(getenv("windir"));
//    qDebug(path.toLatin1());
//    path.append("/System32/osk.exe");
//    qDebug(path.toLatin1());
//    QProcess *process = new QProcess(this);
//    process->start("C:/Windows/System32/osk.exe");

//}

//void MyLineEdit::focusOutEvent(QFocusEvent *e) {
//    QLineEdit::focusOutEvent(e);
//    // close keyboard
//    qDebug("focus is off");
//}
