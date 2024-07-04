#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cpr/cpr.h>
#include <QInputDialog>
#include <QLabel>
#include <QTextListFormat>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    currentFile.clear();
    ui->textEdit->setText(QString());
    setWindowTitle("New File");



    QWidget *statusContainer = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(statusContainer);

    QLabel *statuslabelChar = new QLabel("Character Count: 0", this);
    QLabel *statuslabelMode = new QLabel("Mode: Edit", this);


    layout->addWidget(statuslabelChar);
    layout->setContentsMargins(5,1,5,1);
    layout->setSpacing(15);
    statusContainer->setLayout(layout);
    ui->statusbar->addWidget(statusContainer);

    statusBar()->addPermanentWidget(statuslabelMode);



    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q), this, SLOT(close()));

    QShortcut *shortcutSave = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this);
    connect(shortcutSave, &QShortcut::activated, this, &MainWindow::on_actionSave_triggered);

    QShortcut *shortcutMenu = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_M), this);
    connect(shortcutMenu, &QShortcut::activated, this, &MainWindow::on_actionshare_triggered);

    QShortcut *shortcutNew = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_N), this);
    connect(shortcutNew, &QShortcut::activated, this, &MainWindow::on_actionNew_triggered);


    QShortcut *shortcutOpen = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_O), this);
    connect(shortcutOpen, &QShortcut::activated, this, &MainWindow::on_actionOpen_triggered);


    connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::on_actionCheckBulletPoint_triggered);

    connect(ui->textEdit, &QTextEdit::textChanged, this, [this, statuslabelChar] {
        on_actionCheckChar_triggered(statuslabelChar);
    });

    on_actionCheckChar_triggered(statuslabelChar);

}

void MainWindow::on_actionCheckChar_triggered(QLabel *text)
{
    static QRegularExpression removeNewSpace ("[\\n\\r]");
    int charNumber = ui->textEdit->toPlainText().trimmed().remove(removeNewSpace).length();
    text->setText(QString("Character Count: %1").arg(charNumber));
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

    QString text = ui->textEdit->toPlainText();


    if(currentFile.trimmed().isEmpty() && !text.trimmed().isEmpty() ){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Not Saved", "Your File is Not Saved!\nDo you want to proceed with the process?",
                                      QMessageBox::Ok | QMessageBox::Cancel);

        if (reply == QMessageBox::Cancel) {
            return;
        }
    }


    currentFile.clear();
    ui->textEdit->setText(QString());
    setWindowTitle("New File");

}


void MainWindow::on_actionOpen_triggered()
{
    QString check = ui->textEdit->toPlainText();


    if(currentFile.trimmed().isEmpty() && !check.trimmed().isEmpty() ){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Not Saved", "Your File is Not Saved!\nDo you want to proceed with the process?",
                                      QMessageBox::Ok | QMessageBox::Cancel);

        if (reply == QMessageBox::Cancel) {
            return;
        }
    }


    QString filename = QFileDialog::getOpenFileName(this, "Open the file");

    QFile file(filename);
    currentFile = filename;

    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"Warning","Cant Open The File: " + file.errorString());
        return;
    }

    int lastSlashIndex = filename.lastIndexOf('/');
    QString nameOnly = filename.mid(lastSlashIndex + 1);

    setWindowTitle(nameOnly);
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

    int lastSlashIndex = filename.lastIndexOf('/');
    QString nameOnly = filename.mid(lastSlashIndex + 1);

    setWindowTitle(nameOnly);
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


    int lastSlashIndex = currentFile.lastIndexOf('/');
    QString nameOnly = currentFile.mid(lastSlashIndex + 1);

    setWindowTitle(nameOnly);
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

    bool ok;
    QString fileName = QInputDialog::getText(this, "Title", "Enter a file name to share with:", QLineEdit::Normal, "", &ok);

    if (!ok | fileName.isEmpty()) {
        QMessageBox::warning(this,"Title","Please try a new title!");
        return;
    }

    bool ok2;
    QString binName = QInputDialog::getText(this, "Bin", "Enter Bin Name to upload to, \nLong names are recommended!", QLineEdit::Normal, "", &ok2);

    if (!ok2 | binName.isEmpty()) {
        QMessageBox::warning(this,"Bin Name","Please try a new Bin Name!");
        return;
    }


    cpr::Url url{"https://filebin.net/" + binName.toStdString() + "/" + fileName.toStdString()};
    QString body = ui->textEdit->toPlainText();


    cpr::Response r = cpr::Post(url,
                                cpr::Header{
                                    {"accept", "application/json"},
                                    {"Content-Type", "application/octet-stream"}
                                },
                                cpr::Body{body.toStdString()});


    if (r.error) {
        QMessageBox::critical(this, "Error", r.error.message.c_str());
        return;
    } else {
        QString url = QString::fromStdString(r.url.str());

        int lastSlashIndex = url.lastIndexOf('/');

        if (lastSlashIndex != -1) {
            url.truncate(lastSlashIndex);
        }

        QInputDialog inputDialog(this);
        inputDialog.setInputMode(QInputDialog::TextInput);
        inputDialog.setLabelText("Filebin URL:");
        inputDialog.setTextValue(url);
        inputDialog.setOptions(QInputDialog::NoButtons);  // Hide buttons


        // Make the input field read-only
        QLineEdit *lineEdit = inputDialog.findChild<QLineEdit *>();
        if (lineEdit) {
            lineEdit->setReadOnly(true);

            // Calculate the required width for the text
            QFontMetrics fm(inputDialog.font());
            int textWidth = fm.horizontalAdvance(url) + 50; // Add some padding

            // Set the dialog width
            inputDialog.resize(textWidth, inputDialog.sizeHint().height());
        }

        inputDialog.exec();
    }

}


void MainWindow::on_actionCheckBulletPoint_triggered()
{

    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);

    // Get the current block text
    cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
    QString blockText = cursor.selectedText();

    if(blockText.size() > 3 ||  blockText.size() <= 0)
        return;


    if (blockText.startsWith("**")) {
        // Remove the "**" and set the cursor position to the start of the line
        cursor.removeSelectedText();
        cursor.insertText("");

        // Insert bullet list
        QTextListFormat listFormat;
        listFormat.setStyle(QTextListFormat::ListCircle);
        cursor.insertList(listFormat);
        ui->textEdit->setTextCursor(cursor);
    }
}


void MainWindow::on_actionModeCycle_triggered()
{

}


