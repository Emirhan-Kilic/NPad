#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QShortcut>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_actionChangeMode_triggered(QLabel *text);

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_actionExit_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionSave_triggered();

    void on_actionshare_triggered();

    void on_actionShare_ctxt_triggered();

    void on_actionCheckBulletPoint_triggered();

    void on_actionModeCycle_triggered();

    void on_actioncheckCharNumber_triggered();

    void on_actionCheckChar_triggered(QLabel *text);

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    int mode = 0;
};
#endif // MAINWINDOW_H
