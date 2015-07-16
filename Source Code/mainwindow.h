#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QTextBlock>
#include <QTextCursor>
#include <QList>
#include <QPen>
#include <QBrush>
#include <QScrollBar>
#include <string>
#include "Specification.h"
#include "Loader.h"
#include "MemoryManager.h"
#include "Registers.h"
#include "NIXBPE.h"
#include "Executor.h"
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // popup window
    static void popupWindows(QString msg);
private slots:
    // button click event
    void on_SelectFileBtn_clicked();

    void on_OneStepBtn_clicked();

    void on_RemoveObpBtn_clicked();

    void on_FullExecBtn_clicked();
private:
    Ui::MainWindow *ui;
    Loader *loader;
    MemoryManager *memory;
    NIXBPE *nixbpe;
    Registers *registers;
    Executor *executor;
    bool OpenFileState;
    bool ProgramState;

    bool validator(QString &input) const;
    void highlightInst(int pos, int len);
};

#endif // MAINWINDOW_H
