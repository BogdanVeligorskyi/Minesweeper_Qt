#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <QString>

SettingsWindow::SettingsWindow(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    this->numOfMines = 10;
    this->boardSize = "10x10";
}

// destructor
SettingsWindow::~SettingsWindow() {
    delete ui;
}


void SettingsWindow::on_pushSaveButton_clicked() {
    QString strNumOfMines = ui->lineEditNumOfMines->text();
    QString strBoardSize = ui->lineEditSize->text();
    this->numOfMines = strNumOfMines.toInt();
    this->boardSize = strBoardSize;
    this->hide();
}
