#include "select.h"
#include "ui_select.h"
#include "next.h"
#include "myimage.h"
#include <QMessageBox>
#include <QStringList>
#include <QSettings>
#include <QTime>
#include <QCoreApplication>
#include <QFileInfo>
#include <QThread>

Select::Select(QWidget *parent, QDir parentDir) : QDialog(parent), ui(new Ui::Select), play(false) {
    ui->setupUi(this);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.2); // Scale window to 20% screen size
    ui->comboBox->setMinimumHeight(3*QFontMetrics(QFont("MS Shell Dlg 2", 9)).height()); // Size combobox to fit text
    ui->pushButton->setMinimumHeight(3*QFontMetrics(QFont("MS Shell Dlg 2", 9)).height());
    ui->radioButton->setMinimumHeight(3*QFontMetrics(QFont("MS Shell Dlg 2", 9)).height());
    ui->viewfinder->setMinimumHeight(3*QFontMetrics(QFont("MS Shell Dlg 2", 9)).height());
    dir = parentDir;
    ui->comboBox->addItem("\nSelect anatomic session\n");
    // Read in sites and capture arguments from config file
    QSettings settings("config.ini", QSettings::IniFormat);
    args = settings.value("select/args","").toStringList();
    QStringList sites = settings.value("select/sites","").toStringList();
    // Add sites with linebreaks to make selection easier
    for (int i = 0; i < sites.size(); ++i)
        ui->comboBox->addItem(QString(sites.at(i)).prepend("\n").append("\n"));

    QProcess::execute("setParams.exe", args); // set camera parameters

    thread = new QThread(this);
    camera = new MyCamera();

    camera->moveToThread(thread);

    connect(camera, SIGNAL(grabDone(QPixmap)), this, SLOT(updateViewfinder(QPixmap)));
    //connect(camera, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), camera, SLOT(startGrabbing()));
    connect(camera, SIGNAL(finished()), thread, SLOT(quit()));

    if (camera->connect()) {
        camera->open();
        ui->viewfinder->setText("Camera Ready...");
    }
    else {
        ui->viewfinder->setText("Could not connect to camera.");
        ui->previewButton->setText("Connect");
    }
}

Select::~Select() {
    emit camera->finished();
    delete camera;
    delete thread;
    delete capture;
    delete ui;
}

void Select::closeEvent(QCloseEvent *) {
    this->parentWidget()->parentWidget()->activateWindow();
}

void Select::on_pushButton_clicked() {
    QString text = ui->comboBox->currentText().trimmed(); // Remove linebreaks from selection
    if (ui->comboBox->currentIndex() == 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("No site selected.");
        msgBox.exec();
        return;
    }
    QString side = "NA";
    if(ui->radioButton->isChecked())
        side = "L";
    else if(ui->radioButton_2->isChecked())
        side = "R";
    else {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("No side selected.");
        msgBox.exec();
        return;
    }
    text.append("-").append(side);

    // Check if file exists
    QFileInfo file(dir.path().append("/").append(text).append("/myImage.bin"));
    if (file.exists() && file.isFile()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("This observation already exists.");
        msgBox.setInformativeText("Do you want to overwrite?");
        msgBox.setStandardButtons(QMessageBox::Cancel);
        msgBox.addButton("Overwrite",QMessageBox::AcceptRole);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        if (msgBox.exec() != 0)
            return;
    }

    dir.mkdir(text); // Create site folder site-L/R
    dir.cd(text);

    QString path = dir.path().append("/myImage.bin");

    capture = new QProcess(this);
    connect(capture, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(captureFinished(int)));

    f = new Wait(this);
    f->setModal(true);
    f->setWindowTitle("Warning");
    f->show();
    delay(5000); // Wait 5 seconds before recording
    camera->stopGrabbing();
    camera->close();
    ui->previewButton->setText("Preview");
    ui->previewButton->repaint();
    f->recordStart(); // Change window to "Recording!"

    capture->start("myGrabcpp.exe", QStringList(path) + args);
}

void Select::delay(int n) {
    QTime time;
    time.start();
    while (time.elapsed() < n)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
}

void Select::updateViewfinder(QPixmap image) {
    //ui->viewfinder->setPixmap(image.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->viewfinder->setPixmap(image);
    ui->viewfinder->repaint();
}

void Select::on_previewButton_clicked()
{
    if (ui->previewButton->text() == "Connect") {
        if (camera->connect()) {
            camera->open();
            ui->viewfinder->setText("Camera Ready...");
            ui->previewButton->setText("Preview");
            ui->viewfinder->repaint();
            ui->previewButton->repaint();
        }
    }
    else {
        if (!play) {
            if (!camera->isOpen())
                camera->open();
            thread->start();
            ui->previewButton->setText("Pause");
            ui->previewButton->repaint();
            play = true;
        }
        else {
            camera->stopGrabbing();
            ui->previewButton->setText("Preview");
            ui->previewButton->repaint();
            play = false;
        }
    }
}

void Select::captureFinished(int exitCode) {
    QString output = "Output:\n" +
            capture->readAllStandardOutput() +
            "\n--------------------\n\nErrors:\n" +
            capture->readAllStandardError();

    f->close();
    dir.cd("..");
    QString status;
    switch (exitCode) {
    case -2:
        status = "The video capturing routine could not be started.";
        break;
    case -1:
        status = "The video capturing routine finished unexpectedly.";
        break;
    case 0:
        status = "The video has been recorded successfully!";
        break;
    case 1:
        status = "The video capturing routine finished unexpectedly.";
        break;
    case 2:
        status = "The video capturing routine finished unexpectedly.";
        break;
    case 3:
        status = "The video capturing routine finished unexpectedly.";
        break;
    default:
        status = "42";
        break;
    }

    n = new Next(this);
    n->setWindowModality(Qt::WindowModal);
    n->setWindowTitle("Finished!");
    n->setText(status);
    n->setDetailedText(output);
    n->show();
}
