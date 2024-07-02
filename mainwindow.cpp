#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cpr/cpr.h>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q), this, SLOT(close()));

    QShortcut *shortcutSave = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this);
    connect(shortcutSave, &QShortcut::activated, this, &MainWindow::on_actionSave_triggered);

    QShortcut *shortcutMenu = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_M), this);
    connect(shortcutMenu, &QShortcut::activated, this, &MainWindow::on_actionshare_triggered);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionshare_triggered()
{
    if(ui->menubar->isVisible()){
        ui->menubar->hide();
        ui->statusbar->hide();
    } else{
        ui->menubar->show();
        ui->statusbar->show();
    }

}



void MainWindow::on_actionNew_triggered()
{

    currentFile.clear();
    ui->textEdit->setText(QString());
    setWindowTitle("New File");

}


void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open the file");

    QFile file(filename);
    currentFile = filename;

    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"Warning","Cant Open The File: " + file.errorString());
        return;
    }

    setWindowTitle(filename);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}


void MainWindow::on_actionSave_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(filename);

    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"Warning","Cant Save The File: " + file.errorString());
        return;
    }
    currentFile = filename;
    setWindowTitle(filename);
    QTextStream out(&file);

    QString text = ui->textEdit->toPlainText();

    out << text;
    file.close();
}

void MainWindow::on_actionSave_triggered()
{

    if(currentFile.trimmed().isEmpty()){
        MainWindow::on_actionSave_as_triggered();
        return;
    }

    QFile file(currentFile);

    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"Warning","Cant Save The File: " + file.errorString());
        return;
    }
    setWindowTitle(currentFile);
    QTextStream out(&file);

    QString text = ui->textEdit->toPlainText();

    out << text;
    file.close();
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();

}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();

}





void MainWindow::on_actionShare_ctxt_triggered()
{




    QString body = ui->textEdit->toPlainText();


    cpr::Response r = cpr::Post(cpr::Url{"https://filebin.net/testhahahaha/test"},
                                cpr::Header{
                                    {"accept", "application/json"},
                                    {"cid", "1"},
                                    {"Content-Type", "application/octet-stream"}
                                },
                                cpr::Body{body.toStdString()});


    if (r.error) {
        QMessageBox::critical(this, "Error", r.error.message.c_str());
    } else {
        QString url = QString::fromStdString(r.url.str());
        QInputDialog inputDialog(this);
        inputDialog.setInputMode(QInputDialog::TextInput);
        inputDialog.setLabelText("Filebin URL:");
        inputDialog.setTextValue(url);
        inputDialog.setOptions(QInputDialog::NoButtons);  // Hide buttons
        inputDialog.exec();
    }

}

